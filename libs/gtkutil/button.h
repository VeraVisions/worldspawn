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

#if !defined( INCLUDED_GTKUTIL_BUTTON_H )
#define INCLUDED_GTKUTIL_BUTTON_H

#include <uilib/uilib.h>
#include "generic/callback.h"

typedef int gint;
typedef gint gboolean;
typedef unsigned int guint;

void button_connect_callback(ui::Button button, const Callback<void()> &callback);

void button_connect_callback(ui::ToolButton button, const Callback<void()> &callback);

guint toggle_button_connect_callback(ui::ToggleButton button, const Callback<void()> &callback);

guint toggle_button_connect_callback(ui::ToggleToolButton button, const Callback<void()> &callback);

void button_set_icon(ui::Button button, const char *icon);

void toggle_button_set_active_no_signal(ui::ToggleButton item, gboolean active);

void toggle_button_set_active_no_signal(ui::ToggleToolButton item, gboolean active);

void radio_button_set_active(ui::RadioButton radio, int index);

void radio_button_set_active_no_signal(ui::RadioButton radio, int index);

int radio_button_get_active(ui::RadioButton radio);

#endif
