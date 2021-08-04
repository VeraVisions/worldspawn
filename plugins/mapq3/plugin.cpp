/*
   Copyright (C) 2001-2006, William Joseph.
   All Rights Reserved.

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

#include "iscriplib.h"
#include "ibrush.h"
#include "ipatch.h"
#include "ifiletypes.h"
#include "ieclass.h"
#include "qerplugin.h"

#include "scenelib.h"
#include "string/string.h"
#include "stringio.h"
#include "generic/constant.h"

#include "modulesystem/singletonmodule.h"

#include "parse.h"
#include "write.h"

class MapDependencies :
	public GlobalRadiantModuleRef,
	public GlobalBrushModuleRef,
	public GlobalPatchModuleRef,
	public GlobalFiletypesModuleRef,
	public GlobalScripLibModuleRef,
	public GlobalEntityClassManagerModuleRef,
	public GlobalSceneGraphModuleRef {
public:
MapDependencies() :
	GlobalBrushModuleRef(GlobalRadiant().getRequiredGameDescriptionKeyValue("brushtypes")),
	GlobalPatchModuleRef(GlobalRadiant().getRequiredGameDescriptionKeyValue("patchtypes")),
	GlobalEntityClassManagerModuleRef(GlobalRadiant().getRequiredGameDescriptionKeyValue("entityclass"))
{
}
};

class MapQ3API : public TypeSystemRef, public MapFormat, public PrimitiveParser {
mutable bool detectedFormat;
public:
typedef MapFormat Type;

STRING_CONSTANT(Name, "worldspawn");

MapQ3API()
{
	GlobalFiletypesModule::getTable().addType(Type::Name(), Name(),
	                                          filetype_t("WorldSpawn maps", "*.map", true, true, true));
	GlobalFiletypesModule::getTable().addType(Type::Name(), Name(),
	                                          filetype_t("WorldSpawn region", "*.reg", true, true, true));
	GlobalFiletypesModule::getTable().addType(Type::Name(), Name(),
	                                          filetype_t("WorldSpawn compiled maps", "*.bsp", false, true, false));
}

MapFormat *getTable()
{
	return this;
}

scene::Node &parsePrimitive(Tokeniser &tokeniser) const
{
	const char *primitive = tokeniser.getToken();
	if (primitive != 0) {
		if (string_equal(primitive, "patchDef2") || string_equal(primitive, "patchDef2WS")) {
			return GlobalPatchModule::getTable().createPatch(false, false);
		} if (string_equal(primitive, "patchDef3") || string_equal(primitive, "patchDef3WS")) {
			return GlobalPatchModule::getTable().createPatch(true, false);
		} if (string_equal(primitive, "patchDefWS")) {
			return GlobalPatchModule::getTable().createPatch(false, true);
		}

		if (!detectedFormat)
		{
			EBrushType fmt;
			if (string_equal(primitive, "brushDef"))
				fmt = eBrushTypeQuake3BP;
			else if (string_equal(primitive, "("))
			{
				if (1)        //tokeniser.getLine
					fmt = eBrushTypeQuake3Valve;
				else
					fmt = eBrushTypeQuake3;
			}
			if (fmt != GlobalBrushModule::getTable().getBrushType())
				GlobalBrushModule::getTable().setBrushType(fmt);
		}

		switch(GlobalBrushModule::getTable().getBrushType())
		{
		default:
			tokeniser.ungetToken();         // (
		case eBrushTypeQuake3BP:
			return GlobalBrushModule::getTable().createBrush();
		}
	}

	Tokeniser_unexpectedError(tokeniser, primitive, "#quake3-primitive");
	return g_nullNode;
}

void readGraph(scene::Node &root, TextInputStream &inputStream, EntityCreator &entityTable) const
{
	detectedFormat = false;
	wrongFormat = false;
	Tokeniser &tokeniser = GlobalScripLibModule::getTable().m_pfnNewSimpleTokeniser(inputStream);
	Map_Read(root, tokeniser, entityTable, *this);
	tokeniser.release();
}

void writeGraph(scene::Node &root, GraphTraversalFunc traverse, TextOutputStream &outputStream) const
{
	TokenWriter &writer = GlobalScripLibModule::getTable().m_pfnNewSimpleTokenWriter(outputStream);
	Map_Write(root, traverse, writer, false);
	writer.release();
}
};

typedef SingletonModule<MapQ3API, MapDependencies> MapQ3Module;

MapQ3Module g_MapQ3Module;


extern "C" void
#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
Radiant_RegisterModules(ModuleServer &server)
{
	initialiseModule(server);

	g_MapQ3Module.selfRegister();
}
