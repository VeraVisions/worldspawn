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

#include "menu.h"

#include <ctype.h>
#include <gtk/gtk.h>
#include <uilib/uilib.h>
#include <debugging/debugging.h>

#include "generic/callback.h"

#include "accelerator.h"
#include "closure.h"
#include "container.h"
#include "pointer.h"

void menu_add_item(ui::Menu menu, ui::MenuItem item)
{
    menu.add(item);
}

ui::MenuItem menu_separator(ui::Menu menu)
{
    auto menu_item = ui::MenuItem::from(gtk_menu_item_new());
    menu.add(menu_item);
    gtk_widget_set_sensitive(menu_item, FALSE);
    menu_item.show();
    return menu_item;
}

ui::TearoffMenuItem menu_tearoff(ui::Menu menu)
{
    auto menu_item = ui::TearoffMenuItem::from(gtk_tearoff_menu_item_new());
    menu.add(menu_item);
// gtk_widget_set_sensitive(menu_item, FALSE); -- controls whether menu is detachable
    menu_item.show();
    return menu_item;
}

ui::MenuItem new_sub_menu_item_with_mnemonic(const char *mnemonic)
{
    auto item = ui::MenuItem(mnemonic, true);
    item.show();

    auto sub_menu = ui::Menu(ui::New);
    gtk_menu_item_set_submenu(item, sub_menu);

    return item;
}

ui::Menu create_sub_menu_with_mnemonic(ui::MenuShell parent, const char *mnemonic)
{
    auto item = new_sub_menu_item_with_mnemonic(mnemonic);
    parent.add(item);
    return ui::Menu::from(gtk_menu_item_get_submenu(item));
}

ui::Menu create_sub_menu_with_mnemonic(ui::MenuBar bar, const char *mnemonic)
{
    return create_sub_menu_with_mnemonic(ui::MenuShell::from(bar._handle), mnemonic);
}

ui::Menu create_sub_menu_with_mnemonic(ui::Menu parent, const char *mnemonic)
{
    return create_sub_menu_with_mnemonic(ui::MenuShell::from(parent._handle), mnemonic);
}

void activate_closure_callback(ui::Widget widget, gpointer data)
{
    (*reinterpret_cast<Callback<void()> *>( data ))();
}

guint menu_item_connect_callback(ui::MenuItem item, const Callback<void()> &callback)
{
#if 1
    return g_signal_connect_swapped(G_OBJECT(item), "activate", G_CALLBACK(callback.getThunk()),
                                    callback.getEnvironment());
#else
    return g_signal_connect_closure( G_OBJECT( item ), "activate", create_cclosure( G_CALLBACK( activate_closure_callback ), callback ), FALSE );
#endif
}

guint check_menu_item_connect_callback(ui::CheckMenuItem item, const Callback<void()> &callback)
{
#if 1
    guint handler = g_signal_connect_swapped(G_OBJECT(item), "toggled", G_CALLBACK(callback.getThunk()),
                                             callback.getEnvironment());
#else
    guint handler = g_signal_connect_closure( G_OBJECT( item ), "toggled", create_cclosure( G_CALLBACK( activate_closure_callback ), callback ), TRUE );
#endif
    g_object_set_data(G_OBJECT(item), "handler", gint_to_pointer(handler));
    return handler;
}

ui::MenuItem new_menu_item_with_mnemonic(const char *mnemonic, const Callback<void()> &callback)
{
    auto item = ui::MenuItem(mnemonic, true);
    item.show();
    menu_item_connect_callback(item, callback);
    return item;
}

ui::MenuItem create_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Callback<void()> &callback)
{
    auto item = new_menu_item_with_mnemonic(mnemonic, callback);
    menu.add(item);
    return item;
}

ui::CheckMenuItem new_check_menu_item_with_mnemonic(const char *mnemonic, const Callback<void()> &callback)
{
    auto item = ui::CheckMenuItem::from(gtk_check_menu_item_new_with_mnemonic(mnemonic));
    item.show();
    check_menu_item_connect_callback(item, callback);
    return item;
}

ui::CheckMenuItem
create_check_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Callback<void()> &callback)
{
    auto item = new_check_menu_item_with_mnemonic(mnemonic, callback);
    menu.add(item);
    return item;
}

ui::RadioMenuItem
new_radio_menu_item_with_mnemonic(GSList **group, const char *mnemonic, const Callback<void()> &callback)
{
    auto item = ui::RadioMenuItem::from(gtk_radio_menu_item_new_with_mnemonic(*group, mnemonic));
    if (*group == 0) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
    }
    *group = gtk_radio_menu_item_get_group(item);
    item.show();
    check_menu_item_connect_callback(item, callback);
    return item;
}

ui::RadioMenuItem create_radio_menu_item_with_mnemonic(ui::Menu menu, GSList **group, const char *mnemonic,
                                                       const Callback<void()> &callback)
{
    auto item = new_radio_menu_item_with_mnemonic(group, mnemonic, callback);
    menu.add(item);
    return item;
}

void check_menu_item_set_active_no_signal(ui::CheckMenuItem item, gboolean active)
{
    guint handler_id = gpointer_to_int(g_object_get_data(G_OBJECT(item), "handler"));
    g_signal_handler_block(G_OBJECT(item), handler_id);
    gtk_check_menu_item_set_active(item, active);
    g_signal_handler_unblock(G_OBJECT(item), handler_id);
}


void radio_menu_item_set_active_no_signal(ui::RadioMenuItem item, gboolean active)
{
    {
        for (GSList *l = gtk_radio_menu_item_get_group(item); l != 0; l = g_slist_next(l)) {
            g_signal_handler_block(G_OBJECT(l->data), gpointer_to_int(g_object_get_data(G_OBJECT(l->data), "handler")));
        }
    }
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), active);
    {
        for (GSList *l = gtk_radio_menu_item_get_group(item); l != 0; l = g_slist_next(l)) {
            g_signal_handler_unblock(G_OBJECT(l->data),
                                     gpointer_to_int(g_object_get_data(G_OBJECT(l->data), "handler")));
        }
    }
}


void menu_item_set_accelerator(ui::MenuItem item, GClosure *closure)
{
    GtkAccelLabel *accel_label = GTK_ACCEL_LABEL(gtk_bin_get_child(GTK_BIN(item)));
    gtk_accel_label_set_accel_closure(accel_label, closure);
}

void accelerator_name(const Accelerator &accelerator, GString *gstring)
{
    gboolean had_mod = FALSE;
    if (accelerator.modifiers & GDK_SHIFT_MASK) {
        g_string_append(gstring, "Shift");
        had_mod = TRUE;
    }
    if (accelerator.modifiers & GDK_CONTROL_MASK) {
        if (had_mod) {
            g_string_append(gstring, "+");
        }
        g_string_append(gstring, "Ctrl");
        had_mod = TRUE;
    }
    if (accelerator.modifiers & GDK_MOD1_MASK) {
        if (had_mod) {
            g_string_append(gstring, "+");
        }
        g_string_append(gstring, "Alt");
        had_mod = TRUE;
    }

    if (had_mod) {
        g_string_append(gstring, "+");
    }
    if (accelerator.key < 0x80 || (accelerator.key > 0x80 && accelerator.key <= 0xff)) {
        switch (accelerator.key) {
            case ' ':
                g_string_append(gstring, "Space");
                break;
            case '\\':
                g_string_append(gstring, "Backslash");
                break;
            default:
                g_string_append_c(gstring, gchar(toupper(accelerator.key)));
                break;
        }
    } else {
        gchar *tmp;

        tmp = gtk_accelerator_name(accelerator.key, (GdkModifierType) 0);
        if (tmp[0] != 0 && tmp[1] == 0) {
            tmp[0] = gchar(toupper(tmp[0]));
        }
        g_string_append(gstring, tmp);
        g_free(tmp);
    }
}

void menu_item_add_accelerator(ui::MenuItem item, Accelerator accelerator)
{
    if (accelerator.key != 0 && gtk_accelerator_valid(accelerator.key, accelerator.modifiers)) {
        GClosure *closure = global_accel_group_find(accelerator);
        ASSERT_NOTNULL(closure);
        menu_item_set_accelerator(item, closure);
    }
}

ui::MenuItem create_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Command &command)
{
    auto item = create_menu_item_with_mnemonic(menu, mnemonic, command.m_callback);
    menu_item_add_accelerator(item, command.m_accelerator);
    return item;
}

void check_menu_item_set_active_callback(void *it, bool enabled)
{
    auto item = ui::CheckMenuItem::from(it);
    check_menu_item_set_active_no_signal(item, enabled);
}

ui::CheckMenuItem create_check_menu_item_with_mnemonic(ui::Menu menu, const char *mnemonic, const Toggle &toggle)
{
    auto item = create_check_menu_item_with_mnemonic(menu, mnemonic, toggle.m_command.m_callback);
    menu_item_add_accelerator(item, toggle.m_command.m_accelerator);
    toggle.m_exportCallback(PointerCaller<void, void(bool), check_menu_item_set_active_callback>(item._handle));
    return item;
}
