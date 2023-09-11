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

*/
// sdlquake.h: SDL specific Quake header file

#if defined(WIN32)

#include <Windows.h>
#include <dsound.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "../../qcommon/qexport.h"

#include <SDL.h>

extern	HINSTANCE	global_hInstance;

extern LPDIRECTSOUND pDS;
extern LPDIRECTSOUNDBUFFER pDSBuf;

extern DWORD gSndBufSize;

Q_EXTERN_VAR	unsigned	sys_frame_time;

Q_EXTERN_VAR 	HWND		cl_hwnd;
Q_EXTERN_VAR 	SDL_Window	*cl_window;
Q_EXTERN_VAR 	qboolean	ActiveApp, Minimized;

void IN_Activate (qboolean active);

void IN_PollSDL (void);

extern int		window_center_x, window_center_y;
extern RECT		window_rect;

#ifdef __cplusplus
};
#endif