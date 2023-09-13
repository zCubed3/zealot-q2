/*
Copyright (C) 1997-2001 Id Software, Inc., 2023 zCubed3 (Liam R.)

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
// qcore.h - Common definitions, between all modules
//

#ifndef ZEALOT_QCORE_H
#define ZEALOT_QCORE_H

//============================================================================

#ifdef __cplusplus

extern "C" {

#endif

//============================================================================

#include "qlimits.h"

#include "qdefs.h"

#include "qtypes.h"

#include "qmath.h"

#include "qstr.h"

#include "qendian.h"

#include "qhunk.h"

#include "qsys.h"

#include "qcvar.h"

//============================================================================

/*
==============================================================

MISC FUNCTIONS / DEFS

==============================================================
*/

char	*va(char *format, ...);

//============================================================================

#define SFF_ARCH    0x01
#define SFF_HIDDEN  0x02
#define SFF_RDONLY  0x04
#define SFF_SUBDIR  0x08
#define SFF_SYSTEM  0x10

//============================================================================

#ifdef __cplusplus

};

#endif

//============================================================================

#endif //ZEALOT_QCORE_H
