#include <gtk/gtk.h>
#include <uilib/uilib.h>

#include "support.h"

ui::Widget
lookup_widget(ui::Widget widget,
              const gchar *widget_name)
{
	ui::Widget parent{ui::null};

	for (;;) {
		if (GTK_IS_MENU(widget)) {
			parent = ui::Widget::from(gtk_menu_get_attach_widget(GTK_MENU(widget)));
		} else {
			parent = ui::Widget::from(gtk_widget_get_parent(widget));
		}
		if (!parent) {
			parent = ui::Widget::from(g_object_get_data(G_OBJECT(widget), "GladeParentKey"));
		}
		if (parent == NULL) {
			break;
		}
		widget = parent;
	}

	auto found_widget = ui::Widget::from(g_object_get_data(G_OBJECT(widget), widget_name));
	if (!found_widget) {
		g_warning("Widget not found: %s", widget_name);
	}
	return found_widget;
}
