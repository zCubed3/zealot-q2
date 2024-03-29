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

#ifndef ZEALOT_SDL_QUAKE_H
#define ZEALOT_SDL_QUAKE_H

//============================================================================

#if defined(WIN32)

#include <Windows.h>
#include <dsound.h>

#endif

#include <SDL.h>

#ifdef WIN32
extern	HINSTANCE	global_hInstance;

extern LPDIRECTSOUND pDS;
extern LPDIRECTSOUNDBUFFER pDSBuf;

extern DWORD gSndBufSize;

extern HWND			cl_hwnd;
#endif

extern SDL_Window	*cl_window;
extern qboolean		ActiveApp, Minimized;

void IN_Activate (qboolean active);

void IN_PollSDL (void);

extern int		window_center_x, window_center_y;

extern unsigned sys_msg_time, sys_frame_time;

#ifdef USE_IMGUI

void VID_InitImGui(void);
void VID_ShutdownImGui(void);

#endif

//============================================================================

#endif