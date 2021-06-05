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

#include "brushmodule.h"

#include "qerplugin.h"

#include "brushnode.h"
#include "brushmanip.h"

#include "preferencesystem.h"
#include "stringio.h"

#include "map.h"
#include "qe3.h"
#include "mainframe.h"
#include "preferences.h"

LatchedValue<bool> g_useAlternativeTextureProjection(false, "Use alternative texture-projection (\"brush primitives\")");
static int g_numbrushtypes = 0;
static int g_selectedbrushtype;
static EBrushType g_brushTypes[8], g_brushType;
//bool g_showAlternativeTextureProjectionOption = false;
bool g_brush_always_caulk;

bool getTextureLockEnabled()
{
    return g_brush_texturelock_enabled;
}

struct Face_SnapPlanes {
    static void Export(const QuantiseFunc &self, const Callback<void(bool)> &returnz)
    {
        returnz(self == quantiseInteger);
    }

    static void Import(QuantiseFunc &self, bool value)
    {
        self = value ? quantiseInteger : quantiseFloating;
    }
};

const char* BrushType_getName( EBrushType type ){
	switch ( type )
	{
	case eBrushTypeQuake:
	case eBrushTypeQuake2:
	case eBrushTypeQuake3:
		return "Axial Projection";
	case eBrushTypeQuake3BP:
		return "Brush Primitives";
	case eBrushTypeQuake3Valve:
	case eBrushTypeHalfLife:
		return "Valve 220";
	case eBrushTypeDoom3:
		return "Doom3";
	case eBrushTypeQuake4:
		return "Quake4";
	default:
		return "unknown";
	}
}

void Brush_constructPreferences(PreferencesPage &page)
{
    page.appendCheckBox(
            "", "Snap planes to integer grid",
            make_property<Face_SnapPlanes>(Face::m_quantise)
    );
    page.appendEntry(
            "Default texture scale",
            g_texdef_default_scale
    );
    if (g_numbrushtypes > 1) {
		const char *names[8];
		for(int i = 0; i < g_numbrushtypes; i++)
			names[i] = BrushType_getName(g_brushTypes[i]);
		page.appendCombo("Default Brush Type", g_selectedbrushtype, StringArrayRange(names, names+g_numbrushtypes));
    }
    // d1223m
    page.appendCheckBox("",
                        "Always use caulk for new brushes",
                        g_brush_always_caulk
    );
}

void Brush_constructPage(PreferenceGroup &group)
{
    PreferencesPage page(group.createPage("Brush", "Brush Settings"));
    Brush_constructPreferences(page);
}

void Brush_registerPreferencesPage()
{
    PreferencesDialog_addSettingsPage(makeCallbackF(Brush_constructPage));
}

void Brush_unlatchPreferences()
{
    Brush_toggleFormat(0);
}

void Brush_setFormat(EBrushType t)
{
    if (g_numbrushtypes) {
//        g_useAlternativeTextureProjection.m_value = g_useAlternativeTextureProjection.m_latched ^ i;
        Brush::destroyStatic();
		g_brushType = t;
        Brush::constructStatic(t);
    }
}
void Brush_toggleFormat(int i)
{
    if (g_numbrushtypes) {
//        g_useAlternativeTextureProjection.m_value = g_useAlternativeTextureProjection.m_latched ^ i;
        Brush::destroyStatic();
		g_brushType = g_brushTypes[i];
        Brush::constructStatic(g_brushType);
    }
}

int Brush_toggleFormatCount()
{
    if (g_numbrushtypes) {
        return g_numbrushtypes;
    }
    return 1;
}

static void Brush_Construct(void)
{
	EBrushType type = g_brushType = g_brushTypes[g_selectedbrushtype];

	// d1223m
	GlobalPreferenceSystem().registerPreference(
			"BrushAlwaysCaulk",
			make_property_string(g_brush_always_caulk)
	);

    Brush_registerCommands();
    Brush_registerPreferencesPage();

    BrushFilters_construct();

    BrushClipPlane::constructStatic();
    BrushInstance::constructStatic();
    Brush::constructStatic(type);

    Brush::m_maxWorldCoord = g_MaxWorldCoord;
    BrushInstance::m_counter = &g_brushCount;

    g_texdef_default_scale = 0.5f;
    const char *value = g_pGameDescription->getKeyValue("default_scale");
    if (!string_empty(value)) {
        float scale = static_cast<float>( atof(value));
        if (scale != 0) {
            g_texdef_default_scale = scale;
        } else {
            globalErrorStream() << "error parsing \"default_scale\" attribute\n";
        }
    }

    GlobalPreferenceSystem().registerPreference("TextureLock", make_property_string(g_brush_texturelock_enabled));
    GlobalPreferenceSystem().registerPreference("BrushSnapPlanes",
                                                make_property_string<Face_SnapPlanes>(Face::m_quantise));
    GlobalPreferenceSystem().registerPreference("TexdefDefaultScale", make_property_string(g_texdef_default_scale));

    GridStatus_getTextureLockEnabled = getTextureLockEnabled;
    g_texture_lock_status_changed = makeCallbackF(GridStatus_onTextureLockEnabledChanged);
}
static void Brush_Construct(EBrushType type0, EBrushType type1, EBrushType type2)
{
	g_numbrushtypes = 0;
	g_brushTypes[g_numbrushtypes++] = type0;
	if (type1 != type0)
		g_brushTypes[g_numbrushtypes++] = type1;
	if (type2 != type0 && type2 != type1)
		g_brushTypes[g_numbrushtypes++] = type2;

	if (g_numbrushtypes) {
        const char *value = g_pGameDescription->getKeyValue("brush_primit");
        if (!string_empty(value)) {
            g_selectedbrushtype = atoi(value);
			if (g_selectedbrushtype < 0 || g_selectedbrushtype >= g_numbrushtypes)
				g_selectedbrushtype = 0;
        }

//		GlobalPreferenceSystem().registerPreference(
//			"BrushType",
//			make_property(g_selectedbrushtype)
//		);
    }

	Brush_Construct();
}
static void Brush_Construct(EBrushType type0, EBrushType type1)
{
	Brush_Construct(type0, type1, type0);
}
static void Brush_Construct(EBrushType type0)
{
	Brush_Construct(type0, type0, type0);
}

void Brush_Destroy()
{
    Brush::m_maxWorldCoord = 0;
    BrushInstance::m_counter = 0;

    Brush::destroyStatic();
    BrushInstance::destroyStatic();
    BrushClipPlane::destroyStatic();
}

void Brush_clipperColourChanged()
{
    BrushClipPlane::destroyStatic();
    BrushClipPlane::constructStatic();
}

void BrushFaceData_fromFace(const BrushFaceDataCallback &callback, Face &face)
{
    _QERFaceData faceData;
    faceData.m_p0 = face.getPlane().planePoints()[0];
    faceData.m_p1 = face.getPlane().planePoints()[1];
    faceData.m_p2 = face.getPlane().planePoints()[2];
    faceData.m_shader = face.GetShader();
    faceData.m_texdef = face.getTexdef().m_projection.m_texdef;
    faceData.contents = face.getShader().m_flags.m_contentFlags;
    faceData.flags = face.getShader().m_flags.m_surfaceFlags;
    faceData.value = face.getShader().m_flags.m_value;
    callback(faceData);
}

typedef ConstReferenceCaller<BrushFaceDataCallback, void(Face &), BrushFaceData_fromFace> BrushFaceDataFromFaceCaller;
typedef Callback<void(Face &)> FaceCallback;

class Quake3BrushCreator : public BrushCreator {
public:
    scene::Node &createBrush()
    {
        return (new BrushNode)->node();
    }

	virtual EBrushType getBrushType()
	{
		return g_brushType;
	}
	virtual void setBrushType(EBrushType t)
	{
		if (g_brushType != t)
			Brush_setFormat(t);
	}

    void Brush_forEachFace(scene::Node &brush, const BrushFaceDataCallback &callback)
    {
        ::Brush_forEachFace(*Node_getBrush(brush), FaceCallback(BrushFaceDataFromFaceCaller(callback)));
    }

    bool Brush_addFace(scene::Node &brush, const _QERFaceData &faceData)
    {
        Node_getBrush(brush)->undoSave();
        return Node_getBrush(brush)->addPlane(faceData.m_p0, faceData.m_p1, faceData.m_p2, faceData.m_shader,
                                              TextureProjection(faceData.m_texdef, brushprimit_texdef_t(),
                                                                Vector3(0, 0, 0), Vector3(0, 0, 0))) != 0;
    }
};

Quake3BrushCreator g_Quake3BrushCreator;

BrushCreator &GetBrushCreator()
{
    return g_Quake3BrushCreator;
}

#include "modulesystem/singletonmodule.h"
#include "modulesystem/moduleregistry.h"


class BrushDependencies :
        public GlobalRadiantModuleRef,
        public GlobalSceneGraphModuleRef,
        public GlobalShaderCacheModuleRef,
        public GlobalSelectionModuleRef,
        public GlobalOpenGLModuleRef,
        public GlobalUndoModuleRef,
        public GlobalFilterModuleRef {
};

class BrushDoom3API : public TypeSystemRef {
    BrushCreator *m_brushdoom3;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "doom3");

    BrushDoom3API()
    {
        Brush_Construct(eBrushTypeDoom3);

        m_brushdoom3 = &GetBrushCreator();
    }

    ~BrushDoom3API()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushdoom3;
    }
};

typedef SingletonModule<BrushDoom3API, BrushDependencies> BrushDoom3Module;
typedef Static<BrushDoom3Module> StaticBrushDoom3Module;
StaticRegisterModule staticRegisterBrushDoom3(StaticBrushDoom3Module::instance());


class BrushQuake4API : public TypeSystemRef {
    BrushCreator *m_brushquake4;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "quake4");

    BrushQuake4API()
    {
        Brush_Construct(eBrushTypeQuake4);

        m_brushquake4 = &GetBrushCreator();
    }

    ~BrushQuake4API()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushquake4;
    }
};

typedef SingletonModule<BrushQuake4API, BrushDependencies> BrushQuake4Module;
typedef Static<BrushQuake4Module> StaticBrushQuake4Module;
StaticRegisterModule staticRegisterBrushQuake4(StaticBrushQuake4Module::instance());


class BrushQuake3API : public TypeSystemRef {
    BrushCreator *m_brushquake3;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "quake3");

    BrushQuake3API()
    {
		Brush_Construct(eBrushTypeQuake3, eBrushTypeQuake3BP, eBrushTypeQuake3Valve);

        m_brushquake3 = &GetBrushCreator();
    }

    ~BrushQuake3API()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushquake3;
    }
};

typedef SingletonModule<BrushQuake3API, BrushDependencies> BrushQuake3Module;
typedef Static<BrushQuake3Module> StaticBrushQuake3Module;
StaticRegisterModule staticRegisterBrushQuake3(StaticBrushQuake3Module::instance());


class BrushQuake2API : public TypeSystemRef {
    BrushCreator *m_brushquake2;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "quake2");

    BrushQuake2API()
    {
        Brush_Construct(eBrushTypeQuake2);

        m_brushquake2 = &GetBrushCreator();
    }

    ~BrushQuake2API()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushquake2;
    }
};

typedef SingletonModule<BrushQuake2API, BrushDependencies> BrushQuake2Module;
typedef Static<BrushQuake2Module> StaticBrushQuake2Module;
StaticRegisterModule staticRegisterBrushQuake2(StaticBrushQuake2Module::instance());


class BrushQuake1API : public TypeSystemRef {
    BrushCreator *m_brushquake1;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "quake");

    BrushQuake1API()
    {
        Brush_Construct(eBrushTypeQuake, eBrushTypeHalfLife);

        m_brushquake1 = &GetBrushCreator();
    }

    ~BrushQuake1API()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushquake1;
    }
};

typedef SingletonModule<BrushQuake1API, BrushDependencies> BrushQuake1Module;
typedef Static<BrushQuake1Module> StaticBrushQuake1Module;
StaticRegisterModule staticRegisterBrushQuake1(StaticBrushQuake1Module::instance());


class BrushHalfLifeAPI : public TypeSystemRef {
    BrushCreator *m_brushhalflife;
public:
    typedef BrushCreator Type;

    STRING_CONSTANT(Name, "halflife");

    BrushHalfLifeAPI()
    {
        Brush_Construct(eBrushTypeHalfLife);

        m_brushhalflife = &GetBrushCreator();
    }

    ~BrushHalfLifeAPI()
    {
        Brush_Destroy();
    }

    BrushCreator *getTable()
    {
        return m_brushhalflife;
    }
};

typedef SingletonModule<BrushHalfLifeAPI, BrushDependencies> BrushHalfLifeModule;
typedef Static<BrushHalfLifeModule> StaticBrushHalfLifeModule;
StaticRegisterModule staticRegisterBrushHalfLife(StaticBrushHalfLifeModule::instance());
