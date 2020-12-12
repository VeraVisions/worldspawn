/*
   Copyright (C) 1999-2007 id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// scriplib.h

#ifndef __CMDLIB__
#include "../common/cmdlib.h"
#endif
#ifndef __MATHLIB__
#include "mathlib.h"
#endif

#define MAXTOKEN    1024

extern char mattoken[MAXTOKEN];
extern int matline;
extern qboolean endofmat;


void LoadMatFile( const char *filename, int index );
void ParseMatMemory( char *buffer, int size );

qboolean GetMatToken( qboolean crossline );
void UnGetMatToken( void );
qboolean MatTokenAvailable( void );

void MatchMatToken( char *match );

void Parse1DMatMatrix( int x, vec_t *m );
void Parse2DMatMatrix( int y, int x, vec_t *m );
void Parse3DMatMatrix( int z, int y, int x, vec_t *m );

void Write1DMatMatrix( FILE *f, int x, vec_t *m );
void Write2DMatMatrix( FILE *f, int y, int x, vec_t *m );
void Write3DMatMatrix( FILE *f, int z, int y, int x, vec_t *m );
