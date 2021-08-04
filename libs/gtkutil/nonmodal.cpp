#include "nonmodal.h"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

gboolean escape_clear_focus_widget(ui::Widget widget, GdkEventKey *event, gpointer data)
{
	if (event->keyval == GDK_KEY_Escape) {
		gtk_window_set_focus(widget.window(), NULL);
		return TRUE;
	}
	return FALSE;
}

void widget_connect_escape_clear_focus_widget(ui::Widget widget)
{
	widget.connect("key_press_event", G_CALLBACK(escape_clear_focus_widget), 0);
}

gboolean NonModalEntry::focus_in(ui::Entry entry, GdkEventFocus *event, NonModalEntry *self)
{
	self->m_editing = false;
	return FALSE;
}

gboolean NonModalEntry::focus_out(ui::Entry entry, GdkEventFocus *event, NonModalEntry *self)
{
	if (self->m_editing && gtk_widget_get_visible(entry)) {
		self->m_apply();
	}
	self->m_editing = false;
	return FALSE;
}

gboolean NonModalEntry::changed(ui::Entry entry, NonModalEntry *self)
{
	self->m_editing = true;
	return FALSE;
}

gboolean NonModalEntry::enter(ui::Entry entry, GdkEventKey *event, NonModalEntry *self)
{
	if (event->keyval == GDK_KEY_Return) {
		self->m_apply();
		self->m_editing = false;
		gtk_window_set_focus(entry.window(), NULL);
		return TRUE;
	}
	return FALSE;
}

gboolean NonModalEntry::escape(ui::Entry entry, GdkEventKey *event, NonModalEntry *self)
{
	if (event->keyval == GDK_KEY_Escape) {
		self->m_cancel();
		self->m_editing = false;
		gtk_window_set_focus(entry.window(), NULL);
		return TRUE;
	}
	return FALSE;
}

void NonModalEntry::connect(ui::Entry entry)
{
	entry.connect("focus_in_event", G_CALLBACK(focus_in), this);
	entry.connect("focus_out_event", G_CALLBACK(focus_out), this);
	entry.connect("key_press_event", G_CALLBACK(enter), this);
	entry.connect("key_press_event", G_CALLBACK(escape), this);
	entry.connect("changed", G_CALLBACK(changed), this);
}

gboolean NonModalSpinner::changed(ui::SpinButton spin, NonModalSpinner *self)
{
	self->m_apply();
	return FALSE;
}

gboolean NonModalSpinner::enter(ui::SpinButton spin, GdkEventKey *event, NonModalSpinner *self)
{
	if (event->keyval == GDK_KEY_Return) {
		gtk_window_set_focus(spin.window(), NULL);
		return TRUE;
	}
	return FALSE;
}

gboolean NonModalSpinner::escape(ui::SpinButton spin, GdkEventKey *event, NonModalSpinner *self)
{
	if (event->keyval == GDK_KEY_Escape) {
		self->m_cancel();
		gtk_window_set_focus(spin.window(), NULL);
		return TRUE;
	}
	return FALSE;
}

void NonModalSpinner::connect(ui::SpinButton spin)
{
	auto adj = ui::Adjustment::from(gtk_spin_button_get_adjustment(spin));
	guint handler = adj.connect("value_changed", G_CALLBACK(changed), this);
	g_object_set_data(G_OBJECT(spin), "handler", gint_to_pointer(handler));
	spin.connect("key_press_event", G_CALLBACK(enter), this);
	spin.connect("key_press_event", G_CALLBACK(escape), this);
}

void NonModalRadio::connect(ui::RadioButton radio)
{
	GSList *group = gtk_radio_button_get_group(radio);
	for (; group != 0; group = g_slist_next(group)) {
		toggle_button_connect_callback(ui::ToggleButton::from(group->data), m_changed);
	}
}
