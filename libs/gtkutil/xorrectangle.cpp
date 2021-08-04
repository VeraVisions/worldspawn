#include "xorrectangle.h"

#include <gtk/gtk.h>

bool XORRectangle::initialised() const
{
	return !!cr;
}

void XORRectangle::lazy_init()
{
	if (!initialised()) {
		cr = gdk_cairo_create(gtk_widget_get_window(m_widget));
	}
}

void XORRectangle::draw() const
{
	const int x = float_to_integer(m_rectangle.x);
	const int y = float_to_integer(m_rectangle.y);
	const int w = float_to_integer(m_rectangle.w);
	const int h = float_to_integer(m_rectangle.h);
	GtkAllocation allocation;
	gtk_widget_get_allocation(m_widget, &allocation);
	cairo_rectangle(cr, x, -(h) - (y - allocation.height), w, h);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_set_operator(cr, CAIRO_OPERATOR_DIFFERENCE);
	cairo_stroke(cr);
}

XORRectangle::XORRectangle(ui::GLArea widget) : m_widget(widget), cr(0)
{
}

XORRectangle::~XORRectangle()
{
	if (initialised()) {
		cairo_destroy(cr);
	}
}

void XORRectangle::set(rectangle_t rectangle)
{
	if (gtk_widget_get_realized(m_widget)) {
		lazy_init();
		draw();
		m_rectangle = rectangle;
		draw();
	}
}
