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

#if !defined( INCLUDED_GTKUTIL_NONMODAL_H )
#define INCLUDED_GTKUTIL_NONMODAL_H

#include <gdk/gdk.h>
#include "generic/callback.h"

#include "pointer.h"
#include "button.h"

gboolean escape_clear_focus_widget(ui::Widget widget, GdkEventKey *event, gpointer data);

void widget_connect_escape_clear_focus_widget(ui::Widget widget);

class NonModalEntry {
bool m_editing;
Callback<void()> m_apply;
Callback<void()> m_cancel;

static gboolean focus_in(ui::Entry entry, GdkEventFocus *event, NonModalEntry *self);

static gboolean focus_out(ui::Entry entry, GdkEventFocus *event, NonModalEntry *self);

static gboolean changed(ui::Entry entry, NonModalEntry *self);

static gboolean enter(ui::Entry entry, GdkEventKey *event, NonModalEntry *self);

static gboolean escape(ui::Entry entry, GdkEventKey *event, NonModalEntry *self);

public:
NonModalEntry(const Callback<void()> &apply, const Callback<void()> &cancel) : m_editing(false), m_apply(apply),
	m_cancel(cancel)
{
}

void connect(ui::Entry entry);
};


class NonModalSpinner {
Callback<void()> m_apply;
Callback<void()> m_cancel;

static gboolean changed(ui::SpinButton spin, NonModalSpinner *self);

static gboolean enter(ui::SpinButton spin, GdkEventKey *event, NonModalSpinner *self);

static gboolean escape(ui::SpinButton spin, GdkEventKey *event, NonModalSpinner *self);

public:
NonModalSpinner(const Callback<void()> &apply, const Callback<void()> &cancel) : m_apply(apply), m_cancel(cancel)
{
}

void connect(ui::SpinButton spin);
};


class NonModalRadio {
Callback<void()> m_changed;

public:
NonModalRadio(const Callback<void()> &changed) : m_changed(changed)
{
}

void connect(ui::RadioButton radio);
};


#endif
