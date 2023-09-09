/*
Copyright (C) 2023 zCubed

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

//
// q_shsdl.c
//
// (for future reference q_sh is an abbreviation for quake_shared...)

#include <SDL.h>

/*
================
Sys_Milliseconds
================
*/
int	curtime;
int Sys_Milliseconds (void)
{
	// TODO: Change Sys_Milliseconds to ms to long? For modern engine things :)
	return SDL_GetTicks();
}