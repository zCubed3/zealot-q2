/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2023 zCubed3 (Liam R.)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// Main windowed and fullscreen graphics interface module. This module
// is used for both the software and OpenGL rendering versions of the
// Quake refresh engine.

#include "../../client/client.h"
#include "../../qcore/qlib.h"

#include "sdlquake.h"

#include "../../client/id_input.hpp"

#include <cassert>

#ifdef USE_IMGUI
#include <imgui.h>
#endif

// Structure containing functions exported from refresh DLL
refexport_t	re;

// Console variables that we need to access from this module
cvar_t		*vid_gamma;
cvar_t		*vid_ref;			// Name of Refresh DLL loaded
cvar_t		*vid_xpos;			// X coordinate of window position
cvar_t		*vid_ypos;			// Y coordinate of window position
cvar_t		*vid_fullscreen;

// Global variables used internally by this module
viddef_t	viddef;				// global video state; used by other modules
qlib		qlib_ref;			// Handle to refresh DLL
qboolean	reflib_active = 0;

#ifdef WIN32
HWND        cl_hwnd;
#endif

SDL_Window	*cl_window;			// SDL window handle

#define VID_NUM_MODES ( sizeof( vid_modes ) / sizeof( vid_modes[0] ) )

extern	unsigned	sys_msg_time;

/*
==========================================================================

DLL GLUE

==========================================================================
*/

#define	MAXPRINTMSG	4096
void VID_Printf (int print_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];
	static qboolean	inupdate;
	
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);

	if (print_level == PRINT_ALL)
	{
		Com_Printf ("%s", msg);
	}
	else if ( print_level == PRINT_DEVELOPER )
	{
		Com_DPrintf ("%s", msg);
	}
	else if ( print_level == PRINT_ALERT )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "PRINT_ALERT", msg, nullptr);
		Sys_ConsoleOutput( msg );
	}
}

void VID_Error (int err_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];
	static qboolean	inupdate;
	
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);

	Com_Error (err_level,"%s", msg);
}

//==========================================================================

void AppActivate(qboolean fActive, qboolean minimize)
{
	Minimized = minimize;

	Key_ClearStates();

	// we don't want to act like we're active if we're minimized
	if (fActive && !Minimized)
		ActiveApp = true;
	else
		ActiveApp = false;

	// minimize/restore mouse-capture on demand
	if (!ActiveApp)
	{
		id_in->Activate(false);
		CDAudio_Activate (false);
		S_Activate (false);
	}
	else
	{
		id_in->Activate(true);
		CDAudio_Activate (true);
		S_Activate (true);
	}
}

/*
============
VID_Restart_f

Console command to re-start the video mode and refresh DLL. We do this
simply by setting the modified flag for the vid_ref variable, which will
cause the entire video mode and refresh DLL to be reset on the next frame.
============
*/
void VID_Restart_f (void)
{
	vid_ref->modified = true;
}

void VID_Front_f( void )
{
	SDL_RaiseWindow(cl_window);
}

/*
** VID_GetModeInfo
*/
typedef struct vidmode_s
{
	const char *description;
	int         width, height;
	int         mode;
} vidmode_t;

vidmode_t vid_modes[] =
{
	{ "Mode 0: 320x240",   		320, 240,   	0 },
	{ "Mode 1: 400x300",   		400, 300,   	1 },
	{ "Mode 2: 512x384",   		512, 384,   	2 },
	{ "Mode 3: 640x480",   		640, 480,   	3 },
	{ "Mode 4: 800x600",   		800, 600,   	4 },
	{ "Mode 5: 960x720",   		960, 720,   	5 },
	{ "Mode 6: 1024x768",  		1024, 768,  	6 },
	{ "Mode 7: 1152x864",  		1152, 864,  	7 },
	{ "Mode 8: 1280x960",  		1280, 960,  	8 },
	{ "Mode 9: 1280x720",   	1280, 720,   	9 },
	{ "Mode 10: 1920x1080",   	1920, 1080,   	10 },
	{ "Mode 11: 2560x1440",   	2560, 1440,   	11 },
};

qboolean VID_GetModeInfo( int *width, int *height, int mode )
{
	if ( mode < 0 || mode >= VID_NUM_MODES )
		return false;

	*width  = vid_modes[mode].width;
	*height = vid_modes[mode].height;

	return true;
}

/*
** VID_UpdateWindowPosAndSize
*/
void VID_UpdateWindowPosAndSize( int x, int y )
{
	SDL_SetWindowSize(cl_window, viddef.width, viddef.height);
	SDL_SetWindowPosition(cl_window, vid_xpos->value, vid_ypos->value);
}

/*
** VID_NewWindow
*/
void VID_NewWindow ( SDL_Window *window, void* extra, int width, int height)
{
	cl_window = window;

	viddef.width  = width;
	viddef.height = height;

	cl.force_refdef = true;		// can't use a paused refdef

	// Extra is imgui
#ifdef USE_IMGUI
	ImGui::SetCurrentContext(static_cast<ImGuiContext*>(extra));
#endif
}

void VID_FreeReflib (void)
{
	if (!QLib_UnloadLibrary(qlib_ref))
		Com_Error( ERR_FATAL, "Reflib QLib_UnloadLibrary failed" );

	memset (&re, 0, sizeof(re));
	qlib_ref = NULL;
	reflib_active  = false;
}

/*
==============
VID_LoadRefresh
==============
*/
qboolean VID_LoadRefresh( char *name )
{
	refimport_t ri;
	GetRefAPI_t GetRefAPI;
	void		*plat_data;

	if (reflib_active)
	{
		re.Shutdown();
		VID_FreeReflib();
	}

	Com_Printf("------- Loading %s -------\n", name);

	qlib_ref = QLib_LoadLibrary(name);
	if (qlib_ref == NULL)
	{
		Com_Printf("QLib_LoadLibrary(\"%s\") failed\n", name);

		return false;
	}

	ri.Cmd_AddCommand 		= Cmd_AddCommand;
	ri.Cmd_RemoveCommand 	= Cmd_RemoveCommand;
	ri.Cmd_Argc		 		= Cmd_Argc;
	ri.Cmd_Argv 			= Cmd_Argv;
	ri.Cmd_ExecuteText 		= Cbuf_ExecuteText;
	ri.Con_Printf 			= VID_Printf;
	ri.Sys_Error 			= VID_Error;
	ri.FS_LoadFile 			= FS_LoadFile;
	ri.FS_FreeFile 			= FS_FreeFile;
	ri.FS_Gamedir 			= FS_Gamedir;
	ri.Cvar_Get 			= Cvar_Get;
	ri.Cvar_Set 			= Cvar_Set;
	ri.Cvar_SetValue 		= Cvar_SetValue;
	ri.Vid_GetModeInfo 		= VID_GetModeInfo;
	ri.Vid_MenuInit 		= VID_MenuInit;
	ri.Vid_NewWindow 		= VID_NewWindow;

	GetRefAPI = (GetRefAPI_t)QLib_GetFuncPtr(qlib_ref, "GetRefAPI");

	if (GetRefAPI == NULL)
		Com_Error(ERR_FATAL, "GetProcAddress failed on %s", name);

	re = GetRefAPI(ri);

	if (re.api_version != API_VERSION)
	{
		VID_FreeReflib();
		Com_Error(ERR_FATAL, "%s has incompatible api_version", name);
	}

#ifdef WIN32
	plat_data = global_hInstance;
#endif

	if (re.Init(plat_data, NULL) == -1)
	{
		re.Shutdown();
		VID_FreeReflib();
		return false;
	}

	Com_Printf("------------------------------------\n");
	reflib_active = true;

//======
//PGM
	vidref_val = VIDREF_OTHER;
	if (vid_ref)
	{
		// Backwards compat
		if (!strcmp(vid_ref->string, "gl"))
			vidref_val = VIDREF_GL;

		if (!strcmp(vid_ref->string, "gl2"))
			vidref_val = VIDREF_GL;

		if (!strcmp(vid_ref->string, "soft"))
			vidref_val = VIDREF_SOFT;

		if (!strcmp(vid_ref->string, "gl4"))
			vidref_val = VIDREF_GL_4;
	}
//PGM
//======

	return true;
}

/*
============
VID_CheckChanges

This function gets called once just before drawing each frame, and it's sole purpose in life
is to check to see if any of the video mode parameters have changed, and if they have to 
update the rendering DLL and/or video mode to match.
============
*/
void VID_CheckChanges (void)
{
	char name[100];

	if ( vid_ref->modified )
	{
		cl.force_refdef = true;		// can't use a paused refdef
		S_StopAllSounds();
	}
	while (vid_ref->modified)
	{
		/*
		** refresh has changed
		*/
		vid_ref->modified = false;
		vid_fullscreen->modified = true;
		cl.refresh_prepped = false;
		cls.disable_screen = true;

		// If using GL, forward to GL2
		if (strcmp(vid_ref->string, "gl") == 0)
		{
			Cvar_Set("vid_ref", "gl2");
			continue;
		}

		Com_sprintf( name, sizeof(name), "%sref_%s%s", qlib_prefix, vid_ref->string, qlib_postfix);

		if ( !VID_LoadRefresh( name ) )
		{
			if (strcmp(vid_ref->string, "soft") == 0)
				Com_Error(ERR_FATAL, "Couldn't fall back to software refresh!");

			Cvar_Set("vid_ref", "soft");

			/*
			** drop the console if we fail to load a refresh
			*/
			if (cls.key_dest != key_console)
			{
				Con_ToggleConsole_f();
			}
		}

		cls.disable_screen = false;
	}

	/*
	** update our window position
	*/
	if ( vid_xpos->modified || vid_ypos->modified )
	{
		if (!vid_fullscreen->value)
			VID_UpdateWindowPosAndSize( vid_xpos->value, vid_ypos->value );

		vid_xpos->modified = false;
		vid_ypos->modified = false;
	}
}

/*
============
VID_Init
============
*/
void VID_Init (void)
{
	/* Create the video variables so we know how to start the graphics drivers */
	vid_ref = Cvar_Get("vid_ref", "soft", CVAR_ARCHIVE);
	vid_xpos = Cvar_Get("vid_xpos", "8", CVAR_ARCHIVE);
	vid_ypos = Cvar_Get("vid_ypos", "8", CVAR_ARCHIVE);
	vid_fullscreen = Cvar_Get("vid_fullscreen", "0", CVAR_ARCHIVE);
	vid_gamma = Cvar_Get("vid_gamma", "1", CVAR_ARCHIVE);

	/* Add some console commands that we want to handle */
	Cmd_AddCommand("vid_restart", VID_Restart_f);
	Cmd_AddCommand("vid_front", VID_Front_f);

	/* Start the graphics mode and load refresh DLL */
	VID_CheckChanges();
}

/*
============
VID_Shutdown
============
*/
void VID_Shutdown (void)
{
	if ( reflib_active )
	{
		re.Shutdown ();
		VID_FreeReflib ();
	}
}

//============================================================================
