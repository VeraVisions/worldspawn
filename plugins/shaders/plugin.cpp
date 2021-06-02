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

#include "ishaders.h"
#include "ifilesystem.h"
#include "itextures.h"
#include "iscriplib.h"
#include "qerplugin.h"

#include "string/string.h"
#include "modulesystem/singletonmodule.h"
#include "shaders.h"

class ShadersDependencies :
	public GlobalFileSystemModuleRef,
	public GlobalTexturesModuleRef,
	public GlobalScripLibModuleRef,
	public GlobalRadiantModuleRef {
	ImageModuleRef m_bitmapModule;
public:
	ShadersDependencies() :
		m_bitmapModule("tga")
	{
	}

	ImageModuleRef &getBitmapModule()
	{
		return m_bitmapModule;
	}
};

class MaterialAPI {
	ShaderSystem *m_shadersq3;
public:
	typedef ShaderSystem Type;

	STRING_CONSTANT(Name, "mat");

	MaterialAPI(ShadersDependencies &dependencies)
	{
		g_shadersExtension = "mat";
		g_shadersDirectory = "textures/";
		g_enableDefaultShaders = false;
		g_useShaderList = false;
		g_bitmapModule = dependencies.getBitmapModule().getTable();
		Shaders_Construct();
		m_shadersq3 = &GetShaderSystem();
	}

	~MaterialAPI()
	{
		Shaders_Destroy();
	}

	ShaderSystem *getTable()
	{
		return m_shadersq3;
	}
};

typedef SingletonModule<MaterialAPI, ShadersDependencies, DependenciesAPIConstructor<MaterialAPI, ShadersDependencies> > MaterialModule;
MaterialModule g_MaterialModule;

extern "C" void
#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
Radiant_RegisterModules(ModuleServer &server)
{
	initialiseModule(server);
	g_MaterialModule.selfRegister();
}
