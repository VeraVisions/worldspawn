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

#if !defined( INCLUDED_GTKUTIL_DIALOG_H )
#define INCLUDED_GTKUTIL_DIALOG_H

#include "generic/callback.h"
#include "generic/arrayrange.h"
#include "qerplugin.h"

typedef int gint;
typedef gint gboolean;
typedef struct _GdkEventAny GdkEventAny;


struct ModalDialog {
    ModalDialog()
            : loop(true), ret(eIDCANCEL)
    {
    }

    bool loop;
    EMessageBoxReturn ret;
};

struct ModalDialogButton {
    ModalDialogButton(ModalDialog &dialog, EMessageBoxReturn value)
            : m_dialog(dialog), m_value(value)
    {
    }

    ModalDialog &m_dialog;
    EMessageBoxReturn m_value;
};

typedef void ( *GCallback )(void);

typedef void *gpointer;

ui::Window create_fixedsize_modal_window(ui::Window parent, const char *title, int width, int height);

ui::Window create_dialog_window(ui::Window parent, const char *title, GCallback func, gpointer data, int default_w = -1,
                                int default_h = -1);

ui::Table
create_dialog_table(unsigned int rows, unsigned int columns, unsigned int row_spacing, unsigned int col_spacing,
                    int border = 0);

ui::Button create_dialog_button(const char *label, GCallback func, gpointer data);

ui::VBox create_dialog_vbox(int spacing, int border = 0);

ui::HBox create_dialog_hbox(int spacing, int border = 0);

ui::Frame create_dialog_frame(const char *label, ui::Shadow shadow = ui::Shadow::ETCHED_IN);

ui::Button create_modal_dialog_button(const char *label, ModalDialogButton &button);

ui::Window create_modal_dialog_window(ui::Window parent, const char *title, ModalDialog &dialog, int default_w = -1,
                                      int default_h = -1);

ui::Window
create_fixedsize_modal_dialog_window(ui::Window parent, const char *title, ModalDialog &dialog, int width = -1,
                                     int height = -1);

EMessageBoxReturn modal_dialog_show(ui::Window window, ModalDialog &dialog);


gboolean dialog_button_ok(ui::Widget widget, ModalDialog *data);

gboolean dialog_button_cancel(ui::Widget widget, ModalDialog *data);

gboolean dialog_button_yes(ui::Widget widget, ModalDialog *data);

gboolean dialog_button_no(ui::Widget widget, ModalDialog *data);

gboolean dialog_delete_callback(ui::Widget widget, GdkEventAny *event, ModalDialog *data);

ui::Window create_simple_modal_dialog_window(const char *title, ModalDialog &dialog, ui::Widget contents);

class RadioHBox {
public:
    ui::HBox m_hbox;
    ui::RadioButton m_radio;

    RadioHBox(ui::HBox hbox, ui::RadioButton radio) :
            m_hbox(hbox),
            m_radio(radio)
    {
    }
};

RadioHBox RadioHBox_new(StringArrayRange names);


class PathEntry {
public:
    ui::Frame m_frame;
    ui::Entry m_entry;
    ui::Button m_button;

    PathEntry(ui::Frame frame, ui::Entry entry, ui::Button button) :
            m_frame(frame),
            m_entry(entry),
            m_button(button)
    {
    }
};

PathEntry PathEntry_new();

class BrowsedPathEntry {
public:
    typedef Callback<void(const char *)> SetPathCallback;
    typedef Callback<void(const SetPathCallback &)> BrowseCallback;

    PathEntry m_entry;
    BrowseCallback m_browse;

    BrowsedPathEntry(const BrowseCallback &browse);
};

ui::Label DialogLabel_new(const char *name);

ui::Table DialogRow_new(const char *name, ui::Widget widget);

void DialogVBox_packRow(ui::Box vbox, ui::Widget row);


#endif
