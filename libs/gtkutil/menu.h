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

#if !defined( INCLUDED_GTKUTIL_MENU_H )
#define INCLUDED_GTKUTIL_MENU_H

#include <uilib/uilib.h>
#include "generic/callback.h"

typedef int gint;
typedef gint gboolean;
typedef struct _GSList GSList;

void menu_add_item(ui::Menu menu, ui::MenuItem item);

ui::MenuItem menu_separator(ui::Menu menu);

ui::TearoffMenuItem menu_tearoff(ui::Menu menu);

ui::MenuItem new_sub_menu_item_with_mnemonic(const char *mnemonic);

ui::Menu create_sub_menu_with_mnemonic(ui::MenuBar bar, const char *mnemonic);

ui::Menu create_sub_menu_with_mnemonic(ui::Menu parent, const char *mnemonic);

ui::MenuItem create_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Callback<void()> &callback);

ui::CheckMenuItem
create_check_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Callback<void()> &callback);

ui::RadioMenuItem create_radio_menu_item_with_mnemonic(ui::Menu menu, GSList **group, const char *mnemonic,
                                                       const Callback<void()> &callback);

class Command;

ui::MenuItem create_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Command &command);

class Toggle;

ui::CheckMenuItem create_check_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Toggle &toggle);


void check_menu_item_set_active_no_signal(ui::CheckMenuItem item, gboolean active);

void radio_menu_item_set_active_no_signal(ui::RadioMenuItem item, gboolean active);

#endif
