/////////////////////////////////////////
//
//             Street Rod 3
//
// Copyright (C) Auxiliary Software 2002-2004
//
//
/////////////////////////////////////////
/*
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


// Standard Definitions
// Created 7/5/02
// Jason Boettcher


#ifndef __DEFS_H__
#define __DEFS_H__


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifndef _LINUX
#include <io.h>
#endif
#include <time.h>
#include <assert.h>
#ifndef _CONSOLE
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#endif

#include "bass.h"
#include <math.h>

#include "types.h"
#include "CommonFunc.h"
#include "KeywordManager.h"
#include "ConfigHandler.h"
#include "CEventQueue.h"
#include "FindFile.h"
#include "CInput.h"

#ifdef _LINUX
#include <sys/stat.h>
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define quad_t quad_t2
typedef unsigned int UINT;
#define _mkdir(X) mkdir(X, 0700)
#define GetForegroundWindow() 0
#define MessageBox(A, B, C, D) fprintf(stderr, B "\n")
#endif



#endif  //  __DEFS_H__
