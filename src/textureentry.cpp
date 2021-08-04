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

#include "textureentry.h"

#include <gtk/gtk.h>

template<class StringList>
void EntryCompletion<StringList>::connect(ui::Entry entry)
{
	if (!m_store) {
		m_store = ui::ListStore::from(gtk_list_store_new(1, G_TYPE_STRING));

		fill();

		StringList().connect(IdleDraw::QueueDrawCaller(m_idleUpdate));
	}

	auto completion = ui::EntryCompletion::from(gtk_entry_completion_new());
	gtk_entry_set_completion(entry, completion);
	gtk_entry_completion_set_model(completion, m_store);
	gtk_entry_completion_set_text_column(completion, 0);
}

template<class StringList>
void EntryCompletion<StringList>::append(const char *string)
{
	m_store.append(0, string);
}

template<class StringList>
void EntryCompletion<StringList>::fill()
{
	StringList().forEach(AppendCaller(*this));
}

template<class StringList>
void EntryCompletion<StringList>::clear()
{
	m_store.clear();
}

template<class StringList>
void EntryCompletion<StringList>::update()
{
	clear();
	fill();
}

template
class EntryCompletion<TextureNameList>;

template
class EntryCompletion<ShaderList>;
