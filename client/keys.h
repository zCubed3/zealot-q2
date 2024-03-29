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

#include "../qcore/qexport.h"
#include "../qcore/qkeys.h"

extern char		*keybindings[256];
extern	int		key_repeats[256];

extern	int	anykeydown;
extern char chat_buffer[];
extern	int chat_bufferlen;
extern	qboolean	chat_team;

QEXTERN_FUNC void Key_Event (int key, qboolean down, unsigned time);
QEXTERN_FUNC void Key_Init (void);
QEXTERN_FUNC void Key_WriteBindings (FILE *f);
QEXTERN_FUNC void Key_SetBinding (int keynum, char *binding);
QEXTERN_FUNC void Key_ClearStates (void);
QEXTERN_FUNC int Key_GetKey (void);

