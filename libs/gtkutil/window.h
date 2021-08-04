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

#if !defined( INCLUDED_GTKUTIL_WINDOW_H )
#define INCLUDED_GTKUTIL_WINDOW_H

#include <uilib/uilib.h>

#include "debugging/debugging.h"
#include "generic/callback.h"
#include "widget.h"

gboolean window_focus_in_clear_focus_widget(ui::Window widget, GdkEventKey *event, gpointer data);

guint window_connect_focus_in_clear_focus_widget(ui::Window window);

unsigned int connect_floating(ui::Window main_window, ui::Window floating);

ui::Window create_floating_window(const char *title, ui::Window parent);

void destroy_floating_window(ui::Window window);

ui::Window create_persistent_floating_window(const char *title, ui::Window main_window);

gboolean persistent_floating_window_delete(ui::Window floating, GdkEvent *event, ui::Window main_window);

void window_remove_minmax(ui::Window window);

ui::ScrolledWindow create_scrolled_window(ui::Policy hscrollbar_policy, ui::Policy vscrollbar_policy, int border = 0);


struct WindowPosition {
	int x, y, w, h;

	WindowPosition()
	{
	}

	WindowPosition(int _x, int _y, int _w, int _h)
		: x(_x), y(_y), w(_w), h(_h)
	{
	}
};

const WindowPosition c_default_window_pos(50, 25, 400, 300);

void window_get_position(ui::Window window, WindowPosition &position);

void window_set_position(ui::Window window, const WindowPosition &position);

struct WindowPosition_String {

	static void Export(const WindowPosition &self, const Callback<void(const char *)> &returnz);

	static void Import(WindowPosition &self, const char *value);

};

class WindowPositionTracker {
WindowPosition m_position;

static gboolean configure(ui::Widget widget, GdkEventConfigure *event, WindowPositionTracker *self);

public:
WindowPositionTracker()
	: m_position(c_default_window_pos)
{
}

void sync(ui::Window window);

void connect(ui::Window window);

const WindowPosition &getPosition() const;

//hack
void setPosition(const WindowPosition &position);
};


struct WindowPositionTracker_String {
	static void Export(const WindowPositionTracker &self, const Callback<void(const char *)> &returnz);

	static void Import(WindowPositionTracker &self, const char *value);
};

#endif
