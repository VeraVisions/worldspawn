/* -------------------------------------------------------------------------------

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

   ----------------------------------------------------------------------------------

   This code has been altered significantly from its original form, to support
   several games based on the Quake III Arena engine, in the form of "Q3Map2."

   ------------------------------------------------------------------------------- */



/* marker */
#define SHADERS_C



/* dependencies */
#include "vmap.h"



/*
   ColorMod()
   routines for dealing with vertex color/alpha modification
 */

void ColorMod( colorMod_t *cm, int numVerts, bspDrawVert_t *drawVerts ){
	int i, j, k;
	float c;
	vec4_t mult, add;
	bspDrawVert_t   *dv;
	colorMod_t      *cm2;


	/* dummy check */
	if ( cm == NULL || numVerts < 1 || drawVerts == NULL ) {
		return;
	}


	/* walk vertex list */
	for ( i = 0; i < numVerts; i++ )
	{
		/* get vertex */
		dv = &drawVerts[ i ];

		/* walk colorMod list */
		for ( cm2 = cm; cm2 != NULL; cm2 = cm2->next )
		{
			/* default */
			VectorSet( mult, 1.0f, 1.0f, 1.0f );
			mult[ 3 ] = 1.0f;
			VectorSet( add, 0.0f, 0.0f, 0.0f );
			mult[ 3 ] = 0.0f;

			/* switch on type */
			switch ( cm2->type )
			{
			case CM_COLOR_SET:
				VectorClear( mult );
				VectorScale( cm2->data, 255.0f, add );
				break;

			case CM_ALPHA_SET:
				mult[ 3 ] = 0.0f;
				add[ 3 ] = cm2->data[ 0 ] * 255.0f;
				break;

			case CM_COLOR_SCALE:
				VectorCopy( cm2->data, mult );
				break;

			case CM_ALPHA_SCALE:
				mult[ 3 ] = cm2->data[ 0 ];
				break;

			case CM_COLOR_DOT_PRODUCT:
				c = DotProduct( dv->normal, cm2->data );
				VectorSet( mult, c, c, c );
				break;

			case CM_COLOR_DOT_PRODUCT_SCALE:
				c = DotProduct( dv->normal, cm2->data );
				c = ( c - cm2->data[3] ) / ( cm2->data[4] - cm2->data[3] );
				VectorSet( mult, c, c, c );
				break;

			case CM_ALPHA_DOT_PRODUCT:
				mult[ 3 ] = DotProduct( dv->normal, cm2->data );
				break;

			case CM_ALPHA_DOT_PRODUCT_SCALE:
				c = DotProduct( dv->normal, cm2->data );
				c = ( c - cm2->data[3] ) / ( cm2->data[4] - cm2->data[3] );
				mult[ 3 ] = c;
				break;

			case CM_COLOR_DOT_PRODUCT_2:
				c = DotProduct( dv->normal, cm2->data );
				c *= c;
				VectorSet( mult, c, c, c );
				break;

			case CM_COLOR_DOT_PRODUCT_2_SCALE:
				c = DotProduct( dv->normal, cm2->data );
				c *= c;
				c = ( c - cm2->data[3] ) / ( cm2->data[4] - cm2->data[3] );
				VectorSet( mult, c, c, c );
				break;

			case CM_ALPHA_DOT_PRODUCT_2:
				mult[ 3 ] = DotProduct( dv->normal, cm2->data );
				mult[ 3 ] *= mult[ 3 ];
				break;

			case CM_ALPHA_DOT_PRODUCT_2_SCALE:
				c = DotProduct( dv->normal, cm2->data );
				c *= c;
				c = ( c - cm2->data[3] ) / ( cm2->data[4] - cm2->data[3] );
				mult[ 3 ] = c;
				break;

			default:
				break;
			}

			/* apply mod */
			for ( j = 0; j < MAX_LIGHTMAPS; j++ )
			{
				for ( k = 0; k < 4; k++ )
				{
					c = ( mult[ k ] * dv->color[ j ][ k ] ) + add[ k ];
					if ( c < 0 ) {
						c = 0;
					}
					else if ( c > 255 ) {
						c = 255;
					}
					dv->color[ j ][ k ] = c;
				}
			}
		}
	}
}



/*
   TCMod*()
   routines for dealing with a 3x3 texture mod matrix
 */

void TCMod( tcMod_t mod, float st[ 2 ] ){
	float old[ 2 ];


	old[ 0 ] = st[ 0 ];
	old[ 1 ] = st[ 1 ];
	st[ 0 ] = ( mod[ 0 ][ 0 ] * old[ 0 ] ) + ( mod[ 0 ][ 1 ] * old[ 1 ] ) + mod[ 0 ][ 2 ];
	st[ 1 ] = ( mod[ 1 ][ 0 ] * old[ 0 ] ) + ( mod[ 1 ][ 1 ] * old[ 1 ] ) + mod[ 1 ][ 2 ];
}


void TCModIdentity( tcMod_t mod ){
	mod[ 0 ][ 0 ] = 1.0f;   mod[ 0 ][ 1 ] = 0.0f;   mod[ 0 ][ 2 ] = 0.0f;
	mod[ 1 ][ 0 ] = 0.0f;   mod[ 1 ][ 1 ] = 1.0f;   mod[ 1 ][ 2 ] = 0.0f;
	mod[ 2 ][ 0 ] = 0.0f;   mod[ 2 ][ 1 ] = 0.0f;   mod[ 2 ][ 2 ] = 1.0f;   /* this row is only used for multiples, not transformation */
}


void TCModMultiply( tcMod_t a, tcMod_t b, tcMod_t out ){
	int i;


	for ( i = 0; i < 3; i++ )
	{
		out[ i ][ 0 ] = ( a[ i ][ 0 ] * b[ 0 ][ 0 ] ) + ( a[ i ][ 1 ] * b[ 1 ][ 0 ] ) + ( a[ i ][ 2 ] * b[ 2 ][ 0 ] );
		out[ i ][ 1 ] = ( a[ i ][ 0 ] * b[ 0 ][ 1 ] ) + ( a[ i ][ 1 ] * b[ 1 ][ 1 ] ) + ( a[ i ][ 2 ] * b[ 2 ][ 1 ] );
		out[ i ][ 2 ] = ( a[ i ][ 0 ] * b[ 0 ][ 2 ] ) + ( a[ i ][ 1 ] * b[ 1 ][ 2 ] ) + ( a[ i ][ 2 ] * b[ 2 ][ 2 ] );
	}
}


void TCModTranslate( tcMod_t mod, float s, float t ){
	mod[ 0 ][ 2 ] += s;
	mod[ 1 ][ 2 ] += t;
}


void TCModScale( tcMod_t mod, float s, float t ){
	mod[ 0 ][ 0 ] *= s;
	mod[ 1 ][ 1 ] *= t;
}


void TCModRotate( tcMod_t mod, float euler ){
	tcMod_t old, temp;
	float radians, sinv, cosv;


	memcpy( old, mod, sizeof( tcMod_t ) );
	TCModIdentity( temp );

	radians = euler / 180 * Q_PI;
	sinv = sin( radians );
	cosv = cos( radians );

	temp[ 0 ][ 0 ] = cosv;  temp[ 0 ][ 1 ] = -sinv;
	temp[ 1 ][ 0 ] = sinv;  temp[ 1 ][ 1 ] = cosv;

	TCModMultiply( old, temp, mod );
}



/*
   ApplySurfaceParm() - ydnar
   applies a named surfaceparm to the supplied flags
 */

qboolean ApplySurfaceParm( char *name, int *contentFlags, int *surfaceFlags, int *compileFlags ){
	int i, fake;
	surfaceParm_t   *sp;


	/* dummy check */
	if ( name == NULL ) {
		name = "";
	}
	if ( contentFlags == NULL ) {
		contentFlags = &fake;
	}
	if ( surfaceFlags == NULL ) {
		surfaceFlags = &fake;
	}
	if ( compileFlags == NULL ) {
		compileFlags = &fake;
	}

	/* walk the current game's surfaceparms */
	sp = game->surfaceParms;
	while ( sp->name != NULL )
	{
		/* match? */
		if ( !Q_stricmp( name, sp->name ) ) {
			/* clear and set flags */
			*contentFlags &= ~( sp->contentFlagsClear );
			*contentFlags |= sp->contentFlags;
			*surfaceFlags &= ~( sp->surfaceFlagsClear );
			*surfaceFlags |= sp->surfaceFlags;
			*compileFlags &= ~( sp->compileFlagsClear );
			*compileFlags |= sp->compileFlags;

			/* return ok */
			return qtrue;
		}

		/* next */
		sp++;
	}

	/* check custom info parms */
	for ( i = 0; i < numCustSurfaceParms; i++ )
	{
		/* get surfaceparm */
		sp = &custSurfaceParms[ i ];

		/* match? */
		if ( !Q_stricmp( name, sp->name ) ) {
			/* clear and set flags */
			*contentFlags &= ~( sp->contentFlagsClear );
			*contentFlags |= sp->contentFlags;
			*surfaceFlags &= ~( sp->surfaceFlagsClear );
			*surfaceFlags |= sp->surfaceFlags;
			*compileFlags &= ~( sp->compileFlagsClear );
			*compileFlags |= sp->compileFlags;

			/* return ok */
			return qtrue;
		}
	}

	/* no matching surfaceparm found */
	return qfalse;
}



/*
   BeginMapShaderFile() - ydnar
   erases and starts a new map shader script
 */

void BeginMapShaderFile( const char *mapFile ){
	char base[ 1024 ];
	int len;


	/* dummy check */
	mapName[ 0 ] = '\0';
	mapShaderFile[ 0 ] = '\0';
	if ( mapFile == NULL || mapFile[ 0 ] == '\0' ) {
		return;
	}

	/* copy map name */
	strcpy( base, mapFile );
	StripExtension( base );

	/* extract map name */
	len = strlen( base ) - 1;
	while ( len > 0 && base[ len ] != '/' && base[ len ] != '\\' )
		len--;
	strcpy( mapName, &base[ len + 1 ] );
	base[ len ] = '\0';
	if ( len <= 0 ) {
		return;
	}

	/* append ../scripts/vmap_<mapname>.shader */
	sprintf( mapShaderFile, "%s/../%s/vmap_%s.shader", base, game->shaderPath, mapName );
	Sys_FPrintf( SYS_VRB, "Map has shader script %s\n", mapShaderFile );

	/* remove it */
	remove( mapShaderFile );

	/* stop making warnings about missing images */
	warnImage = qfalse;
}



/*
   WriteMapShaderFile() - ydnar
   writes a shader to the map shader script
 */

void WriteMapShaderFile( void ){
	FILE            *file;
	shaderInfo_t    *si;
	int i, num;


	/* dummy check */
	if ( mapShaderFile[ 0 ] == '\0' ) {
		return;
	}

	/* are there any custom shaders? */
	for ( i = 0, num = 0; i < numShaderInfo; i++ )
	{
		if ( shaderInfo[ i ].custom ) {
			break;
		}
	}
	if ( i == numShaderInfo ) {
		return;
	}

	/* note it */
	Sys_FPrintf( SYS_VRB, "--- WriteMapShaderFile ---\n" );
	Sys_FPrintf( SYS_VRB, "Writing %s", mapShaderFile );

	/* open shader file */
	file = fopen( mapShaderFile, "w" );
	if ( file == NULL ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Unable to open map shader file %s for writing\n", mapShaderFile );
		return;
	}

	/* print header */
	fprintf( file,
	         "// Custom shader file for %s.bsp\n"
	         "// Generated by Q3Map2 (ydnar)\n"
	         "// Do not edit! This file is overwritten on recompiles.\n\n",
	         mapName );

	/* walk the shader list */
	for ( i = 0, num = 0; i < numShaderInfo; i++ )
	{
		/* get the shader and print it */
		si = &shaderInfo[ i ];
		if ( si->custom == qfalse || si->shaderText == NULL || si->shaderText[ 0 ] == '\0' ) {
			continue;
		}
		num++;

		/* print it to the file */
		fprintf( file, "%s%s\n", si->shader, si->shaderText );
		//Sys_Printf( "%s%s\n", si->shader, si->shaderText ); /* FIXME: remove debugging code */

		Sys_FPrintf( SYS_VRB, "." );
	}

	/* close the shader */
	fflush( file );
	fclose( file );

	Sys_FPrintf( SYS_VRB, "\n" );

	/* print some stats */
	Sys_Printf( "%9d custom shaders emitted\n", num );
}



/*
   CustomShader() - ydnar
   sets up a custom map shader
 */

shaderInfo_t *CustomShader( shaderInfo_t *si, char *find, char *replace ){
	shaderInfo_t    *csi;
	char shader[ MAX_QPATH ];
	char            *s;
	int loc;
	byte digest[ 16 ];
	char            *srcShaderText, temp[ 8192 ], shaderText[ 8192 ];   /* ydnar: fixme (make this bigger?) */


	/* dummy check */
	if ( si == NULL ) {
		return ShaderInfoForShader( "default" );
	}

	/* default shader text source */
	srcShaderText = si->shaderText;

	/* et: implicitMap */
	if ( si->implicitMap == IM_OPAQUE ) {
		srcShaderText = temp;
		sprintf( temp, "\n"
		         "{ // Q3Map2 defaulted (implicitMap)\n"
		         "\t{\n"
		         "\t\tmap $lightmap\n"
		         "\t\trgbGen identity\n"
		         "\t}\n"
		         "\tq3map_styleMarker\n"
		         "\t{\n"
		         "\t\tmap %s\n"
		         "\t\tblendFunc GL_DST_COLOR GL_ZERO\n"
		         "\t\trgbGen identity\n"
		         "\t}\n"
		         "}\n",
		         si->implicitImagePath );
	}

	/* et: implicitMask */
	else if ( si->implicitMap == IM_MASKED ) {
		srcShaderText = temp;
		sprintf( temp, "\n"
		         "{ // Q3Map2 defaulted (implicitMask)\n"
		         "\tcull none\n"
		         "\t{\n"
		         "\t\tmap %s\n"
		         "\t\talphaFunc GE128\n"
		         "\t\tdepthWrite\n"
		         "\t}\n"
		         "\t{\n"
		         "\t\tmap $lightmap\n"
		         "\t\trgbGen identity\n"
		         "\t\tdepthFunc equal\n"
		         "\t}\n"
		         "\tq3map_styleMarker\n"
		         "\t{\n"
		         "\t\tmap %s\n"
		         "\t\tblendFunc GL_DST_COLOR GL_ZERO\n"
		         "\t\tdepthFunc equal\n"
		         "\t\trgbGen identity\n"
		         "\t}\n"
		         "}\n",
		         si->implicitImagePath,
		         si->implicitImagePath );
	}

	/* et: implicitBlend */
	else if ( si->implicitMap == IM_BLEND ) {
		srcShaderText = temp;
		sprintf( temp, "\n"
		         "{ // Q3Map2 defaulted (implicitBlend)\n"
		         "\tcull none\n"
		         "\t{\n"
		         "\t\tmap %s\n"
		         "\t\tblendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA\n"
		         "\t}\n"
		         "\t{\n"
		         "\t\tmap $lightmap\n"
		         "\t\trgbGen identity\n"
		         "\t\tblendFunc GL_DST_COLOR GL_ZERO\n"
		         "\t}\n"
		         "\tq3map_styleMarker\n"
		         "}\n",
		         si->implicitImagePath );
	}

	/* default shader text */
	else if ( srcShaderText == NULL ) {
		srcShaderText = temp;
		sprintf( temp, "\n"
		         "{ // Q3Map2 defaulted\n"
		         "\t{\n"
		         "\t\tmap $lightmap\n"
		         "\t\trgbGen identity\n"
		         "\t}\n"
		         "\tq3map_styleMarker\n"
		         "\t{\n"
		         "\t\tmap %s.tga\n"
		         "\t\tblendFunc GL_DST_COLOR GL_ZERO\n"
		         "\t\trgbGen identity\n"
		         "\t}\n"
		         "}\n",
		         si->shader );
	}

	/* error check */
	if ( ( strlen( mapName ) + 1 + 32 ) > MAX_QPATH ) {
		Error( "Custom shader name length (%d) exceeded. Shorten your map name.\n", MAX_QPATH );
	}

	/* do some bad find-replace */
	s = strstr( srcShaderText, find );
	if ( s == NULL ) {
		//%	strcpy( shaderText, srcShaderText );
		return si;  /* testing just using the existing shader if this fails */
	}
	else
	{
		/* substitute 'find' with 'replace' */
		loc = s - srcShaderText;
		strcpy( shaderText, srcShaderText );
		shaderText[ loc ] = '\0';
		strcat( shaderText, replace );
		strcat( shaderText, &srcShaderText[ loc + strlen( find ) ] );
	}

	/* make md4 hash of the shader text */
	Com_BlockFullChecksum( shaderText, strlen( shaderText ), digest );

	/* mangle hash into a shader name */
	sprintf( shader, "%s/%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", mapName,
	         digest[ 0 ], digest[ 1 ], digest[ 2 ], digest[ 3 ], digest[ 4 ], digest[ 5 ], digest[ 6 ], digest[ 7 ],
	         digest[ 8 ], digest[ 9 ], digest[ 10 ], digest[ 11 ], digest[ 12 ], digest[ 13 ], digest[ 14 ], digest[ 15 ] );

	/* get shader */
	csi = ShaderInfoForShader( shader );

	/* might be a preexisting shader */
	if ( csi->custom ) {
		return csi;
	}

	/* clone the existing shader and rename */
	memcpy( csi, si, sizeof( shaderInfo_t ) );
	strcpy( csi->shader, shader );
	csi->custom = qtrue;

	/* store new shader text */
	csi->shaderText = safe_malloc( strlen( shaderText ) + 1 );
	strcpy( csi->shaderText, shaderText );  /* LEAK! */

	/* return it */
	return csi;
}



/*
   EmitVertexRemapShader()
   adds a vertexremapshader key/value pair to worldspawn
 */

void EmitVertexRemapShader( char *from, char *to ){
	byte digest[ 16 ];
	char key[ 64 ], value[ 256 ];


	/* dummy check */
	if ( from == NULL || from[ 0 ] == '\0' ||
	     to == NULL || to[ 0 ] == '\0' ) {
		return;
	}

	/* build value */
	sprintf( value, "%s;%s", from, to );

	/* make md4 hash */
	Com_BlockFullChecksum( value, strlen( value ), digest );

	/* make key (this is annoying, as vertexremapshader is precisely 17 characters,
	   which is one too long, so we leave off the last byte of the md5 digest) */
	sprintf( key, "vertexremapshader%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
	         digest[ 0 ], digest[ 1 ], digest[ 2 ], digest[ 3 ], digest[ 4 ], digest[ 5 ], digest[ 6 ], digest[ 7 ],
	         digest[ 8 ], digest[ 9 ], digest[ 10 ], digest[ 11 ], digest[ 12 ], digest[ 13 ], digest[ 14 ] );         /* no: digest[ 15 ] */

	/* add key/value pair to worldspawn */
	SetKeyValue( &entities[ 0 ], key, value );
}



/*
   AllocShaderInfo()
   allocates and initializes a new shader
 */

static shaderInfo_t *AllocShaderInfo( void ){
	shaderInfo_t    *si;


	/* allocate? */
	if ( shaderInfo == NULL ) {
		shaderInfo = safe_malloc( sizeof( shaderInfo_t ) * MAX_SHADER_INFO );
		numShaderInfo = 0;
	}

	/* bounds check */
	if ( numShaderInfo == MAX_SHADER_INFO ) {
		Error( "MAX_SHADER_INFO exceeded. Remove some PK3 files or shader scripts from shaderlist.txt and try again." );
	}
	si = &shaderInfo[ numShaderInfo ];
	numShaderInfo++;

	/* ydnar: clear to 0 first */
	memset( si, 0, sizeof( shaderInfo_t ) );

	/* set defaults */
	ApplySurfaceParm( "default", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );

	si->backsplashFraction = DEF_BACKSPLASH_FRACTION;
	si->backsplashDistance = DEF_BACKSPLASH_DISTANCE;

	si->bounceScale = DEF_RADIOSITY_BOUNCE;

	si->lightStyle = LS_NORMAL;

	si->polygonOffset = qfalse;

	si->shadeAngleDegrees = 0.0f;
	si->lightmapSampleSize = 0;
	si->lightmapSampleOffset = DEFAULT_LIGHTMAP_SAMPLE_OFFSET;
	si->patchShadows = qfalse;
	si->vertexShadows = qtrue;  /* ydnar: changed default behavior */
	si->forceSunlight = qfalse;
	si->vertexScale = 1.0;
	si->notjunc = qfalse;

	/* ydnar: set texture coordinate transform matrix to identity */
	TCModIdentity( si->mod );

	/* ydnar: lightmaps can now be > 128x128 in certain games or an externally generated tga */
	si->lmCustomWidth = lmCustomSize;
	si->lmCustomHeight = lmCustomSize;

	/* return to sender */
	return si;
}



/*
   FinishShader() - ydnar
   sets a shader's width and height among other things
 */

void FinishShader( shaderInfo_t *si ){
	int x, y;
	float st[ 2 ], o[ 2 ], dist, bestDist;
	vec4_t color, delta;


	/* don't double-dip */
	if ( si->finished ) {
		return;
	}

	/* if they're explicitly set, copy from image size */
	if ( si->shaderWidth == 0 && si->shaderHeight == 0 ) {
		si->shaderWidth = si->shaderImage->width;
		si->shaderHeight = si->shaderImage->height;
	}

	/* legacy terrain has explicit image-sized texture projection */
	if ( si->legacyTerrain && si->tcGen == qfalse ) {
		/* set xy texture projection */
		si->tcGen = qtrue;
		VectorSet( si->vecs[ 0 ], ( 1.0f / ( si->shaderWidth * 0.5f ) ), 0, 0 );
		VectorSet( si->vecs[ 1 ], 0, ( 1.0f / ( si->shaderHeight * 0.5f ) ), 0 );
	}

	/* find pixel coordinates best matching the average color of the image */
	bestDist = 99999999;
	o[ 0 ] = 1.0f / si->shaderImage->width;
	o[ 1 ] = 1.0f / si->shaderImage->height;
	for ( y = 0, st[ 1 ] = 0.0f; y < si->shaderImage->height; y++, st[ 1 ] += o[ 1 ] )
	{
		for ( x = 0, st[ 0 ] = 0.0f; x < si->shaderImage->width; x++, st[ 0 ] += o[ 0 ] )
		{
			/* sample the shader image */
			RadSampleImage( si->shaderImage->pixels, si->shaderImage->width, si->shaderImage->height, st, color );

			/* determine error squared */
			VectorSubtract( color, si->averageColor, delta );
			delta[ 3 ] = color[ 3 ] - si->averageColor[ 3 ];
			dist = delta[ 0 ] * delta[ 0 ] + delta[ 1 ] * delta[ 1 ] + delta[ 2 ] * delta[ 2 ] + delta[ 3 ] * delta[ 3 ];
			if ( dist < bestDist ) {
				si->stFlat[ 0 ] = st[ 0 ];
				si->stFlat[ 1 ] = st[ 1 ];
			}
		}
	}

	/* set to finished */
	si->finished = qtrue;
}



/*
   LoadShaderImages()
   loads a shader's images
   ydnar: image.c made this a bit simpler
 */

static void LoadShaderImages( shaderInfo_t *si ){
	int i, count;
	float color[ 4 ];


	/* nodraw shaders don't need images */
	if ( si->compileFlags & C_NODRAW ) {
		si->shaderImage = ImageLoad( DEFAULT_IMAGE );
	}
	else
	{
		/* try to load editor image first */
		si->shaderImage = ImageLoad( si->editorImagePath );

		/* then try shadername */
		if ( si->shaderImage == NULL ) {
			si->shaderImage = ImageLoad( si->shader );
		}

		/* then try implicit image path (note: new behavior!) */
		if ( si->shaderImage == NULL ) {
			si->shaderImage = ImageLoad( si->implicitImagePath );
		}

		/* then try lightimage (note: new behavior!) */
		if ( si->shaderImage == NULL ) {
			si->shaderImage = ImageLoad( si->lightImagePath );
		}

		/* otherwise, use default image */
		if ( si->shaderImage == NULL ) {
			si->shaderImage = ImageLoad( DEFAULT_IMAGE );
			if ( warnImage && strcmp( si->shader, "noshader" ) ) {
				Sys_FPrintf( SYS_WRN, "WARNING: Couldn't find image for shader %s\n", si->shader );
			}
		}

		/* load light image */
		si->lightImage = ImageLoad( si->lightImagePath );

		/* load normalmap image (ok if this is NULL) */
		si->normalImage = ImageLoad( si->normalImagePath );
		if ( si->normalImage != NULL ) {
			Sys_FPrintf( SYS_VRB, "Shader %s has\n"
			             "    NM %s\n", si->shader, si->normalImagePath );
		}
	}

	/* if no light image, use shader image */
	if ( si->lightImage == NULL ) {
		si->lightImage = ImageLoad( si->shaderImage->name );
	}

	/* create default and average colors */
	count = si->lightImage->width * si->lightImage->height;
	VectorClear( color );
	color[ 3 ] = 0.0f;
	for ( i = 0; i < count; i++ )
	{
		color[ 0 ] += si->lightImage->pixels[ i * 4 + 0 ];
		color[ 1 ] += si->lightImage->pixels[ i * 4 + 1 ];
		color[ 2 ] += si->lightImage->pixels[ i * 4 + 2 ];
		color[ 3 ] += si->lightImage->pixels[ i * 4 + 3 ];
	}

	if ( VectorLength( si->color ) <= 0.0f ) {
		ColorNormalize( color, si->color );
		VectorScale( color, ( 1.0f / count ), si->averageColor );
	}
	else
	{
		VectorCopy( si->color, si->averageColor );
	}
}



/*
   ShaderInfoForShader()
   finds a shaderinfo for a named shader
 */

#define MAX_SHADER_DEPRECATION_DEPTH 16

shaderInfo_t *ShaderInfoForShaderNull( const char *shaderName ){
	if ( !strcmp( shaderName, "noshader" ) ) {
		return NULL;
	}
	return ShaderInfoForShader( shaderName );
}

/*
   GetTokenAppend() - ydnar
   gets a token and appends its text to the specified buffer
 */

static int oldMatLine = 0;
static int mattabDepth = 0;

qboolean GetMatTokenAppend( char *buffer, qboolean crossline ){
	qboolean r;
	int i;


	/* get the token */
	r = GetMatToken( crossline );
	if ( r == qfalse || buffer == NULL || mattoken[ 0 ] == '\0' ) {
		return r;
	}

	/* pre-tabstops */
	if ( mattoken[ 0 ] == '}' ) {
		mattabDepth--;
	}

	/* append? */
	if ( oldMatLine != matline ) {
		strcat( buffer, "\n" );
		for ( i = 0; i < mattabDepth; i++ )
			strcat( buffer, "\t" );
	}
	else{
		strcat( buffer, " " );
	}
	oldMatLine = matline;
	strcat( buffer, mattoken );

	/* post-tabstops */
	if ( mattoken[ 0 ] == '{' ) {
		mattabDepth++;
	}

	/* return */
	return r;
}

void Parse1DMatMatrixAppend( char *buffer, int x, vec_t *m ){
	int i;


	if ( !GetMatTokenAppend( buffer, qtrue ) || strcmp( mattoken, "(" ) ) {
		Error( "Parse1DMatrixAppend(): line %d: ( not found!", matline );
	}
	for ( i = 0; i < x; i++ )
	{
		if ( !GetMatTokenAppend( buffer, qfalse ) ) {
			Error( "Parse1DMatrixAppend(): line %d: Number not found!", matline );
		}
		m[ i ] = atof( mattoken );
	}
	if ( !GetMatTokenAppend( buffer, qtrue ) || strcmp( mattoken, ")" ) ) {
		Error( "Parse1DMatrixAppend(): line %d: ) not found!", matline );
	}
}

shaderInfo_t *ShaderInfoForShader( const char *shaderName ){
	int i;
	int deprecationDepth;
	shaderInfo_t    *si;
	char shader[ MAX_QPATH ];
	char filename[ MAX_QPATH ];
	char shaderText[ 8192 ], temp[ 1024 ];
	int val;
	qboolean parsedContent;


	/* init */
	si = NULL;

	/* dummy check */
	if ( shaderName == NULL || shaderName[ 0 ] == '\0' ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Null or empty shader name\n" );
		shaderName = "missing";
	}

	/* strip off extension */
	strcpy( shader, shaderName );
	StripExtension( shader );

#if 1
	deprecationDepth = 0;
	for ( i = 0; i < numShaderInfo; i++ )
	{
		si = &shaderInfo[ i ];
		if ( !Q_stricmp( shader, si->shader ) ) {
			/* check if shader is deprecated */
			if ( deprecationDepth < MAX_SHADER_DEPRECATION_DEPTH && si->deprecateShader && si->deprecateShader[ 0 ] ) {
				/* override name */
				strcpy( shader, si->deprecateShader );
				StripExtension( shader );
				/* increase deprecation depth */
				deprecationDepth++;
				if ( deprecationDepth == MAX_SHADER_DEPRECATION_DEPTH ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Max deprecation depth of %i is reached on shader '%s'\n", MAX_SHADER_DEPRECATION_DEPTH, shader );
				}
				/* search again from beginning */
				i = -1;
				continue;
			}

			/* load image if necessary */
			if ( si->finished == qfalse ) {
				LoadShaderImages( si );
				FinishShader( si );
			}

			/* return it */
			return si;
		}
	}
#endif

	/* allocate a default shader */
	si = AllocShaderInfo();
	strcpy( si->shader, shader );

#if 1
	sprintf( filename, "%s.mat", shader );
	if ( vfsGetFileCount(filename) == 0 ) {
		LoadShaderImages( si );
		FinishShader( si );

		/* return it */
		return si;
	}

	/* load the shader */
	LoadMatFile( filename, 0 );

	/* copy shader text to the shaderinfo */
	if ( si != NULL && shaderText[ 0 ] != '\0' ) {
		strcat( shaderText, "\n" );
		si->shaderText = safe_malloc( strlen( shaderText ) + 1 );
		strcpy( si->shaderText, shaderText );
		//%	if( VectorLength( si->vecs[ 0 ] ) )
		//%		Sys_Printf( "%s\n", shaderText );
	}

	/* ydnar: clear shader text buffer */
	shaderText[ 0 ] = '\0';

	while ( 1 )
	{
		/* get the next mattoken */
		if ( !GetMatTokenAppend( shaderText, qtrue ) ) {
			break;
		}

		if ( !strcmp( mattoken, "}" ) ) {
			break;
		}

		/* -----------------------------------------------------------------
		   surfaceparm * directives
		   ----------------------------------------------------------------- */

		/* match surfaceparm */
		if ( !Q_stricmp( mattoken, "surfaceparm" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			if ( ApplySurfaceParm( mattoken, &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
				Sys_FPrintf( SYS_WRN, "WARNING: Unknown surfaceparm: \"%s\"\n", mattoken );
			}
		}


		/* -----------------------------------------------------------------
		   game-related shader directives
		   ----------------------------------------------------------------- */

		/* ydnar: fogparms (for determining fog volumes) */
		else if ( !Q_stricmp( mattoken, "fogparms" ) ) {
			si->fogParms = qtrue;
		}

		/* ydnar: polygonoffset (for no culling) */
		else if ( !Q_stricmp( mattoken, "polygonoffset" ) ) {
			si->polygonOffset = qtrue;
		}

		/* tesssize is used to force liquid surfaces to subdivide */
		else if ( !Q_stricmp( mattoken, "tessSize" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			si->subdivisions = atof( mattoken );
		}

		/* cull none will set twoSided (ydnar: added disable too) */
		else if ( !Q_stricmp( mattoken, "cull" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			if ( !Q_stricmp( mattoken, "none" ) || !Q_stricmp( mattoken, "disable" ) || !Q_stricmp( mattoken, "twosided" ) ) {
				si->twoSided = qtrue;
			}
		}

		/* deformVertexes autosprite[ 2 ]
		   we catch this so autosprited surfaces become point
		   lights instead of area lights */
		else if ( !Q_stricmp( mattoken, "deformVertexes" ) ) {
			GetMatTokenAppend( shaderText, qfalse );

			/* deformVertexes autosprite(2) */
			if ( !Q_strncasecmp( mattoken, "autosprite", 10 ) ) {
				/* set it as autosprite and detail */
				si->autosprite = qtrue;
				ApplySurfaceParm( "detail", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );

				/* ydnar: gs mods: added these useful things */
				si->noClip = qtrue;
				si->notjunc = qtrue;
			}

			/* deformVertexes move <x> <y> <z> <func> <base> <amplitude> <phase> <freq> (ydnar: for particle studio support) */
			if ( !Q_stricmp( mattoken, "move" ) ) {
				vec3_t amt, mins, maxs;
				float base, amp;


				/* get move amount */
				GetMatTokenAppend( shaderText, qfalse );   amt[ 0 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );   amt[ 1 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );   amt[ 2 ] = atof( mattoken );

				/* skip func */
				GetMatTokenAppend( shaderText, qfalse );

				/* get base and amplitude */
				GetMatTokenAppend( shaderText, qfalse );   base = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );   amp = atof( mattoken );

				/* calculate */
				VectorScale( amt, base, mins );
				VectorMA( mins, amp, amt, maxs );
				VectorAdd( si->mins, mins, si->mins );
				VectorAdd( si->maxs, maxs, si->maxs );
			}
		}

		/* ydnar: damageShader <shader> <health> (sof2 mods) */
		else if ( !Q_stricmp( mattoken, "damageShader" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			if ( mattoken[ 0 ] != '\0' ) {
				si->damageShader = safe_malloc( strlen( mattoken ) + 1 );
				strcpy( si->damageShader, mattoken );
			}
			GetMatTokenAppend( shaderText, qfalse );           /* don't do anything with health */
		}

		/* ydnar: enemy territory implicit shaders */
		else if ( !Q_stricmp( mattoken, "implicitMap" ) ) {
			si->implicitMap = IM_OPAQUE;
			GetMatTokenAppend( shaderText, qfalse );
			if ( mattoken[ 0 ] == '-' && mattoken[ 1 ] == '\0' ) {
				sprintf( si->implicitImagePath, "%s.tga", si->shader );
			}
			else{
				strcpy( si->implicitImagePath, mattoken );
			}
		}

		else if ( !Q_stricmp( mattoken, "implicitMask" ) ) {
			si->implicitMap = IM_MASKED;
			GetMatTokenAppend( shaderText, qfalse );
			if ( mattoken[ 0 ] == '-' && mattoken[ 1 ] == '\0' ) {
				sprintf( si->implicitImagePath, "%s.tga", si->shader );
			}
			else{
				strcpy( si->implicitImagePath, mattoken );
			}
		}

		else if ( !Q_stricmp( mattoken, "implicitBlend" ) ) {
			si->implicitMap = IM_MASKED;
			GetMatTokenAppend( shaderText, qfalse );
			if ( mattoken[ 0 ] == '-' && mattoken[ 1 ] == '\0' ) {
				sprintf( si->implicitImagePath, "%s.tga", si->shader );
			}
			else{
				strcpy( si->implicitImagePath, mattoken );
			}
		}


		/* -----------------------------------------------------------------
		   image directives
		   ----------------------------------------------------------------- */

		/* qer_editorimage <image> */
		else if ( !Q_stricmp( mattoken, "qer_editorImage" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			strcpy( si->editorImagePath, mattoken );
			DefaultExtension( si->editorImagePath, ".tga" );
		}

		/* diffusemap <image> */
		else if ( !Q_stricmp( mattoken, "diffusemap" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			strcpy( si->editorImagePath, mattoken );
			DefaultExtension( si->editorImagePath, ".tga" );
		}

		/* ydnar: q3map_normalimage <image> (bumpmapping normal map) */
		/*else if ( !Q_stricmp( mattoken, "q3map_normalImage" ) ) {
		        GetMatTokenAppend( shaderText, qfalse );
		        strcpy( si->normalImagePath, mattoken );
		        DefaultExtension( si->normalImagePath, ".tga" );
		   }*/

		/* q3map_lightimage <image> */
		else if ( !Q_stricmp( mattoken, "q3map_lightImage" ) || !Q_stricmp( mattoken, "vmap_lightImage" ) ) {
			GetMatTokenAppend( shaderText, qfalse );
			strcpy( si->lightImagePath, mattoken );
			DefaultExtension( si->lightImagePath, ".tga" );
		}

		/* ydnar: skyparms <outer image> <cloud height> <inner image> */
		else if ( !Q_stricmp( mattoken, "skyParms" ) ) {
			/* get image base */
			GetMatTokenAppend( shaderText, qfalse );

			/* ignore bogus paths */
			if ( Q_stricmp( mattoken, "-" ) && Q_stricmp( mattoken, "full" ) ) {
				strcpy( si->skyParmsImageBase, mattoken );
			}

			/* skip rest of line */
			GetMatTokenAppend( shaderText, qfalse );
			GetMatTokenAppend( shaderText, qfalse );
		}

		/* -----------------------------------------------------------------
		   q3map_* directives
		   ----------------------------------------------------------------- */

		/* q3map_sun <red> <green> <blue> <intensity> <degrees> <elevation>
		   color will be normalized, so it doesn't matter what range you use
		   intensity falls off with angle but not distance 100 is a fairly bright sun
		   degree of 0 = from the east, 90 = north, etc.  altitude of 0 = sunrise/set, 90 = noon */
#if 0
		else if ( !Q_stricmp( mattoken, "vmap_sun" ) || !Q_stricmp( mattoken, "q3map_sun" ) || !Q_stricmp( mattoken, "q3map_sunExt" ) ) {
			float a, b;
			sun_t       *sun;
			qboolean ext = qfalse;

			/* ydnar: extended sun directive? */
			if ( !Q_stricmp( mattoken, "q3map_sunext" ) || !Q_stricmp( mattoken, "vmap_sun" ) ) {
				ext = qtrue;
			}

			/* allocate sun */
			sun = safe_malloc( sizeof( *sun ) );
			memset( sun, 0, sizeof( *sun ) );

			/* set style */
			sun->style = si->lightStyle;

			/* get color */
			GetTokenAppend( shaderText, qfalse );
			sun->color[ 0 ] = atof( mattoken );
			GetTokenAppend( shaderText, qfalse );
			sun->color[ 1 ] = atof( mattoken );
			GetTokenAppend( shaderText, qfalse );
			sun->color[ 2 ] = atof( mattoken );

			if ( colorsRGB ) {
				sun->color[0] = Image_LinearFloatFromsRGBFloat( sun->color[0] );
				sun->color[1] = Image_LinearFloatFromsRGBFloat( sun->color[1] );
				sun->color[2] = Image_LinearFloatFromsRGBFloat( sun->color[2] );
			}

			/* normalize it */
			ColorNormalize( sun->color, sun->color );

			/* scale color by brightness */
			GetTokenAppend( shaderText, qfalse );
			sun->photons = atof( mattoken );

			/* get sun angle/elevation */
			GetTokenAppend( shaderText, qfalse );
			a = atof( mattoken );
			a = a / 180.0f * Q_PI;

			GetTokenAppend( shaderText, qfalse );
			b = atof( mattoken );
			b = b / 180.0f * Q_PI;

			sun->direction[ 0 ] = cos( a ) * cos( b );
			sun->direction[ 1 ] = sin( a ) * cos( b );
			sun->direction[ 2 ] = sin( b );

			/* get filter radius from shader */
			sun->filterRadius = si->lightFilterRadius;

			/* ydnar: get sun angular deviance/samples */
			if ( ext && TokenAvailable() ) {
				GetTokenAppend( shaderText, qfalse );
				sun->deviance = atof( mattoken );
				sun->deviance = sun->deviance / 180.0f * Q_PI;

				GetTokenAppend( shaderText, qfalse );
				sun->numSamples = atoi( mattoken );
			}

			/* store sun */
			sun->next = si->sun;
			si->sun = sun;

			/* apply sky surfaceparm */
			ApplySurfaceParm( "sky", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );

			/* don't process any more mattokens on this line */
			continue;
		}
#endif
		/* match q3map_ */
		else if ( !Q_strncasecmp( mattoken, "q3map_", 6 ) || !Q_strncasecmp( mattoken, "vmap_", 5 ) ) {
			/* ydnar: q3map_baseShader <shader> (inherit this shader's parameters) */
			if ( !Q_stricmp( mattoken, "q3map_baseShader" ) || !Q_stricmp( mattoken, "vmap_baseShader" ) ) {
				shaderInfo_t    *si2;
				qboolean oldWarnImage;


				/* get shader */
				GetMatTokenAppend( shaderText, qfalse );
				//%	Sys_FPrintf( SYS_VRB, "Shader %s has base shader %s\n", si->shader, mattoken );
				oldWarnImage = warnImage;
				warnImage = qfalse;
				si2 = ShaderInfoForShader( mattoken );
				warnImage = oldWarnImage;

				/* subclass it */
				if ( si2 != NULL ) {
					/* preserve name */
					strcpy( temp, si->shader );

					/* copy shader */
					memcpy( si, si2, sizeof( *si ) );

					/* restore name and set to unfinished */
					strcpy( si->shader, temp );
					si->shaderWidth = 0;
					si->shaderHeight = 0;
					si->finished = qfalse;
				}
			}

			/* ydnar: q3map_surfacemodel <path to model> <density> <min scale> <max scale> <min angle> <max angle> <oriented (0 or 1)> */
			else if ( !Q_stricmp( mattoken, "q3map_surfacemodel" ) || !Q_stricmp( mattoken, "vmap_surfacemodel" ) ) {
				surfaceModel_t  *model;

				/* allocate new model and attach it */
				model = safe_malloc( sizeof( *model ) );
				memset( model, 0, sizeof( *model ) );
				model->next = si->surfaceModel;
				si->surfaceModel = model;

				/* get parameters */
				GetMatTokenAppend( shaderText, qfalse );
				strcpy( model->model, mattoken );

				GetMatTokenAppend( shaderText, qfalse );
				model->density = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				model->odds = atof( mattoken );

				GetMatTokenAppend( shaderText, qfalse );
				model->minScale = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				model->maxScale = atof( mattoken );

				GetMatTokenAppend( shaderText, qfalse );
				model->minAngle = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				model->maxAngle = atof( mattoken );

				GetMatTokenAppend( shaderText, qfalse );
				model->oriented = ( mattoken[ 0 ] == '1' ? qtrue : qfalse );
			}

			/* ydnar/sd: q3map_foliage <path to model> <scale> <density> <odds> <invert alpha (1 or 0)> */
			else if ( !Q_stricmp( mattoken, "q3map_foliage" ) || !Q_stricmp( mattoken, "vmap_foliage" ) ) {
				foliage_t   *foliage;


				/* allocate new foliage struct and attach it */
				foliage = safe_malloc( sizeof( *foliage ) );
				memset( foliage, 0, sizeof( *foliage ) );
				foliage->next = si->foliage;
				si->foliage = foliage;

				/* get parameters */
				GetMatTokenAppend( shaderText, qfalse );
				strcpy( foliage->model, mattoken );

				GetMatTokenAppend( shaderText, qfalse );
				foliage->scale = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				foliage->density = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				foliage->odds = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				foliage->inverseAlpha = atoi( mattoken );
			}

			/* ydnar: q3map_bounce <value> (fraction of light to re-emit during radiosity passes) */
			else if ( !Q_stricmp( mattoken, "q3map_bounce" ) || !Q_stricmp( mattoken, "vmap_bounce" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->bounceScale = atof( mattoken );
			}
#if 0
			/* ydnar/splashdamage: q3map_skyLight <value> <iterations> */
			else if ( !Q_stricmp( mattoken, "q3map_skyLight" ) || !Q_stricmp( mattoken, "vmap_skyLight" )  ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->skyLightValue = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->skyLightIterations = atoi( mattoken );

				/* clamp */
				if ( si->skyLightValue < 0.0f ) {
					si->skyLightValue = 0.0f;
				}
				if ( si->skyLightIterations < 2 ) {
					si->skyLightIterations = 2;
				}
			}
#endif
			/* q3map_surfacelight <value> */
			else if ( !Q_stricmp( mattoken, "q3map_surfacelight" ) || !Q_stricmp( mattoken, "vmap_surfacelight" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->value = atof( mattoken );
			}

			/* q3map_lightStyle (sof2/jk2 lightstyle) */
			else if ( !Q_stricmp( mattoken, "q3map_lightStyle" ) || !Q_stricmp( mattoken, "vmap_lightStyle" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				val = atoi( mattoken );
				if ( val < 0 ) {
					val = 0;
				}
				else if ( val > LS_NONE ) {
					val = LS_NONE;
				}
				si->lightStyle = val;
			}

			/* wolf: q3map_lightRGB <red> <green> <blue> */
			else if ( !Q_stricmp( mattoken, "q3map_lightRGB" ) || !Q_stricmp( mattoken, "vmap_lightRGB" )) {
				VectorClear( si->color );
				GetMatTokenAppend( shaderText, qfalse );
				si->color[ 0 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->color[ 1 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->color[ 2 ] = atof( mattoken );
				if ( colorsRGB ) {
					si->color[0] = Image_LinearFloatFromsRGBFloat( si->color[0] );
					si->color[1] = Image_LinearFloatFromsRGBFloat( si->color[1] );
					si->color[2] = Image_LinearFloatFromsRGBFloat( si->color[2] );
				}
				ColorNormalize( si->color, si->color );
			}

			/* q3map_lightSubdivide <value> */
			else if ( !Q_stricmp( mattoken, "q3map_lightSubdivide" ) || !Q_stricmp( mattoken, "vmap_lightSubdivide" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lightSubdivide = atoi( mattoken );
			}

			/* vmap_surfacelight multiplier */
			else if ( !Q_stricmp( mattoken, "vmap_surfaceLightDistance" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->surfLightDistance = atof( mattoken );
			}

			/* q3map_backsplash <percent> <distance> */
			else if ( !Q_stricmp( mattoken, "q3map_backsplash" ) || !Q_stricmp( mattoken, "vmap_backsplash" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->backsplashFraction = atof( mattoken ) * 0.01f;
				GetMatTokenAppend( shaderText, qfalse );
				si->backsplashDistance = atof( mattoken );
			}
#if 0
			/* q3map_floodLight <r> <g> <b> <diste> <intensity> <light_direction_power> */
			else if ( !Q_stricmp( mattoken, "q3map_floodLight" ) || !Q_stricmp( mattoken, "vmap_floodLight" ) ) {
				/* get color */
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightRGB[ 0 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightRGB[ 1 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightRGB[ 2 ] = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightDistance = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightIntensity = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->floodlightDirectionScale = atof( mattoken );
				if ( colorsRGB ) {
					si->floodlightRGB[0] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[0] );
					si->floodlightRGB[1] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[1] );
					si->floodlightRGB[2] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[2] );
				}
				ColorNormalize( si->floodlightRGB, si->floodlightRGB );
			}
#endif
			/* jal: q3map_nodirty : skip dirty */
			else if ( !Q_stricmp( mattoken, "q3map_nodirty" ) || !Q_stricmp( mattoken, "vmap_nodirty" ) ) {
				si->noDirty = qtrue;
			}

			/* q3map_lightmapSampleSize <value> */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapSampleSize" ) || !Q_stricmp( mattoken, "vmap_lightmapSampleSize" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lightmapSampleSize = atoi( mattoken );
			}

			/* q3map_lightmapSampleOffset <value> */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapSampleOffset" ) || !Q_stricmp( mattoken, "vmap_lightmapSampleOffset" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lightmapSampleOffset = atof( mattoken );
			}

			/* ydnar: q3map_lightmapFilterRadius <self> <other> */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapFilterRadius" ) || !Q_stricmp( mattoken, "vmap_lightmapFilterRadius" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lmFilterRadius = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->lightFilterRadius = atof( mattoken );
			}

			/* ydnar: q3map_lightmapAxis [xyz] */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapAxis" ) || !Q_stricmp( mattoken, "vmap_lightmapAxis" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				if ( !Q_stricmp( mattoken, "x" ) ) {
					VectorSet( si->lightmapAxis, 1, 0, 0 );
				}
				else if ( !Q_stricmp( mattoken, "y" ) ) {
					VectorSet( si->lightmapAxis, 0, 1, 0 );
				}
				else if ( !Q_stricmp( mattoken, "z" ) ) {
					VectorSet( si->lightmapAxis, 0, 0, 1 );
				}
				else
				{
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown value for lightmap axis: %s\n", mattoken );
					VectorClear( si->lightmapAxis );
				}
			}

			/* ydnar: q3map_lightmapSize <width> <height> (for autogenerated shaders + external tga lightmaps) */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapSize" ) || !Q_stricmp( mattoken, "vmap_lightmapSize" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lmCustomWidth = atoi( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->lmCustomHeight = atoi( mattoken );

				/* must be a power of 2 */
				if ( ( ( si->lmCustomWidth - 1 ) & si->lmCustomWidth ) ||
				     ( ( si->lmCustomHeight - 1 ) & si->lmCustomHeight ) ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Non power-of-two lightmap size specified (%d, %d)\n",
					             si->lmCustomWidth, si->lmCustomHeight );
					si->lmCustomWidth = lmCustomSize;
					si->lmCustomHeight = lmCustomSize;
				}
			}

			/* ydnar: q3map_lightmapBrightness N (for autogenerated shaders + external tga lightmaps) */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapBrightness" ) || !Q_stricmp( mattoken, "q3map_lightmapGamma" ) || !Q_stricmp( mattoken, "vmap_lightmapBrightness" ) || !Q_stricmp( mattoken, "vmap_lightmapGamma" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->lmBrightness = atof( mattoken );
				if ( si->lmBrightness < 0 ) {
					si->lmBrightness = 1.0;
				}
			}

			/* q3map_vertexScale (scale vertex lighting by this fraction) */
			else if ( !Q_stricmp( mattoken, "q3map_vertexScale" ) || !Q_stricmp( mattoken, "vmap_vertexScale" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->vertexScale = atof( mattoken );
			}

			/* q3map_noVertexLight */
			else if ( !Q_stricmp( mattoken, "q3map_noVertexLight" ) || !Q_stricmp( mattoken, "vmap_noVertexLight" ) ) {
				si->noVertexLight = qtrue;
			}

			/* q3map_backShader <shader> */
			else if ( !Q_stricmp( mattoken, "q3map_backShader" ) || !Q_stricmp( mattoken, "vmap_backShader" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				if ( mattoken[ 0 ] != '\0' ) {
					si->backShader = safe_malloc( strlen( mattoken ) + 1 );
					strcpy( si->backShader, mattoken );
				}
			}

			/* ydnar: q3map_cloneShader <shader> */
			else if ( !Q_stricmp( mattoken, "q3map_cloneShader" ) || !Q_stricmp( mattoken, "vmap_cloneShader" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				if ( mattoken[ 0 ] != '\0' ) {
					si->cloneShader = safe_malloc( strlen( mattoken ) + 1 );
					strcpy( si->cloneShader, mattoken );
				}
			}

			/* q3map_remapShader <shader> */
			else if ( !Q_stricmp( mattoken, "q3map_remapShader" ) || !Q_stricmp( mattoken, "vmap_remapShader" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				if ( mattoken[ 0 ] != '\0' ) {
					si->remapShader = safe_malloc( strlen( mattoken ) + 1 );
					strcpy( si->remapShader, mattoken );
				}
			}

			/* q3map_deprecateShader <shader> */
			else if ( !Q_stricmp( mattoken, "q3map_deprecateShader" ) || !Q_stricmp( mattoken, "vmap_deprecateShader" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				if ( mattoken[ 0 ] != '\0' ) {

					si->deprecateShader = safe_malloc( strlen( mattoken ) + 1 );
					strcpy( si->deprecateShader, mattoken );
				}
			}

			/* ydnar: q3map_offset <value> */
			else if ( !Q_stricmp( mattoken, "q3map_offset" ) || !Q_stricmp( mattoken, "vmap_offset" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->offset = atof( mattoken );
			}

			/* ydnar: q3map_fur <numlayers> <offset> <fade> */
			else if ( !Q_stricmp( mattoken, "q3map_fur" ) || !Q_stricmp( mattoken, "vmap_fur" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->furNumLayers = atoi( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->furOffset = atof( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->furFade = atof( mattoken );
			}

			/* ydnar: gs mods: legacy support for terrain/terrain2 shaders */
			else if ( !Q_stricmp( mattoken, "q3map_terrain" ) || !Q_stricmp( mattoken, "vmap_terrain" ) ) {
				/* team arena terrain is assumed to be nonplanar, with full normal averaging,
				   passed through the metatriangle surface pipeline, with a lightmap axis on z */
				si->legacyTerrain = qtrue;
				si->noClip = qtrue;
				si->notjunc = qtrue;
				si->indexed = qtrue;
				si->nonplanar = qtrue;
				si->forceMeta = qtrue;
				si->shadeAngleDegrees = 179.0f;
				//%	VectorSet( si->lightmapAxis, 0, 0, 1 );	/* ydnar 2002-09-21: turning this off for better lightmapping of cliff faces */
			}

			/* ydnar: picomodel: q3map_forceMeta (forces brush faces and/or triangle models to go through the metasurface pipeline) */
			else if ( !Q_stricmp( mattoken, "q3map_forceMeta" ) || !Q_stricmp( mattoken, "vmap_forceMeta" ) ) {
				si->forceMeta = qtrue;
			}

			/* ydnar: gs mods: q3map_shadeAngle <degrees> */
			else if ( !Q_stricmp( mattoken, "q3map_shadeAngle" ) || !Q_stricmp( mattoken, "vmap_shadeAngle" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->shadeAngleDegrees = atof( mattoken );
			}

			/* ydnar: q3map_textureSize <width> <height> (substitute for q3map_lightimage derivation for terrain) */
			else if ( !Q_stricmp( mattoken, "q3map_textureSize" ) || !Q_stricmp( mattoken, "vmap_textureSize" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				si->shaderWidth = atoi( mattoken );
				GetMatTokenAppend( shaderText, qfalse );
				si->shaderHeight = atoi( mattoken );
			}

			/* ydnar: gs mods: q3map_tcGen <style> <parameters> */
			else if ( !Q_stricmp( mattoken, "q3map_tcGen" ) || !Q_stricmp( mattoken, "vmap_tcGen" ) ) {
				si->tcGen = qtrue;
				GetMatTokenAppend( shaderText, qfalse );

				/* q3map_tcGen vector <s vector> <t vector> */
				if ( !Q_stricmp( mattoken, "vector" ) ) {
					Parse1DMatMatrixAppend( shaderText, 3, si->vecs[ 0 ] );
					Parse1DMatMatrixAppend( shaderText, 3, si->vecs[ 1 ] );
				}

				/* q3map_tcGen ivector <1.0/s vector> <1.0/t vector> (inverse vector, easier for mappers to understand) */
				else if ( !Q_stricmp( mattoken, "ivector" ) ) {
					Parse1DMatMatrixAppend( shaderText, 3, si->vecs[ 0 ] );
					Parse1DMatMatrixAppend( shaderText, 3, si->vecs[ 1 ] );
					for ( i = 0; i < 3; i++ )
					{
						si->vecs[ 0 ][ i ] = si->vecs[ 0 ][ i ] ? 1.0 / si->vecs[ 0 ][ i ] : 0;
						si->vecs[ 1 ][ i ] = si->vecs[ 1 ][ i ] ? 1.0 / si->vecs[ 1 ][ i ] : 0;
					}
				}
				else
				{
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_tcGen method: %s\n", mattoken );
					VectorClear( si->vecs[ 0 ] );
					VectorClear( si->vecs[ 1 ] );
				}
			}

			/* ydnar: gs mods: q3map_[color|rgb|alpha][Gen|Mod] <style> <parameters> */
			else if ( !Q_stricmp( mattoken, "q3map_colorGen" ) || !Q_stricmp( mattoken, "q3map_colorMod" ) ||
			          !Q_stricmp( mattoken, "q3map_rgbGen" ) || !Q_stricmp( mattoken, "q3map_rgbMod" ) ||
			          !Q_stricmp( mattoken, "q3map_alphaGen" ) || !Q_stricmp( mattoken, "q3map_alphaMod" ) ) {
				colorMod_t  *cm, *cm2;
				int alpha;


				/* alphamods are colormod + 1 */
				alpha = ( !Q_stricmp( mattoken, "q3map_alphaGen" ) || !Q_stricmp( mattoken, "q3map_alphaMod" ) ) ? 1 : 0;

				/* allocate new colormod */
				cm = safe_malloc( sizeof( *cm ) );
				memset( cm, 0, sizeof( *cm ) );

				/* attach to shader */
				if ( si->colorMod == NULL ) {
					si->colorMod = cm;
				}
				else
				{
					for ( cm2 = si->colorMod; cm2 != NULL; cm2 = cm2->next )
					{
						if ( cm2->next == NULL ) {
							cm2->next = cm;
							break;
						}
					}
				}

				/* get type */
				GetMatTokenAppend( shaderText, qfalse );

				/* alpha set|const A */
				if ( alpha && ( !Q_stricmp( mattoken, "set" ) || !Q_stricmp( mattoken, "const" ) ) ) {
					cm->type = CM_ALPHA_SET;
					GetMatTokenAppend( shaderText, qfalse );
					cm->data[ 0 ] = atof( mattoken );
				}

				/* color|rgb set|const ( X Y Z ) */
				else if ( !Q_stricmp( mattoken, "set" ) || !Q_stricmp( mattoken, "const" ) ) {
					cm->type = CM_COLOR_SET;
					Parse1DMatMatrixAppend( shaderText, 3, cm->data );
					if ( colorsRGB ) {
						cm->data[0] = Image_LinearFloatFromsRGBFloat( cm->data[0] );
						cm->data[1] = Image_LinearFloatFromsRGBFloat( cm->data[1] );
						cm->data[2] = Image_LinearFloatFromsRGBFloat( cm->data[2] );
					}
				}

				/* alpha scale A */
				else if ( alpha && !Q_stricmp( mattoken, "scale" ) ) {
					cm->type = CM_ALPHA_SCALE;
					GetMatTokenAppend( shaderText, qfalse );
					cm->data[ 0 ] = atof( mattoken );
				}

				/* color|rgb scale ( X Y Z ) */
				else if ( !Q_stricmp( mattoken, "scale" ) ) {
					cm->type = CM_COLOR_SCALE;
					Parse1DMatMatrixAppend( shaderText, 3, cm->data );
				}

				/* dotProduct ( X Y Z ) */
				else if ( !Q_stricmp( mattoken, "dotProduct" ) ) {
					cm->type = CM_COLOR_DOT_PRODUCT + alpha;
					Parse1DMatMatrixAppend( shaderText, 3, cm->data );
				}

				/* dotProductScale ( X Y Z MIN MAX ) */
				else if ( !Q_stricmp( mattoken, "dotProductScale" ) ) {
					cm->type = CM_COLOR_DOT_PRODUCT_SCALE + alpha;
					Parse1DMatMatrixAppend( shaderText, 5, cm->data );
				}

				/* dotProduct2 ( X Y Z ) */
				else if ( !Q_stricmp( mattoken, "dotProduct2" ) ) {
					cm->type = CM_COLOR_DOT_PRODUCT_2 + alpha;
					Parse1DMatMatrixAppend( shaderText, 3, cm->data );
				}

				/* dotProduct2scale ( X Y Z MIN MAX ) */
				else if ( !Q_stricmp( mattoken, "dotProduct2scale" ) ) {
					cm->type = CM_COLOR_DOT_PRODUCT_2_SCALE + alpha;
					Parse1DMatMatrixAppend( shaderText, 5, cm->data );
				}

				/* volume */
				else if ( !Q_stricmp( mattoken, "volume" ) ) {
					/* special stub mode for flagging volume brushes */
					cm->type = CM_VOLUME;
				}

				/* unknown */
				else{
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown colorMod method: %s\n", mattoken );
				}
			}

			/* ydnar: gs mods: q3map_tcMod <style> <parameters> */
			else if ( !Q_stricmp( mattoken, "q3map_tcMod" ) || !Q_stricmp( mattoken, "vmap_tcMod" ) ) {
				float a, b;


				GetMatTokenAppend( shaderText, qfalse );

				/* q3map_tcMod [translate | shift | offset] <s> <t> */
				if ( !Q_stricmp( mattoken, "translate" ) || !Q_stricmp( mattoken, "shift" ) || !Q_stricmp( mattoken, "offset" ) ) {
					GetMatTokenAppend( shaderText, qfalse );
					a = atof( mattoken );
					GetMatTokenAppend( shaderText, qfalse );
					b = atof( mattoken );

					TCModTranslate( si->mod, a, b );
				}

				/* q3map_tcMod scale <s> <t> */
				else if ( !Q_stricmp( mattoken, "scale" ) ) {
					GetMatTokenAppend( shaderText, qfalse );
					a = atof( mattoken );
					GetMatTokenAppend( shaderText, qfalse );
					b = atof( mattoken );

					TCModScale( si->mod, a, b );
				}

				/* q3map_tcMod rotate <s> <t> (fixme: make this communitive) */
				else if ( !Q_stricmp( mattoken, "rotate" ) ) {
					GetMatTokenAppend( shaderText, qfalse );
					a = atof( mattoken );
					TCModRotate( si->mod, a );
				}
				else{
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_tcMod method: %s\n", mattoken );
				}
			}

			/* q3map_fogDir (direction a fog shader fades from transparent to opaque) */
			else if ( !Q_stricmp( mattoken, "q3map_fogDir" ) || !Q_stricmp( mattoken, "vmap_fogDir" ) ) {
				Parse1DMatMatrixAppend( shaderText, 3, si->fogDir );
				VectorNormalize( si->fogDir, si->fogDir );
			}

			/* q3map_globaltexture */
			else if ( !Q_stricmp( mattoken, "q3map_globaltexture" ) || !Q_stricmp( mattoken, "vmap_globaltexture" )  ) {
				si->globalTexture = qtrue;
			}

			/* ydnar: gs mods: q3map_nonplanar (make it a nonplanar merge candidate for meta surfaces) */
			else if ( !Q_stricmp( mattoken, "q3map_nonplanar" ) || !Q_stricmp( mattoken, "vmap_nonplanar" ) ) {
				si->nonplanar = qtrue;
			}

			/* ydnar: gs mods: q3map_noclip (preserve original face winding, don't clip by bsp tree) */
			else if ( !Q_stricmp( mattoken, "q3map_noclip" ) || !Q_stricmp( mattoken, "vmap_noclip" ) ) {
				si->noClip = qtrue;
			}

			/* q3map_notjunc */
			else if ( !Q_stricmp( mattoken, "q3map_notjunc" ) || !Q_stricmp( mattoken, "vmap_notjunc" ) ) {
				si->notjunc = qtrue;
			}

			/* q3map_nofog */
			else if ( !Q_stricmp( mattoken, "q3map_nofog" ) || !Q_stricmp( mattoken, "vmap_nofog" ) ) {
				si->noFog = qtrue;
			}

			/* ydnar: gs mods: q3map_indexed (for explicit terrain-style indexed mapping) */
			else if ( !Q_stricmp( mattoken, "q3map_indexed" ) || !Q_stricmp( mattoken, "vmap_indexed" ) ) {
				si->indexed = qtrue;
			}

			/* ydnar: q3map_invert (inverts a drawsurface's facing) */
			else if ( !Q_stricmp( mattoken, "q3map_invert" ) || !Q_stricmp( mattoken, "vmap_invert" ) ) {
				si->invert = qtrue;
			}

			/* ydnar: gs mods: q3map_lightmapMergable (ok to merge non-planar */
			else if ( !Q_stricmp( mattoken, "q3map_lightmapMergable" ) || !Q_stricmp( mattoken, "vmap_lightmapMergable" ) ) {
				si->lmMergable = qtrue;
			}

			/* ydnar: q3map_nofast */
			else if ( !Q_stricmp( mattoken, "q3map_noFast" ) || !Q_stricmp( mattoken, "vmap_noFast" ) ) {
				si->noFast = qtrue;
			}

			/* q3map_patchshadows */
			else if ( !Q_stricmp( mattoken, "q3map_patchShadows" ) || !Q_stricmp( mattoken, "vmap_patchShadows" ) ) {
				si->patchShadows = qtrue;
			}

			/* q3map_vertexshadows */
			else if ( !Q_stricmp( mattoken, "q3map_vertexShadows" ) || !Q_stricmp( mattoken, "vmap_vertexShadows" ) ) {
				si->vertexShadows = qtrue;          /* ydnar */

			}
			/* q3map_novertexshadows */
			else if ( !Q_stricmp( mattoken, "q3map_noVertexShadows" ) || !Q_stricmp( mattoken, "vmap_noVertexShadows" ) ) {
				si->vertexShadows = qfalse;         /* ydnar */

			}
			/* q3map_splotchfix (filter dark lightmap luxels on lightmapped models) */
			else if ( !Q_stricmp( mattoken, "q3map_splotchfix" ) || !Q_stricmp( mattoken, "vmap_splotchfix" ) ) {
				si->splotchFix = qtrue;         /* ydnar */

			}
			/* q3map_forcesunlight */
			else if ( !Q_stricmp( mattoken, "q3map_forceSunlight" ) || !Q_stricmp( mattoken, "vmap_forceSunlight" ) ) {
				si->forceSunlight = qtrue;
			}

			/* q3map_onlyvertexlighting (sof2) */
			else if ( !Q_stricmp( mattoken, "q3map_onlyVertexLighting" ) || !Q_stricmp( mattoken, "vmap_onlyVertexLighting" ) ) {
				ApplySurfaceParm( "pointlight", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );
			}

			/* q3map_material (sof2) */
			else if ( !Q_stricmp( mattoken, "q3map_material" ) || !Q_stricmp( mattoken, "vmap_material" ) ) {
				GetMatTokenAppend( shaderText, qfalse );
				sprintf( temp, "*mat_%s", mattoken );
				if ( ApplySurfaceParm( temp, &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown material \"%s\"\n", mattoken );
				}
			}

			/* ydnar: q3map_clipmodel (autogenerate clip brushes for model triangles using this shader) */
			else if ( !Q_stricmp( mattoken, "q3map_clipmodel" ) || !Q_stricmp( mattoken, "vmap_clipmodel" ) ) {
				si->clipModel = qtrue;
			}

			/* ydnar: q3map_styleMarker[2] */
			else if ( !Q_stricmp( mattoken, "q3map_styleMarker" ) || !Q_stricmp( mattoken, "vmap_styleMarker" ) ) {
				si->styleMarker = 1;
			}
			else if ( !Q_stricmp( mattoken, "q3map_styleMarker2" ) || !Q_stricmp( mattoken, "vmap_styleMarker2" ) ) {          /* uses depthFunc equal */
				si->styleMarker = 2;
			}

			/* ydnar: default to searching for q3map_<surfaceparm> */
#if 0
			else
			{
				Sys_FPrintf( SYS_VRB, "Attempting to match %s with a known surfaceparm\n", mattoken );
				if ( ApplySurfaceParm( &mattoken[ 6 ], &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_* directive \"%s\"\n", mattoken );
				}
			}
#endif
		}
	}
#endif

	LoadShaderImages( si );
	FinishShader( si );

	/* return it */
	return si;
}



/*
   GetTokenAppend() - ydnar
   gets a token and appends its text to the specified buffer
 */

static int oldScriptLine = 0;
static int tabDepth = 0;

qboolean GetTokenAppend( char *buffer, qboolean crossline ){
	qboolean r;
	int i;


	/* get the token */
	r = GetToken( crossline );
	if ( r == qfalse || buffer == NULL || token[ 0 ] == '\0' ) {
		return r;
	}

	/* pre-tabstops */
	if ( token[ 0 ] == '}' ) {
		tabDepth--;
	}

	/* append? */
	if ( oldScriptLine != scriptline ) {
		strcat( buffer, "\n" );
		for ( i = 0; i < tabDepth; i++ )
			strcat( buffer, "\t" );
	}
	else{
		strcat( buffer, " " );
	}
	oldScriptLine = scriptline;
	strcat( buffer, token );

	/* post-tabstops */
	if ( token[ 0 ] == '{' ) {
		tabDepth++;
	}

	/* return */
	return r;
}


void Parse1DMatrixAppend( char *buffer, int x, vec_t *m ){
	int i;


	if ( !GetTokenAppend( buffer, qtrue ) || strcmp( token, "(" ) ) {
		Error( "Parse1DMatrixAppend(): line %d: ( not found!", scriptline );
	}
	for ( i = 0; i < x; i++ )
	{
		if ( !GetTokenAppend( buffer, qfalse ) ) {
			Error( "Parse1DMatrixAppend(): line %d: Number not found!", scriptline );
		}
		m[ i ] = atof( token );
	}
	if ( !GetTokenAppend( buffer, qtrue ) || strcmp( token, ")" ) ) {
		Error( "Parse1DMatrixAppend(): line %d: ) not found!", scriptline );
	}
}




/*
   ParseShaderFile()
   parses a shader file into discrete shaderInfo_t
 */

static void ParseShaderFile( const char *filename ){
	int i, val;
	shaderInfo_t    *si;
	char            *suffix, temp[ 1024 ];
	char shaderText[ 8192 ];            /* ydnar: fixme (make this bigger?) */


	/* init */
	si = NULL;
	shaderText[ 0 ] = '\0';

	/* load the shader */
	LoadScriptFile( filename, 0 );

	/* tokenize it */
	while ( 1 )
	{
		/* copy shader text to the shaderinfo */
		if ( si != NULL && shaderText[ 0 ] != '\0' ) {
			strcat( shaderText, "\n" );
			si->shaderText = safe_malloc( strlen( shaderText ) + 1 );
			strcpy( si->shaderText, shaderText );
			//%	if( VectorLength( si->vecs[ 0 ] ) )
			//%		Sys_Printf( "%s\n", shaderText );
		}

		/* ydnar: clear shader text buffer */
		shaderText[ 0 ] = '\0';

		/* test for end of file */
		if ( !GetToken( qtrue ) ) {
			break;
		}

		/* shader name is initial token */
		si = AllocShaderInfo();
		strcpy( si->shader, token );

		/* ignore ":q3map" suffix */
		suffix = strstr( si->shader, ":q3map" );
		if ( suffix != NULL ) {
			*suffix = '\0';
		}

		/* handle { } section */
		if ( !GetTokenAppend( shaderText, qtrue ) ) {
			break;
		}
		if ( strcmp( token, "{" ) ) {
			if ( si != NULL ) {
				Error( "ParseShaderFile(): %s, line %d: { not found!\nFound instead: %s\nLast known shader: %s",
				       filename, scriptline, token, si->shader );
			}
			else{
				Error( "ParseShaderFile(): %s, line %d: { not found!\nFound instead: %s",
				       filename, scriptline, token );
			}
		}

		while ( 1 )
		{
			/* get the next token */
			if ( !GetTokenAppend( shaderText, qtrue ) ) {
				break;
			}
			if ( !strcmp( token, "}" ) ) {
				break;
			}


			/* -----------------------------------------------------------------
			   shader stages (passes)
			   ----------------------------------------------------------------- */

			/* parse stage directives */
			if ( !strcmp( token, "{" ) ) {
				si->hasPasses = qtrue;
				while ( 1 )
				{
					if ( !GetTokenAppend( shaderText, qtrue ) ) {
						break;
					}
					if ( !strcmp( token, "}" ) ) {
						break;
					}

					/* only care about images if we don't have a editor/light image */
					if ( si->editorImagePath[ 0 ] == '\0' && si->lightImagePath[ 0 ] == '\0' && si->implicitImagePath[ 0 ] == '\0' ) {
						/* digest any images */
						if ( !Q_stricmp( token, "map" ) ||
						     !Q_stricmp( token, "clampMap" ) ||
						     !Q_stricmp( token, "animMap" ) ||
						     !Q_stricmp( token, "clampAnimMap" ) ||
						     !Q_stricmp( token, "clampMap" ) ||
						     !Q_stricmp( token, "mapComp" ) ||
						     !Q_stricmp( token, "mapNoComp" ) ) {
							/* skip one token for animated stages */
							if ( !Q_stricmp( token, "animMap" ) || !Q_stricmp( token, "clampAnimMap" ) ) {
								GetTokenAppend( shaderText, qfalse );
							}
						}
					}
				}
			}


			/* -----------------------------------------------------------------
			   surfaceparm * directives
			   ----------------------------------------------------------------- */

			/* match surfaceparm */
			else if ( !Q_stricmp( token, "surfaceparm" ) ) {
				GetTokenAppend( shaderText, qfalse );
				if ( ApplySurfaceParm( token, &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Unknown surfaceparm: \"%s\"\n", token );
				}
			}


			/* -----------------------------------------------------------------
			   game-related shader directives
			   ----------------------------------------------------------------- */

			/* ydnar: fogparms (for determining fog volumes) */
			else if ( !Q_stricmp( token, "fogparms" ) ) {
				si->fogParms = qtrue;
			}

			/* ydnar: polygonoffset (for no culling) */
			else if ( !Q_stricmp( token, "polygonoffset" ) ) {
				si->polygonOffset = qtrue;
			}

			/* tesssize is used to force liquid surfaces to subdivide */
			else if ( !Q_stricmp( token, "tessSize" ) ) {
				GetTokenAppend( shaderText, qfalse );
				si->subdivisions = atof( token );
			}

			/* cull none will set twoSided (ydnar: added disable too) */
			else if ( !Q_stricmp( token, "cull" ) ) {
				GetTokenAppend( shaderText, qfalse );
				if ( !Q_stricmp( token, "none" ) || !Q_stricmp( token, "disable" ) || !Q_stricmp( token, "twosided" ) ) {
					si->twoSided = qtrue;
				}
			}

			/* deformVertexes autosprite[ 2 ]
			   we catch this so autosprited surfaces become point
			   lights instead of area lights */
			else if ( !Q_stricmp( token, "deformVertexes" ) ) {
				GetTokenAppend( shaderText, qfalse );

				/* deformVertexes autosprite(2) */
				if ( !Q_strncasecmp( token, "autosprite", 10 ) ) {
					/* set it as autosprite and detail */
					si->autosprite = qtrue;
					ApplySurfaceParm( "detail", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );

					/* ydnar: gs mods: added these useful things */
					si->noClip = qtrue;
					si->notjunc = qtrue;
				}

				/* deformVertexes move <x> <y> <z> <func> <base> <amplitude> <phase> <freq> (ydnar: for particle studio support) */
				if ( !Q_stricmp( token, "move" ) ) {
					vec3_t amt, mins, maxs;
					float base, amp;


					/* get move amount */
					GetTokenAppend( shaderText, qfalse );   amt[ 0 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );   amt[ 1 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );   amt[ 2 ] = atof( token );

					/* skip func */
					GetTokenAppend( shaderText, qfalse );

					/* get base and amplitude */
					GetTokenAppend( shaderText, qfalse );   base = atof( token );
					GetTokenAppend( shaderText, qfalse );   amp = atof( token );

					/* calculate */
					VectorScale( amt, base, mins );
					VectorMA( mins, amp, amt, maxs );
					VectorAdd( si->mins, mins, si->mins );
					VectorAdd( si->maxs, maxs, si->maxs );
				}
			}

			/* ydnar: damageShader <shader> <health> (sof2 mods) */
			else if ( !Q_stricmp( token, "damageShader" ) ) {
				GetTokenAppend( shaderText, qfalse );
				if ( token[ 0 ] != '\0' ) {
					si->damageShader = safe_malloc( strlen( token ) + 1 );
					strcpy( si->damageShader, token );
				}
				GetTokenAppend( shaderText, qfalse );   /* don't do anything with health */
			}

			/* ydnar: enemy territory implicit shaders */
			else if ( !Q_stricmp( token, "implicitMap" ) ) {
				si->implicitMap = IM_OPAQUE;
				GetTokenAppend( shaderText, qfalse );
				if ( token[ 0 ] == '-' && token[ 1 ] == '\0' ) {
					sprintf( si->implicitImagePath, "%s.tga", si->shader );
				}
				else{
					strcpy( si->implicitImagePath, token );
				}
			}

			else if ( !Q_stricmp( token, "implicitMask" ) ) {
				si->implicitMap = IM_MASKED;
				GetTokenAppend( shaderText, qfalse );
				if ( token[ 0 ] == '-' && token[ 1 ] == '\0' ) {
					sprintf( si->implicitImagePath, "%s.tga", si->shader );
				}
				else{
					strcpy( si->implicitImagePath, token );
				}
			}

			else if ( !Q_stricmp( token, "implicitBlend" ) ) {
				si->implicitMap = IM_MASKED;
				GetTokenAppend( shaderText, qfalse );
				if ( token[ 0 ] == '-' && token[ 1 ] == '\0' ) {
					sprintf( si->implicitImagePath, "%s.tga", si->shader );
				}
				else{
					strcpy( si->implicitImagePath, token );
				}
			}


			/* -----------------------------------------------------------------
			   image directives
			   ----------------------------------------------------------------- */

			/* qer_editorimage <image> */
			else if ( !Q_stricmp( token, "qer_editorImage" ) ) {
				GetTokenAppend( shaderText, qfalse );
				strcpy( si->editorImagePath, token );
				DefaultExtension( si->editorImagePath, ".tga" );
			}

			/* diffusemap <image> */
			else if ( !Q_stricmp( token, "diffusemap" ) ) {
				GetTokenAppend( shaderText, qfalse );
				strcpy( si->editorImagePath, token );
				DefaultExtension( si->editorImagePath, ".tga" );
			}

			/* ydnar: q3map_normalimage <image> (bumpmapping normal map) */
			/*else if ( !Q_stricmp( token, "q3map_normalImage" ) ) {
			        GetTokenAppend( shaderText, qfalse );
			        strcpy( si->normalImagePath, token );
			        DefaultExtension( si->normalImagePath, ".tga" );
			   }*/

			/* q3map_lightimage <image> */
			else if ( !Q_stricmp( token, "q3map_lightImage" ) || !Q_stricmp( token, "vmap_lightImage" ) ) {
				GetTokenAppend( shaderText, qfalse );
				strcpy( si->lightImagePath, token );
				DefaultExtension( si->lightImagePath, ".tga" );
			}

			/* ydnar: skyparms <outer image> <cloud height> <inner image> */
			else if ( !Q_stricmp( token, "skyParms" ) ) {
				/* get image base */
				GetTokenAppend( shaderText, qfalse );

				/* ignore bogus paths */
				if ( Q_stricmp( token, "-" ) && Q_stricmp( token, "full" ) ) {
					strcpy( si->skyParmsImageBase, token );
				}

				/* skip rest of line */
				GetTokenAppend( shaderText, qfalse );
				GetTokenAppend( shaderText, qfalse );
			}

			/* -----------------------------------------------------------------
			   q3map_* directives
			   ----------------------------------------------------------------- */

			/* q3map_sun <red> <green> <blue> <intensity> <degrees> <elevation>
			   color will be normalized, so it doesn't matter what range you use
			   intensity falls off with angle but not distance 100 is a fairly bright sun
			   degree of 0 = from the east, 90 = north, etc.  altitude of 0 = sunrise/set, 90 = noon */
#if 0
			else if ( !Q_stricmp( token, "vmap_sun" ) || !Q_stricmp( token, "q3map_sun" ) || !Q_stricmp( token, "q3map_sunExt" ) ) {
				float a, b;
				sun_t       *sun;
				qboolean ext = qfalse;

				/* ydnar: extended sun directive? */
				if ( !Q_stricmp( token, "q3map_sunext" ) || !Q_stricmp( token, "vmap_sun" ) ) {
					ext = qtrue;
				}

				/* allocate sun */
				sun = safe_malloc( sizeof( *sun ) );
				memset( sun, 0, sizeof( *sun ) );

				/* set style */
				sun->style = si->lightStyle;

				/* get color */
				GetTokenAppend( shaderText, qfalse );
				sun->color[ 0 ] = atof( token );
				GetTokenAppend( shaderText, qfalse );
				sun->color[ 1 ] = atof( token );
				GetTokenAppend( shaderText, qfalse );
				sun->color[ 2 ] = atof( token );

				if ( colorsRGB ) {
					sun->color[0] = Image_LinearFloatFromsRGBFloat( sun->color[0] );
					sun->color[1] = Image_LinearFloatFromsRGBFloat( sun->color[1] );
					sun->color[2] = Image_LinearFloatFromsRGBFloat( sun->color[2] );
				}

				/* normalize it */
				ColorNormalize( sun->color, sun->color );

				/* scale color by brightness */
				GetTokenAppend( shaderText, qfalse );
				sun->photons = atof( token );

				/* get sun angle/elevation */
				GetTokenAppend( shaderText, qfalse );
				a = atof( token );
				a = a / 180.0f * Q_PI;

				GetTokenAppend( shaderText, qfalse );
				b = atof( token );
				b = b / 180.0f * Q_PI;

				sun->direction[ 0 ] = cos( a ) * cos( b );
				sun->direction[ 1 ] = sin( a ) * cos( b );
				sun->direction[ 2 ] = sin( b );

				/* get filter radius from shader */
				sun->filterRadius = si->lightFilterRadius;

				/* ydnar: get sun angular deviance/samples */
				if ( ext && TokenAvailable() ) {
					GetTokenAppend( shaderText, qfalse );
					sun->deviance = atof( token );
					sun->deviance = sun->deviance / 180.0f * Q_PI;

					GetTokenAppend( shaderText, qfalse );
					sun->numSamples = atoi( token );
				}

				/* store sun */
				sun->next = si->sun;
				si->sun = sun;

				/* apply sky surfaceparm */
				ApplySurfaceParm( "sky", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );

				/* don't process any more tokens on this line */
				continue;
			}
#endif
			/* match q3map_ */
			else if ( !Q_strncasecmp( token, "q3map_", 6 ) || !Q_strncasecmp( token, "vmap_", 5 ) ) {
				/* ydnar: q3map_baseShader <shader> (inherit this shader's parameters) */
				if ( !Q_stricmp( token, "q3map_baseShader" ) || !Q_stricmp( token, "vmap_baseShader" ) ) {
					shaderInfo_t    *si2;
					qboolean oldWarnImage;


					/* get shader */
					GetTokenAppend( shaderText, qfalse );
					//%	Sys_FPrintf( SYS_VRB, "Shader %s has base shader %s\n", si->shader, token );
					oldWarnImage = warnImage;
					warnImage = qfalse;
					si2 = ShaderInfoForShader( token );
					warnImage = oldWarnImage;

					/* subclass it */
					if ( si2 != NULL ) {
						/* preserve name */
						strcpy( temp, si->shader );

						/* copy shader */
						memcpy( si, si2, sizeof( *si ) );

						/* restore name and set to unfinished */
						strcpy( si->shader, temp );
						si->shaderWidth = 0;
						si->shaderHeight = 0;
						si->finished = qfalse;
					}
				}

				/* ydnar: q3map_surfacemodel <path to model> <density> <min scale> <max scale> <min angle> <max angle> <oriented (0 or 1)> */
				else if ( !Q_stricmp( token, "q3map_surfacemodel" ) || !Q_stricmp( token, "vmap_surfacemodel" ) ) {
					surfaceModel_t  *model;

					/* allocate new model and attach it */
					model = safe_malloc( sizeof( *model ) );
					memset( model, 0, sizeof( *model ) );
					model->next = si->surfaceModel;
					si->surfaceModel = model;

					/* get parameters */
					GetTokenAppend( shaderText, qfalse );
					strcpy( model->model, token );

					GetTokenAppend( shaderText, qfalse );
					model->density = atof( token );
					GetTokenAppend( shaderText, qfalse );
					model->odds = atof( token );

					GetTokenAppend( shaderText, qfalse );
					model->minScale = atof( token );
					GetTokenAppend( shaderText, qfalse );
					model->maxScale = atof( token );

					GetTokenAppend( shaderText, qfalse );
					model->minAngle = atof( token );
					GetTokenAppend( shaderText, qfalse );
					model->maxAngle = atof( token );

					GetTokenAppend( shaderText, qfalse );
					model->oriented = ( token[ 0 ] == '1' ? qtrue : qfalse );
				}

				/* ydnar/sd: q3map_foliage <path to model> <scale> <density> <odds> <invert alpha (1 or 0)> */
				else if ( !Q_stricmp( token, "q3map_foliage" ) || !Q_stricmp( token, "vmap_foliage" ) ) {
					foliage_t   *foliage;


					/* allocate new foliage struct and attach it */
					foliage = safe_malloc( sizeof( *foliage ) );
					memset( foliage, 0, sizeof( *foliage ) );
					foliage->next = si->foliage;
					si->foliage = foliage;

					/* get parameters */
					GetTokenAppend( shaderText, qfalse );
					strcpy( foliage->model, token );

					GetTokenAppend( shaderText, qfalse );
					foliage->scale = atof( token );
					GetTokenAppend( shaderText, qfalse );
					foliage->density = atof( token );
					GetTokenAppend( shaderText, qfalse );
					foliage->odds = atof( token );
					GetTokenAppend( shaderText, qfalse );
					foliage->inverseAlpha = atoi( token );
				}

				/* ydnar: q3map_bounce <value> (fraction of light to re-emit during radiosity passes) */
				else if ( !Q_stricmp( token, "q3map_bounce" ) || !Q_stricmp( token, "vmap_bounce" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->bounceScale = atof( token );
				}
#if 0
				/* ydnar/splashdamage: q3map_skyLight <value> <iterations> */
				else if ( !Q_stricmp( token, "q3map_skyLight" ) || !Q_stricmp( token, "vmap_skyLight" )  ) {
					GetTokenAppend( shaderText, qfalse );
					si->skyLightValue = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->skyLightIterations = atoi( token );

					/* clamp */
					if ( si->skyLightValue < 0.0f ) {
						si->skyLightValue = 0.0f;
					}
					if ( si->skyLightIterations < 2 ) {
						si->skyLightIterations = 2;
					}
				}
#endif
				/* q3map_surfacelight <value> */
				else if ( !Q_stricmp( token, "q3map_surfacelight" ) || !Q_stricmp( token, "vmap_surfacelight" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->value = atof( token );
				}

				/* q3map_lightStyle (sof2/jk2 lightstyle) */
				else if ( !Q_stricmp( token, "q3map_lightStyle" ) || !Q_stricmp( token, "vmap_lightStyle" ) ) {
					GetTokenAppend( shaderText, qfalse );
					val = atoi( token );
					if ( val < 0 ) {
						val = 0;
					}
					else if ( val > LS_NONE ) {
						val = LS_NONE;
					}
					si->lightStyle = val;
				}

				/* wolf: q3map_lightRGB <red> <green> <blue> */
				else if ( !Q_stricmp( token, "q3map_lightRGB" ) || !Q_stricmp( token, "vmap_lightRGB" )) {
					VectorClear( si->color );
					GetTokenAppend( shaderText, qfalse );
					si->color[ 0 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->color[ 1 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->color[ 2 ] = atof( token );
					if ( colorsRGB ) {
						si->color[0] = Image_LinearFloatFromsRGBFloat( si->color[0] );
						si->color[1] = Image_LinearFloatFromsRGBFloat( si->color[1] );
						si->color[2] = Image_LinearFloatFromsRGBFloat( si->color[2] );
					}
					ColorNormalize( si->color, si->color );
				}

				/* q3map_lightSubdivide <value> */
				else if ( !Q_stricmp( token, "q3map_lightSubdivide" ) || !Q_stricmp( token, "vmap_lightSubdivide" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lightSubdivide = atoi( token );
				}

				/* q3map_backsplash <percent> <distance> */
				else if ( !Q_stricmp( token, "q3map_backsplash" ) || !Q_stricmp( token, "vmap_backsplash" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->backsplashFraction = atof( token ) * 0.01f;
					GetTokenAppend( shaderText, qfalse );
					si->backsplashDistance = atof( token );
				}
#if 0
				/* q3map_floodLight <r> <g> <b> <diste> <intensity> <light_direction_power> */
				else if ( !Q_stricmp( token, "q3map_floodLight" ) || !Q_stricmp( token, "vmap_floodLight" ) ) {
					/* get color */
					GetTokenAppend( shaderText, qfalse );
					si->floodlightRGB[ 0 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->floodlightRGB[ 1 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->floodlightRGB[ 2 ] = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->floodlightDistance = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->floodlightIntensity = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->floodlightDirectionScale = atof( token );
					if ( colorsRGB ) {
						si->floodlightRGB[0] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[0] );
						si->floodlightRGB[1] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[1] );
						si->floodlightRGB[2] = Image_LinearFloatFromsRGBFloat( si->floodlightRGB[2] );
					}
					ColorNormalize( si->floodlightRGB, si->floodlightRGB );
				}
#endif
				/* jal: q3map_nodirty : skip dirty */
				else if ( !Q_stricmp( token, "q3map_nodirty" ) || !Q_stricmp( token, "vmap_nodirty" ) ) {
					si->noDirty = qtrue;
				}

				/* q3map_lightmapSampleSize <value> */
				else if ( !Q_stricmp( token, "q3map_lightmapSampleSize" ) || !Q_stricmp( token, "vmap_lightmapSampleSize" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lightmapSampleSize = atoi( token );
				}

				/* q3map_lightmapSampleOffset <value> */
				else if ( !Q_stricmp( token, "q3map_lightmapSampleOffset" ) || !Q_stricmp( token, "vmap_lightmapSampleOffset" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lightmapSampleOffset = atof( token );
				}

				/* ydnar: q3map_lightmapFilterRadius <self> <other> */
				else if ( !Q_stricmp( token, "q3map_lightmapFilterRadius" ) || !Q_stricmp( token, "vmap_lightmapFilterRadius" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lmFilterRadius = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->lightFilterRadius = atof( token );
				}

				/* ydnar: q3map_lightmapAxis [xyz] */
				else if ( !Q_stricmp( token, "q3map_lightmapAxis" ) || !Q_stricmp( token, "vmap_lightmapAxis" ) ) {
					GetTokenAppend( shaderText, qfalse );
					if ( !Q_stricmp( token, "x" ) ) {
						VectorSet( si->lightmapAxis, 1, 0, 0 );
					}
					else if ( !Q_stricmp( token, "y" ) ) {
						VectorSet( si->lightmapAxis, 0, 1, 0 );
					}
					else if ( !Q_stricmp( token, "z" ) ) {
						VectorSet( si->lightmapAxis, 0, 0, 1 );
					}
					else
					{
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown value for lightmap axis: %s\n", token );
						VectorClear( si->lightmapAxis );
					}
				}

				/* ydnar: q3map_lightmapSize <width> <height> (for autogenerated shaders + external tga lightmaps) */
				else if ( !Q_stricmp( token, "q3map_lightmapSize" ) || !Q_stricmp( token, "vmap_lightmapSize" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lmCustomWidth = atoi( token );
					GetTokenAppend( shaderText, qfalse );
					si->lmCustomHeight = atoi( token );

					/* must be a power of 2 */
					if ( ( ( si->lmCustomWidth - 1 ) & si->lmCustomWidth ) ||
					     ( ( si->lmCustomHeight - 1 ) & si->lmCustomHeight ) ) {
						Sys_FPrintf( SYS_WRN, "WARNING: Non power-of-two lightmap size specified (%d, %d)\n",
						             si->lmCustomWidth, si->lmCustomHeight );
						si->lmCustomWidth = lmCustomSize;
						si->lmCustomHeight = lmCustomSize;
					}
				}

				/* ydnar: q3map_lightmapBrightness N (for autogenerated shaders + external tga lightmaps) */
				else if ( !Q_stricmp( token, "q3map_lightmapBrightness" ) || !Q_stricmp( token, "q3map_lightmapGamma" ) || !Q_stricmp( token, "vmap_lightmapBrightness" ) || !Q_stricmp( token, "vmap_lightmapGamma" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->lmBrightness = atof( token );
					if ( si->lmBrightness < 0 ) {
						si->lmBrightness = 1.0;
					}
				}

				/* q3map_vertexScale (scale vertex lighting by this fraction) */
				else if ( !Q_stricmp( token, "q3map_vertexScale" ) || !Q_stricmp( token, "vmap_vertexScale" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->vertexScale = atof( token );
				}

				/* q3map_noVertexLight */
				else if ( !Q_stricmp( token, "q3map_noVertexLight" ) || !Q_stricmp( token, "vmap_noVertexLight" ) ) {
					si->noVertexLight = qtrue;
				}

				/* q3map_backShader <shader> */
				else if ( !Q_stricmp( token, "q3map_backShader" ) || !Q_stricmp( token, "vmap_backShader" ) ) {
					GetTokenAppend( shaderText, qfalse );
					if ( token[ 0 ] != '\0' ) {
						si->backShader = safe_malloc( strlen( token ) + 1 );
						strcpy( si->backShader, token );
					}
				}

				/* ydnar: q3map_cloneShader <shader> */
				else if ( !Q_stricmp( token, "q3map_cloneShader" ) || !Q_stricmp( token, "vmap_cloneShader" ) ) {
					GetTokenAppend( shaderText, qfalse );
					if ( token[ 0 ] != '\0' ) {
						si->cloneShader = safe_malloc( strlen( token ) + 1 );
						strcpy( si->cloneShader, token );
					}
				}

				/* q3map_remapShader <shader> */
				else if ( !Q_stricmp( token, "q3map_remapShader" ) || !Q_stricmp( token, "vmap_remapShader" ) ) {
					GetTokenAppend( shaderText, qfalse );
					if ( token[ 0 ] != '\0' ) {
						si->remapShader = safe_malloc( strlen( token ) + 1 );
						strcpy( si->remapShader, token );
					}
				}

				/* q3map_deprecateShader <shader> */
				else if ( !Q_stricmp( token, "q3map_deprecateShader" ) || !Q_stricmp( token, "vmap_deprecateShader" ) ) {
					GetTokenAppend( shaderText, qfalse );
					if ( token[ 0 ] != '\0' ) {

						si->deprecateShader = safe_malloc( strlen( token ) + 1 );
						strcpy( si->deprecateShader, token );
					}
				}

				/* ydnar: q3map_offset <value> */
				else if ( !Q_stricmp( token, "q3map_offset" ) || !Q_stricmp( token, "vmap_offset" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->offset = atof( token );
				}

				/* ydnar: q3map_fur <numlayers> <offset> <fade> */
				else if ( !Q_stricmp( token, "q3map_fur" ) || !Q_stricmp( token, "vmap_fur" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->furNumLayers = atoi( token );
					GetTokenAppend( shaderText, qfalse );
					si->furOffset = atof( token );
					GetTokenAppend( shaderText, qfalse );
					si->furFade = atof( token );
				}

				/* ydnar: gs mods: legacy support for terrain/terrain2 shaders */
				else if ( !Q_stricmp( token, "q3map_terrain" ) || !Q_stricmp( token, "vmap_terrain" ) ) {
					/* team arena terrain is assumed to be nonplanar, with full normal averaging,
					   passed through the metatriangle surface pipeline, with a lightmap axis on z */
					si->legacyTerrain = qtrue;
					si->noClip = qtrue;
					si->notjunc = qtrue;
					si->indexed = qtrue;
					si->nonplanar = qtrue;
					si->forceMeta = qtrue;
					si->shadeAngleDegrees = 179.0f;
					//%	VectorSet( si->lightmapAxis, 0, 0, 1 );	/* ydnar 2002-09-21: turning this off for better lightmapping of cliff faces */
				}

				/* ydnar: picomodel: q3map_forceMeta (forces brush faces and/or triangle models to go through the metasurface pipeline) */
				else if ( !Q_stricmp( token, "q3map_forceMeta" ) || !Q_stricmp( token, "vmap_forceMeta" ) ) {
					si->forceMeta = qtrue;
				}

				/* ydnar: gs mods: q3map_shadeAngle <degrees> */
				else if ( !Q_stricmp( token, "q3map_shadeAngle" ) || !Q_stricmp( token, "vmap_shadeAngle" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->shadeAngleDegrees = atof( token );
				}

				/* ydnar: q3map_textureSize <width> <height> (substitute for q3map_lightimage derivation for terrain) */
				else if ( !Q_stricmp( token, "q3map_textureSize" ) || !Q_stricmp( token, "vmap_textureSize" ) ) {
					GetTokenAppend( shaderText, qfalse );
					si->shaderWidth = atoi( token );
					GetTokenAppend( shaderText, qfalse );
					si->shaderHeight = atoi( token );
				}

				/* ydnar: gs mods: q3map_tcGen <style> <parameters> */
				else if ( !Q_stricmp( token, "q3map_tcGen" ) || !Q_stricmp( token, "vmap_tcGen" ) ) {
					si->tcGen = qtrue;
					GetTokenAppend( shaderText, qfalse );

					/* q3map_tcGen vector <s vector> <t vector> */
					if ( !Q_stricmp( token, "vector" ) ) {
						Parse1DMatrixAppend( shaderText, 3, si->vecs[ 0 ] );
						Parse1DMatrixAppend( shaderText, 3, si->vecs[ 1 ] );
					}

					/* q3map_tcGen ivector <1.0/s vector> <1.0/t vector> (inverse vector, easier for mappers to understand) */
					else if ( !Q_stricmp( token, "ivector" ) ) {
						Parse1DMatrixAppend( shaderText, 3, si->vecs[ 0 ] );
						Parse1DMatrixAppend( shaderText, 3, si->vecs[ 1 ] );
						for ( i = 0; i < 3; i++ )
						{
							si->vecs[ 0 ][ i ] = si->vecs[ 0 ][ i ] ? 1.0 / si->vecs[ 0 ][ i ] : 0;
							si->vecs[ 1 ][ i ] = si->vecs[ 1 ][ i ] ? 1.0 / si->vecs[ 1 ][ i ] : 0;
						}
					}
					else
					{
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_tcGen method: %s\n", token );
						VectorClear( si->vecs[ 0 ] );
						VectorClear( si->vecs[ 1 ] );
					}
				}

				/* ydnar: gs mods: q3map_[color|rgb|alpha][Gen|Mod] <style> <parameters> */
				else if ( !Q_stricmp( token, "q3map_colorGen" ) || !Q_stricmp( token, "q3map_colorMod" ) ||
				          !Q_stricmp( token, "q3map_rgbGen" ) || !Q_stricmp( token, "q3map_rgbMod" ) ||
				          !Q_stricmp( token, "q3map_alphaGen" ) || !Q_stricmp( token, "q3map_alphaMod" ) ) {
					colorMod_t  *cm, *cm2;
					int alpha;


					/* alphamods are colormod + 1 */
					alpha = ( !Q_stricmp( token, "q3map_alphaGen" ) || !Q_stricmp( token, "q3map_alphaMod" ) ) ? 1 : 0;

					/* allocate new colormod */
					cm = safe_malloc( sizeof( *cm ) );
					memset( cm, 0, sizeof( *cm ) );

					/* attach to shader */
					if ( si->colorMod == NULL ) {
						si->colorMod = cm;
					}
					else
					{
						for ( cm2 = si->colorMod; cm2 != NULL; cm2 = cm2->next )
						{
							if ( cm2->next == NULL ) {
								cm2->next = cm;
								break;
							}
						}
					}

					/* get type */
					GetTokenAppend( shaderText, qfalse );

					/* alpha set|const A */
					if ( alpha && ( !Q_stricmp( token, "set" ) || !Q_stricmp( token, "const" ) ) ) {
						cm->type = CM_ALPHA_SET;
						GetTokenAppend( shaderText, qfalse );
						cm->data[ 0 ] = atof( token );
					}

					/* color|rgb set|const ( X Y Z ) */
					else if ( !Q_stricmp( token, "set" ) || !Q_stricmp( token, "const" ) ) {
						cm->type = CM_COLOR_SET;
						Parse1DMatrixAppend( shaderText, 3, cm->data );
						if ( colorsRGB ) {
							cm->data[0] = Image_LinearFloatFromsRGBFloat( cm->data[0] );
							cm->data[1] = Image_LinearFloatFromsRGBFloat( cm->data[1] );
							cm->data[2] = Image_LinearFloatFromsRGBFloat( cm->data[2] );
						}
					}

					/* alpha scale A */
					else if ( alpha && !Q_stricmp( token, "scale" ) ) {
						cm->type = CM_ALPHA_SCALE;
						GetTokenAppend( shaderText, qfalse );
						cm->data[ 0 ] = atof( token );
					}

					/* color|rgb scale ( X Y Z ) */
					else if ( !Q_stricmp( token, "scale" ) ) {
						cm->type = CM_COLOR_SCALE;
						Parse1DMatrixAppend( shaderText, 3, cm->data );
					}

					/* dotProduct ( X Y Z ) */
					else if ( !Q_stricmp( token, "dotProduct" ) ) {
						cm->type = CM_COLOR_DOT_PRODUCT + alpha;
						Parse1DMatrixAppend( shaderText, 3, cm->data );
					}

					/* dotProductScale ( X Y Z MIN MAX ) */
					else if ( !Q_stricmp( token, "dotProductScale" ) ) {
						cm->type = CM_COLOR_DOT_PRODUCT_SCALE + alpha;
						Parse1DMatrixAppend( shaderText, 5, cm->data );
					}

					/* dotProduct2 ( X Y Z ) */
					else if ( !Q_stricmp( token, "dotProduct2" ) ) {
						cm->type = CM_COLOR_DOT_PRODUCT_2 + alpha;
						Parse1DMatrixAppend( shaderText, 3, cm->data );
					}

					/* dotProduct2scale ( X Y Z MIN MAX ) */
					else if ( !Q_stricmp( token, "dotProduct2scale" ) ) {
						cm->type = CM_COLOR_DOT_PRODUCT_2_SCALE + alpha;
						Parse1DMatrixAppend( shaderText, 5, cm->data );
					}

					/* volume */
					else if ( !Q_stricmp( token, "volume" ) ) {
						/* special stub mode for flagging volume brushes */
						cm->type = CM_VOLUME;
					}

					/* unknown */
					else{
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown colorMod method: %s\n", token );
					}
				}

				/* ydnar: gs mods: q3map_tcMod <style> <parameters> */
				else if ( !Q_stricmp( token, "q3map_tcMod" ) || !Q_stricmp( token, "vmap_tcMod" ) ) {
					float a, b;


					GetTokenAppend( shaderText, qfalse );

					/* q3map_tcMod [translate | shift | offset] <s> <t> */
					if ( !Q_stricmp( token, "translate" ) || !Q_stricmp( token, "shift" ) || !Q_stricmp( token, "offset" ) ) {
						GetTokenAppend( shaderText, qfalse );
						a = atof( token );
						GetTokenAppend( shaderText, qfalse );
						b = atof( token );

						TCModTranslate( si->mod, a, b );
					}

					/* q3map_tcMod scale <s> <t> */
					else if ( !Q_stricmp( token, "scale" ) ) {
						GetTokenAppend( shaderText, qfalse );
						a = atof( token );
						GetTokenAppend( shaderText, qfalse );
						b = atof( token );

						TCModScale( si->mod, a, b );
					}

					/* q3map_tcMod rotate <s> <t> (fixme: make this communitive) */
					else if ( !Q_stricmp( token, "rotate" ) ) {
						GetTokenAppend( shaderText, qfalse );
						a = atof( token );
						TCModRotate( si->mod, a );
					}
					else{
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_tcMod method: %s\n", token );
					}
				}

				/* q3map_fogDir (direction a fog shader fades from transparent to opaque) */
				else if ( !Q_stricmp( token, "q3map_fogDir" ) || !Q_stricmp( token, "vmap_fogDir" ) ) {
					Parse1DMatrixAppend( shaderText, 3, si->fogDir );
					VectorNormalize( si->fogDir, si->fogDir );
				}

				/* q3map_globaltexture */
				else if ( !Q_stricmp( token, "q3map_globaltexture" ) || !Q_stricmp( token, "vmap_globaltexture" )  ) {
					si->globalTexture = qtrue;
				}

				/* ydnar: gs mods: q3map_nonplanar (make it a nonplanar merge candidate for meta surfaces) */
				else if ( !Q_stricmp( token, "q3map_nonplanar" ) || !Q_stricmp( token, "vmap_nonplanar" ) ) {
					si->nonplanar = qtrue;
				}

				/* ydnar: gs mods: q3map_noclip (preserve original face winding, don't clip by bsp tree) */
				else if ( !Q_stricmp( token, "q3map_noclip" ) || !Q_stricmp( token, "vmap_noclip" ) ) {
					si->noClip = qtrue;
				}

				/* q3map_notjunc */
				else if ( !Q_stricmp( token, "q3map_notjunc" ) || !Q_stricmp( token, "vmap_notjunc" ) ) {
					si->notjunc = qtrue;
				}

				/* q3map_nofog */
				else if ( !Q_stricmp( token, "q3map_nofog" ) || !Q_stricmp( token, "vmap_nofog" ) ) {
					si->noFog = qtrue;
				}

				/* ydnar: gs mods: q3map_indexed (for explicit terrain-style indexed mapping) */
				else if ( !Q_stricmp( token, "q3map_indexed" ) || !Q_stricmp( token, "vmap_indexed" ) ) {
					si->indexed = qtrue;
				}

				/* ydnar: q3map_invert (inverts a drawsurface's facing) */
				else if ( !Q_stricmp( token, "q3map_invert" ) || !Q_stricmp( token, "vmap_invert" ) ) {
					si->invert = qtrue;
				}

				/* ydnar: gs mods: q3map_lightmapMergable (ok to merge non-planar */
				else if ( !Q_stricmp( token, "q3map_lightmapMergable" ) || !Q_stricmp( token, "vmap_lightmapMergable" ) ) {
					si->lmMergable = qtrue;
				}

				/* ydnar: q3map_nofast */
				else if ( !Q_stricmp( token, "q3map_noFast" ) || !Q_stricmp( token, "vmap_noFast" ) ) {
					si->noFast = qtrue;
				}

				/* q3map_patchshadows */
				else if ( !Q_stricmp( token, "q3map_patchShadows" ) || !Q_stricmp( token, "vmap_patchShadows" ) ) {
					si->patchShadows = qtrue;
				}

				/* q3map_vertexshadows */
				else if ( !Q_stricmp( token, "q3map_vertexShadows" ) || !Q_stricmp( token, "vmap_vertexShadows" ) ) {
					si->vertexShadows = qtrue;  /* ydnar */

				}
				/* q3map_novertexshadows */
				else if ( !Q_stricmp( token, "q3map_noVertexShadows" ) || !Q_stricmp( token, "vmap_noVertexShadows" ) ) {
					si->vertexShadows = qfalse; /* ydnar */

				}
				/* q3map_splotchfix (filter dark lightmap luxels on lightmapped models) */
				else if ( !Q_stricmp( token, "q3map_splotchfix" ) || !Q_stricmp( token, "vmap_splotchfix" ) ) {
					si->splotchFix = qtrue; /* ydnar */

				}
				/* q3map_forcesunlight */
				else if ( !Q_stricmp( token, "q3map_forceSunlight" ) || !Q_stricmp( token, "vmap_forceSunlight" ) ) {
					si->forceSunlight = qtrue;
				}

				/* q3map_onlyvertexlighting (sof2) */
				else if ( !Q_stricmp( token, "q3map_onlyVertexLighting" ) || !Q_stricmp( token, "vmap_onlyVertexLighting" ) ) {
					ApplySurfaceParm( "pointlight", &si->contentFlags, &si->surfaceFlags, &si->compileFlags );
				}

				/* q3map_material (sof2) */
				else if ( !Q_stricmp( token, "q3map_material" ) || !Q_stricmp( token, "vmap_material" ) ) {
					GetTokenAppend( shaderText, qfalse );
					sprintf( temp, "*mat_%s", token );
					if ( ApplySurfaceParm( temp, &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown material \"%s\"\n", token );
					}
				}

				/* ydnar: q3map_clipmodel (autogenerate clip brushes for model triangles using this shader) */
				else if ( !Q_stricmp( token, "q3map_clipmodel" ) || !Q_stricmp( token, "vmap_clipmodel" ) ) {
					si->clipModel = qtrue;
				}

				/* ydnar: q3map_styleMarker[2] */
				else if ( !Q_stricmp( token, "q3map_styleMarker" ) || !Q_stricmp( token, "vmap_styleMarker" ) ) {
					si->styleMarker = 1;
				}
				else if ( !Q_stricmp( token, "q3map_styleMarker2" ) || !Q_stricmp( token, "vmap_styleMarker2" ) ) {  /* uses depthFunc equal */
					si->styleMarker = 2;
				}

				/* ydnar: default to searching for q3map_<surfaceparm> */
#if 0
				else
				{
					Sys_FPrintf( SYS_VRB, "Attempting to match %s with a known surfaceparm\n", token );
					if ( ApplySurfaceParm( &token[ 6 ], &si->contentFlags, &si->surfaceFlags, &si->compileFlags ) == qfalse ) {
						Sys_FPrintf( SYS_WRN, "WARNING: Unknown q3map_* directive \"%s\"\n", token );
					}
				}
#endif
			}


			/* -----------------------------------------------------------------
			   skip
			   ----------------------------------------------------------------- */

			/* ignore all other tokens on the line */
			while ( TokenAvailable() && GetTokenAppend( shaderText, qfalse ) );
		}
	}
}



/*
   ParseCustomInfoParms() - rr2do2
   loads custom info parms file for mods
 */

static void ParseCustomInfoParms( void ){
	qboolean parsedContent, parsedSurface;


	/* file exists? */
	if ( vfsGetFileCount( "scripts/custinfoparms.txt" ) == 0 ) {
		return;
	}

	/* load it */
	LoadScriptFile( "scripts/custinfoparms.txt", 0 );

	/* clear the array */
	memset( custSurfaceParms, 0, sizeof( custSurfaceParms ) );
	numCustSurfaceParms = 0;
	parsedContent = parsedSurface = qfalse;

	/* parse custom contentflags */
	MatchToken( "{" );
	while ( 1 )
	{
		if ( !GetToken( qtrue ) ) {
			break;
		}

		if ( !strcmp( token, "}" ) ) {
			parsedContent = qtrue;
			break;
		}

		custSurfaceParms[ numCustSurfaceParms ].name = safe_malloc( MAX_OS_PATH );
		strcpy( custSurfaceParms[ numCustSurfaceParms ].name, token );
		GetToken( qfalse );
		sscanf( token, "%x", (unsigned int *) &custSurfaceParms[ numCustSurfaceParms ].contentFlags );
		numCustSurfaceParms++;
	}

	/* any content? */
	if ( !parsedContent ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Couldn't find valid custom contentsflag section\n" );
		return;
	}

	/* parse custom surfaceflags */
	MatchToken( "{" );
	while ( 1 )
	{
		if ( !GetToken( qtrue ) ) {
			break;
		}

		if ( !strcmp( token, "}" ) ) {
			parsedSurface = qtrue;
			break;
		}

		custSurfaceParms[ numCustSurfaceParms ].name = safe_malloc( MAX_OS_PATH );
		strcpy( custSurfaceParms[ numCustSurfaceParms ].name, token );
		GetToken( qfalse );
		sscanf( token, "%x", (unsigned int *) &custSurfaceParms[ numCustSurfaceParms ].surfaceFlags );
		numCustSurfaceParms++;
	}

	/* any content? */
	if ( !parsedContent ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Couldn't find valid custom surfaceflag section\n" );
	}
}



/*
   LoadShaderInfo()
   the shaders are parsed out of shaderlist.txt from a main directory
   that is, if using -fs_game we ignore the shader scripts that might be in baseq3/
   on linux there's an additional twist, we actually merge the stuff from ~/.q3a/ and from the base dir
 */

#define MAX_SHADER_FILES 1024

void LoadShaderInfo( void ){
	int i, j, numShaderFiles, count;
	char filename[ 1024 ];
	char            *shaderFiles[ MAX_SHADER_FILES ];

	/* rr2do2: parse custom infoparms first */
	if ( useCustomInfoParms ) {
		ParseCustomInfoParms();
	}

	/* start with zero */
	numShaderFiles = 0;

	/* we can pile up several shader files, the one in baseq3 and ones in the mod dir or other spots */
	sprintf( filename, "%s/shaderlist.txt", game->shaderPath );
	count = vfsGetFileCount( filename );

	/* load them all */
	for ( i = 0; i < count; i++ )
	{
		/* load shader list */
		sprintf( filename, "%s/shaderlist.txt", game->shaderPath );
		LoadScriptFile( filename, i );

		/* parse it */
		while ( GetToken( qtrue ) )
		{
			/* check for duplicate entries */
			for ( j = 0; j < numShaderFiles; j++ )
				if ( !strcmp( shaderFiles[ j ], token ) ) {
					break;
				}

			/* test limit */
			if ( j >= MAX_SHADER_FILES ) {
				Error( "MAX_SHADER_FILES (%d) reached, trim your shaderlist.txt!", (int) MAX_SHADER_FILES );
			}

			/* new shader file */
			if ( j == numShaderFiles ) {
				shaderFiles[ numShaderFiles ] = safe_malloc( MAX_OS_PATH );
				strcpy( shaderFiles[ numShaderFiles ], token );
				numShaderFiles++;
			}
		}
	}

	/* parse the shader files */
	for ( i = 0; i < numShaderFiles; i++ )
	{
		sprintf( filename, "%s/%s.shader", game->shaderPath, shaderFiles[ i ] );
		ParseShaderFile( filename );
		free( shaderFiles[ i ] );
	}

	/* emit some statistics */
	Sys_FPrintf( SYS_VRB, "%9d shaderInfo\n", numShaderInfo );
}
