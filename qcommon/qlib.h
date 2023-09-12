/*
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

//
// qlib.h
//
// This is a wrapper around HINSTANCE / void* dynamic linking
//

#ifndef ZEALOT_SHARED_LIB_H
#define ZEALOT_SHARED_LIB_H

//============================================================================

#ifdef __cplusplus

extern "C" {

#endif

//============================================================================

#include "qexport.h"

#ifdef WIN32

#include <Windows.h>

typedef HMODULE 	qlib;

#elif

#include <dl.h>
typedef void* 		qlib;

#endif


typedef void*(*qlib_fptr)(void*);

QEXTERN_VAR const char* 		qlib_postfix;


QEXTERN_FUNC qlib QLib_LoadLibrary(const char* path);

QEXTERN_FUNC int QLib_UnloadLibrary(qlib lib);

QEXTERN_FUNC qlib_fptr QLib_GetFuncPtr(qlib lib, const char* func);

//============================================================================

#ifdef __cplusplus

};

#endif

//============================================================================

#endif //ZEALOT_SHARED_LIB_H
