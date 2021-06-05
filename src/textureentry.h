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

#if !defined( INCLUDED_TEXTUREENTRY_H )
#define INCLUDED_TEXTUREENTRY_H

#include "gtkutil/idledraw.h"

#include "generic/static.h"
#include "signal/isignal.h"
#include "shaderlib.h"

#include "texwindow.h"

template<typename StringList>
class EntryCompletion {
    ui::ListStore m_store;
    IdleDraw m_idleUpdate;
public:
    EntryCompletion() : m_store(ui::null), m_idleUpdate(UpdateCaller(*this))
    {
    }

    void connect(ui::Entry entry);

    void append(const char *string);

    using AppendCaller = MemberCaller<EntryCompletion, void(const char *), &EntryCompletion::append>;

    void fill();

    void clear();

    void update();

    using UpdateCaller = MemberCaller<EntryCompletion, void(), &EntryCompletion::update>;
};

class TextureNameList {
public:
    void forEach(const ShaderNameCallback &callback) const
    {
        for (QERApp_ActiveShaders_IteratorBegin(); !QERApp_ActiveShaders_IteratorAtEnd(); QERApp_ActiveShaders_IteratorIncrement()) {
            IShader *shader = QERApp_ActiveShaders_IteratorCurrent();

            if (shader_equal_prefix(shader->getName(), "textures/")) {
                callback(shader->getName() + 9);
            }
        }
    }

    void connect(const SignalHandler &update) const
    {
        TextureBrowser_addActiveShadersChangedCallback(update);
    }
};

typedef Static<EntryCompletion<TextureNameList> > GlobalTextureEntryCompletion;


class ShaderList {
public:
    void forEach(const ShaderNameCallback &callback) const
    {
        GlobalShaderSystem().foreachShaderName(callback);
    }

    void connect(const SignalHandler &update) const
    {
        TextureBrowser_addShadersRealiseCallback(update);
    }
};

typedef Static<EntryCompletion<ShaderList> > GlobalShaderEntryCompletion;


#endif
