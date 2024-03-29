/*
Copyright (C) 1997-2001 Id Software, Inc.

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

//
// qgl_sdl.cpp
//

/*
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake2 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/

#include "../../../renderers/ref_gl2/gl_local.hpp"

#include "gl2_sdl.hpp"

#ifdef USE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>
#endif

/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void QGL_Shutdown( void )
{
	// TODO: Glad shutdown
}

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to 
** the appropriate GL stuff.  In Windows this means doing a 
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
** 
*/
qboolean QGL_Init( const char *dllname )
{
	//
	// Ensure SDL is loaded
	//

	// SDL hints about our GL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// We must have a valid window beforehand
	// Thanks Windows!
	glw_state.sdl_window = SDL_CreateWindow(
		"Zealot - Quake 2 - OpenGL 2.0",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (!glw_state.sdl_window)
	{
		fprintf( glw_state.log_fp, "*** Failed to create initial SDL window! ***\n" );
		return false;
	}

	glw_state.sdl_gl_ctx = SDL_GL_CreateContext(glw_state.sdl_window);

	if (!glw_state.sdl_gl_ctx)
	{
		ri.Con_Printf(PRINT_ALL, "*** Failed to create SDL GL context! ***\n");
		return false;
	}

	// Load GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		return false;

	gl_config.allow_cds = true;

	// Initialize ImGui (if enabled)
#ifdef USE_IMGUI
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(glw_state.sdl_window, glw_state.sdl_gl_ctx);
	ImGui_ImplOpenGL2_Init();

	glw_state.imgui_ctx = ImGui::GetCurrentContext();

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);
#endif

	return true;
}

void GLimp_EnableLogging( qboolean enable )
{
	if (enable)
		ri.Con_Printf(PRINT_ALL, "[RefGL2]: Logging does not function!");
}


void GLimp_LogNewFrame( void )
{
	ri.Con_Printf(PRINT_ALL, "*** R_BeginFrame ***\n");
}

#pragma warning (default : 4113 4133 4047 )



