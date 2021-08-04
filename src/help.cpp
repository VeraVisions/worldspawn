/*
   Copyright (C) 1999-2006 Id Software, Inc. and contributors.
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

#include "help.h"

#include "debugging/debugging.h"

#include <vector>
#include <list>

#include "libxml/parser.h"
#include "generic/callback.h"
#include "gtkutil/menu.h"
#include "stream/stringstream.h"
#include "os/file.h"

#include "url.h"
#include "preferences.h"
#include "mainframe.h"


/*!
   needed for hooking in Gtk+
 */
void Help_OpenVV(void)
{
	OpenURL("https://www.vera-visions.com/");
}
void Help_OpenMAT(void)
{
	OpenURL("https://www.vera-visions.com/docs/materials/");
}


void create_game_help_menu(ui::Menu menu)
{
	create_menu_item_with_mnemonic(menu, "Vera Visions", makeCallbackF(Help_OpenVV));
	create_menu_item_with_mnemonic(menu, "Material Manual", makeCallbackF(Help_OpenMAT));
}
