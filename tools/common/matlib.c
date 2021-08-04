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

// matlib.c

#include "cmdlib.h"
#include "mathlib.h"
#include "inout.h"
#include "matlib.h"
#include "vfs.h"

/*
   =============================================================================

                        PARSING STUFF

   =============================================================================
 */

typedef struct
{
	char filename[1024];
	char    *buffer,*script_p,*end_p;
	int line;
} mat_t;

#define MAX_INCLUDES    8
mat_t matstack[MAX_INCLUDES];
mat_t    *mat;
int matline;

char mattoken[MAXTOKEN];
qboolean endofmat;
qboolean mattokenready;                     // only qtrue if UnGetMatToken was just called

/*
   ==============
   AddMatToStack
   ==============
 */
void AddMatToStack( const char *filename, int index ){
	int size;
	void* buffer;

	mat++;
	if ( mat == &matstack[MAX_INCLUDES] ) {
		Error( "mat file exceeded MAX_INCLUDES" );
	}
	strcpy( mat->filename, ExpandPath( filename ) );

	size = vfsLoadFile( mat->filename, &buffer, index );

	if ( size == -1 ) {
		Sys_Printf( "Script file %s was not found\n", mat->filename );
		mat--;
	}
	else
	{
		if ( index > 0 ) {
			Sys_Printf( "entering %s (%d)\n", mat->filename, index + 1 );
		}
		else{
			Sys_Printf( "entering %s\n", mat->filename );
		}

		mat->buffer = buffer;
		mat->line = 1;
		mat->script_p = mat->buffer;
		mat->end_p = mat->buffer + size;
	}
}


/*
   ==============
   LoadMatFile
   ==============
 */
void LoadMatFile( const char *filename, int index ){
	mat = matstack;
	AddMatToStack( filename, index );

	endofmat = qfalse;
	mattokenready = qfalse;
}


/*
   ==============
   ParseMatMemory
   ==============
 */
void ParseMatMemory( char *buffer, int size ){
	mat = matstack;
	mat++;
	if ( mat == &matstack[MAX_INCLUDES] ) {
		Error( "mat file exceeded MAX_INCLUDES" );
	}
	strcpy( mat->filename, "memory buffer" );

	mat->buffer = buffer;
	mat->line = 1;
	mat->script_p = mat->buffer;
	mat->end_p = mat->buffer + size;

	endofmat = qfalse;
	mattokenready = qfalse;
}


/*
   ==============
   UnGetMatToken

   Signals that the current mattoken was not used, and should be reported
   for the next GetMatToken.  Note that

   GetMatToken (qtrue);
   UnGetMatToken ();
   GetMatToken (qfalse);

   could cross a line boundary.
   ==============
 */
void UnGetMatToken( void ){
	mattokenready = qtrue;
}


qboolean EndOfMat( qboolean crossline ){
	if ( !crossline ) {
		Error( "Line %i is incomplete\n",matline );
	}

	if ( !strcmp( mat->filename, "memory buffer" ) ) {
		endofmat = qtrue;
		return qfalse;
	}

	if ( mat->buffer == NULL ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Attempt to free already freed mat buffer\n" );
	}
	else{
		free( mat->buffer );
	}
	mat->buffer = NULL;
	if ( mat == matstack + 1 ) {
		endofmat = qtrue;
		return qfalse;
	}
	mat--;
	matline = mat->line;
	Sys_Printf( "returning to %s\n", mat->filename );
	return GetMatToken( crossline );
}

/*
   ==============
   GetMatToken
   ==============
 */
qboolean GetMatToken( qboolean crossline ){
	char    *mattoken_p;


	/* ydnar: dummy testing */
	if ( mat == NULL || mat->buffer == NULL ) {
		return qfalse;
	}

	if ( mattokenready ) {                       // is a mattoken already waiting?
		mattokenready = qfalse;
		return qtrue;
	}

	if ( ( mat->script_p >= mat->end_p ) || ( mat->script_p == NULL ) ) {
		return EndOfMat( crossline );
	}

//
// skip space
//
skipspace:
	while ( *mat->script_p <= 32 )
	{
		if ( mat->script_p >= mat->end_p ) {
			return EndOfMat( crossline );
		}
		if ( *mat->script_p++ == '\n' ) {
			if ( !crossline ) {
				Error( "Line %i is incomplete\n",matline );
			}
			mat->line++;
			matline = mat->line;
		}
	}

	if ( mat->script_p >= mat->end_p ) {
		return EndOfMat( crossline );
	}

	// ; # // comments
	if ( *mat->script_p == ';' || *mat->script_p == '#'
	     || ( mat->script_p[0] == '/' && mat->script_p[1] == '/' ) ) {
		if ( !crossline ) {
			Error( "Line %i is incomplete\n",matline );
		}
		while ( *mat->script_p++ != '\n' )
			if ( mat->script_p >= mat->end_p ) {
				return EndOfMat( crossline );
			}
		mat->line++;
		matline = mat->line;
		goto skipspace;
	}

	// /* */ comments
	if ( mat->script_p[0] == '/' && mat->script_p[1] == '*' ) {
		if ( !crossline ) {
			Error( "Line %i is incomplete\n",matline );
		}
		mat->script_p += 2;
		while ( mat->script_p[0] != '*' && mat->script_p[1] != '/' )
		{
			if ( *mat->script_p == '\n' ) {
				mat->line++;
				matline = mat->line;
			}
			mat->script_p++;
			if ( mat->script_p >= mat->end_p ) {
				return EndOfMat( crossline );
			}
		}
		mat->script_p += 2;
		goto skipspace;
	}

//
// copy mattoken
//
	mattoken_p = mattoken;

	if ( *mat->script_p == '"' ) {
		// quoted mattoken
		mat->script_p++;
		while ( *mat->script_p != '"' )
		{
			*mattoken_p++ = *mat->script_p++;
			if ( mat->script_p == mat->end_p ) {
				break;
			}
			if ( mattoken_p == &mattoken[MAXTOKEN] ) {
				Error( "Token too large on line %i\n",matline );
			}
		}
		mat->script_p++;
	}
	else{   // regular mattoken
		while ( *mat->script_p > 32 && *mat->script_p != ';' )
		{
			*mattoken_p++ = *mat->script_p++;
			if ( mat->script_p == mat->end_p ) {
				break;
			}
			if ( mattoken_p == &mattoken[MAXTOKEN] ) {
				Error( "Token too large on line %i\n",matline );
			}
		}
	}

	*mattoken_p = 0;

	if ( !strcmp( mattoken, "$include" ) ) {
		GetMatToken( qfalse );
		AddMatToStack( mattoken, 0 );
		return GetMatToken( crossline );
	}

	return qtrue;
}


/*
   ==============
   MatTokenAvailable

   Returns qtrue if there is another mattoken on the line
   ==============
 */
qboolean MatTokenAvailable( void ) {
	int oldLine;
	qboolean r;

	/* save */
	oldLine = matline;

	/* test */
	r = GetMatToken( qtrue );
	if ( !r ) {
		return qfalse;
	}
	UnGetMatToken();
	if ( oldLine == matline ) {
		return qtrue;
	}

	/* restore */
	//%	matline = oldLine;
	//%	script->line = oldScriptLine;

	return qfalse;
}


//=====================================================================


void MatchMatToken( char *match ) {
	GetMatToken( qtrue );

	if ( strcmp( mattoken, match ) ) {
		Error( "MatchMatToken( \"%s\" ) failed at line %i in file %s", match, matline, mat->filename );
	}
}

void Parse1DMatMatrix( int x, vec_t *m ) {
	int i;

	MatchMatToken( "(" );

	for ( i = 0; i < x; i++ ) {
		GetMatToken( qfalse );
		m[i] = atof( mattoken );
	}

	MatchMatToken( ")" );
}

void Parse2DMatMatrix( int y, int x, vec_t *m ) {
	int i;

	MatchMatToken( "(" );

	for ( i = 0; i < y; i++ ) {
		Parse1DMatMatrix( x, m + i * x );
	}

	MatchMatToken( ")" );
}

void Parse3DMatMatrix( int z, int y, int x, vec_t *m ) {
	int i;

	MatchMatToken( "(" );

	for ( i = 0; i < z; i++ ) {
		Parse2DMatMatrix( y, x, m + i * x * y );
	}

	MatchMatToken( ")" );
}


void Write1DMatMatrix( FILE *f, int x, vec_t *m ) {
	int i;

	fprintf( f, "( " );
	for ( i = 0; i < x; i++ ) {
		if ( m[i] == (int)m[i] ) {
			fprintf( f, "%i ", (int)m[i] );
		}
		else {
			fprintf( f, "%f ", m[i] );
		}
	}
	fprintf( f, ")" );
}

void Write2DMatMatrix( FILE *f, int y, int x, vec_t *m ) {
	int i;

	fprintf( f, "( " );
	for ( i = 0; i < y; i++ ) {
		Write1DMatMatrix( f, x, m + i * x );
		fprintf( f, " " );
	}
	fprintf( f, ")\n" );
}


void Write3DMatMatrix( FILE *f, int z, int y, int x, vec_t *m ) {
	int i;

	fprintf( f, "(\n" );
	for ( i = 0; i < z; i++ ) {
		Write2DMatMatrix( f, y, x, m + i * ( x * y ) );
	}
	fprintf( f, ")\n" );
}
