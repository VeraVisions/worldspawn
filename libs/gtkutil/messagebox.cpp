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

#include "messagebox.h"

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "dialog.h"
#include "widget.h"

ui::Widget create_padding(int width, int height)
{
	ui::Alignment widget = ui::Alignment(0.0, 0.0, 0.0, 0.0);
	widget.show();
	widget.dimensions(width, height);
	return widget;
}

const char *messagebox_stock_icon(EMessageBoxIcon type)
{
	switch (type) {
	default:
	case eMB_ICONDEFAULT:
		return GTK_STOCK_DIALOG_INFO;
	case eMB_ICONERROR:
		return GTK_STOCK_DIALOG_ERROR;
	case eMB_ICONWARNING:
		return GTK_STOCK_DIALOG_WARNING;
	case eMB_ICONQUESTION:
		return GTK_STOCK_DIALOG_QUESTION;
	case eMB_ICONASTERISK:
		return GTK_STOCK_DIALOG_INFO;
	}
}

EMessageBoxReturn
gtk_MessageBox(ui::Window parentWindow, const char *text, const char *title, EMessageBoxType type, EMessageBoxIcon icon)
{
	ModalDialog dialog;
	ModalDialogButton ok_button(dialog, eIDOK);
	ModalDialogButton cancel_button(dialog, eIDCANCEL);
	ModalDialogButton yes_button(dialog, eIDYES);
	ModalDialogButton no_button(dialog, eIDNO);

	ui::Window window = create_fixedsize_modal_dialog_window(parentWindow, title, dialog, 400, 100);

	if (parentWindow) {
		//window.connect( "delete_event", G_CALLBACK(floating_window_delete_present), parent);
		gtk_window_deiconify(parentWindow);
	}

	auto accel = ui::AccelGroup(ui::New);
	window.add_accel_group(accel);

	auto vbox = create_dialog_vbox(8, 8);
	window.add(vbox);


	auto hboxDummy = create_dialog_hbox(0, 0);
	vbox.pack_start(hboxDummy, FALSE, FALSE, 0);

	hboxDummy.pack_start(create_padding(0, 50), FALSE, FALSE, 0); // HACK to force minimum height

	auto iconBox = create_dialog_hbox(16, 0);
	hboxDummy.pack_start(iconBox, FALSE, FALSE, 0);

	auto image = ui::Image::from(gtk_image_new_from_stock(messagebox_stock_icon(icon), GTK_ICON_SIZE_DIALOG));
	image.show();
	iconBox.pack_start(image, FALSE, FALSE, 0);

	auto label = ui::Label(text);
	label.show();
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_label_set_justify(label, GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap(label, TRUE);
	iconBox.pack_start(label, TRUE, TRUE, 0);


	auto vboxDummy = create_dialog_vbox(0, 0);
	vbox.pack_start(vboxDummy, FALSE, FALSE, 0);

	auto alignment = ui::Alignment(0.5, 0.0, 0.0, 0.0);
	alignment.show();
	vboxDummy.pack_start(alignment, FALSE, FALSE, 0);

	auto hbox = create_dialog_hbox(8, 0);
	alignment.add(hbox);

	vboxDummy.pack_start(create_padding(400, 0), FALSE, FALSE, 0); // HACK to force minimum width


	if (type == eMB_OK) {
		auto button = create_modal_dialog_button("OK", ok_button);
		hbox.pack_start(button, TRUE, FALSE, 0);
		gtk_widget_add_accelerator(button, "clicked", accel, GDK_KEY_Escape, (GdkModifierType) 0, (GtkAccelFlags) 0);
		gtk_widget_add_accelerator(button, "clicked", accel, GDK_KEY_Return, (GdkModifierType) 0, (GtkAccelFlags) 0);
		widget_make_default(button);
		button.show();

		dialog.ret = eIDOK;
	} else if (type == eMB_OKCANCEL) {
		{
			auto button = create_modal_dialog_button("OK", ok_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			gtk_widget_add_accelerator(button, "clicked", accel, GDK_KEY_Return, (GdkModifierType) 0,
			                           (GtkAccelFlags) 0);
			widget_make_default(button);
			button.show();
		}

		{
			auto button = create_modal_dialog_button("OK", cancel_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			gtk_widget_add_accelerator(button, "clicked", accel, GDK_KEY_Escape, (GdkModifierType) 0,
			                           (GtkAccelFlags) 0);
			button.show();
		}

		dialog.ret = eIDCANCEL;
	} else if (type == eMB_YESNOCANCEL) {
		{
			auto button = create_modal_dialog_button("Yes", yes_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			widget_make_default(button);
			button.show();
		}

		{
			auto button = create_modal_dialog_button("No", no_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			button.show();
		}
		{
			auto button = create_modal_dialog_button("Cancel", cancel_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			button.show();
		}

		dialog.ret = eIDCANCEL;
	} else if (type == eMB_NOYES) {
		{
			auto button = create_modal_dialog_button("No", no_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			widget_make_default(button);
			button.show();
		}
		{
			auto button = create_modal_dialog_button("Yes", yes_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			button.show();
		}

		dialog.ret = eIDNO;
	} else /* if (type == eMB_YESNO) */
	{
		{
			auto button = create_modal_dialog_button("Yes", yes_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			widget_make_default(button);
			button.show();
		}

		{
			auto button = create_modal_dialog_button("No", no_button);
			hbox.pack_start(button, TRUE, FALSE, 0);
			button.show();
		}
		dialog.ret = eIDNO;
	}

	modal_dialog_show(window, dialog);

	window.destroy();

	return dialog.ret;
}
