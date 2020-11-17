#include "entry.h"

#include <gtk/gtk.h>

void entry_set_string(ui::Entry entry, const char *string)
{
    gtk_entry_set_text(entry, string);
}

void entry_set_int(ui::Entry entry, int i)
{
    char buf[32];
    sprintf(buf, "%d", i);
    entry_set_string(entry, buf);
}

void entry_set_float(ui::Entry entry, float f)
{
    char buf[32];
    sprintf(buf, "%g", f);
    entry_set_string(entry, buf);
}

const char *entry_get_string(ui::Entry entry)
{
    return gtk_entry_get_text(entry);
}

int entry_get_int(ui::Entry entry)
{
    return atoi(entry_get_string(entry));
}

double entry_get_float(ui::Entry entry)
{
    return atof(entry_get_string(entry));
}
