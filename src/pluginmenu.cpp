/*
   Copyright (C) 1999-2006 Id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

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

#include "pluginmenu.h"

#include <gtk/gtk.h>

#include "stream/textstream.h"

#include "gtkutil/pointer.h"
#include "gtkutil/menu.h"

#include "pluginmanager.h"
#include "mainframe.h"
#include "preferences.h"


int m_nNextPlugInID = 0;

void plugin_activated(ui::Widget widget, gpointer data)
{
	const char *str = (const char *) g_object_get_data(G_OBJECT(widget), "command");
	GetPlugInMgr().Dispatch(gpointer_to_int(data), str);
}

#include <stack>

void PlugInMenu_Add(ui::Menu plugin_menu, IPlugIn *pPlugIn)
{
	ui::Widget item{ui::null}, parent{ui::null};
	ui::Menu menu{ui::null}, subMenu{ui::null};
	const char *menuText, *menuCommand;
	std::stack<ui::Menu> menuStack;

	parent = ui::MenuItem(pPlugIn->getMenuName());
	parent.show();
	plugin_menu.add(parent);

	std::size_t nCount = pPlugIn->getCommandCount();
	if (nCount > 0) {
		menu = ui::Menu(ui::New);
		/*if (g_Layout_enableOpenStepUX.m_value) {
		    menu_tearoff(menu);
		   }*/
		while (nCount > 0) {
			menuText = pPlugIn->getCommandTitle(--nCount);
			menuCommand = pPlugIn->getCommand(nCount);

			if (menuText != 0 && strlen(menuText) > 0) {
				if (!strcmp(menuText, "-")) {
					item = ui::Widget::from(gtk_menu_item_new());
					gtk_widget_set_sensitive(item, FALSE);
				} else if (!strcmp(menuText, ">")) {
					menuText = pPlugIn->getCommandTitle(--nCount);
					menuCommand = pPlugIn->getCommand(nCount);
					if (!strcmp(menuText, "-") || !strcmp(menuText, ">") || !strcmp(menuText, "<")) {
						globalErrorStream() << pPlugIn->getMenuName() << " Invalid title (" << menuText
						                    << ") for submenu.\n";
						continue;
					}

					item = ui::MenuItem(menuText);
					item.show();
					menu.add(item);

					subMenu = ui::Menu(ui::New);
					gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), subMenu);
					menuStack.push(menu);
					menu = subMenu;
					continue;
				} else if (!strcmp(menuText, "<")) {
					if (!menuStack.empty()) {
						menu = menuStack.top();
						menuStack.pop();
					} else {
						globalErrorStream() << pPlugIn->getMenuName()
						                    << ": Attempt to end non-existent submenu ignored.\n";
					}
					continue;
				} else {
					item = ui::MenuItem(menuText);
					g_object_set_data(G_OBJECT(item), "command",
					                  const_cast<gpointer>( static_cast<const void *>( menuCommand )));
					item.connect("activate", G_CALLBACK(plugin_activated), gint_to_pointer(m_nNextPlugInID));
				}
				item.show();
				menu.add(item);
				pPlugIn->addMenuID(m_nNextPlugInID++);
			}
		}
		if (!menuStack.empty()) {
			std::size_t size = menuStack.size();
			if (size != 0) {
				globalErrorStream() << pPlugIn->getMenuName() << " mismatched > <. " << Unsigned(size)
				                    << " submenu(s) not closed.\n";
			}
			for (std::size_t i = 0; i < (size - 1); i++) {
				menuStack.pop();
			}
			menu = menuStack.top();
			menuStack.pop();
		}
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(parent), menu);
	}
}

ui::Menu g_plugins_menu{ui::null};
ui::MenuItem g_plugins_menu_separator{ui::null};

void PluginsMenu_populate()
{
	class PluginsMenuConstructor : public PluginsVisitor {
	ui::Menu m_menu;
public:
	PluginsMenuConstructor(ui::Menu menu) : m_menu(menu)
	{
	}

	void visit(IPlugIn &plugin)
	{
		PlugInMenu_Add(m_menu, &plugin);
	}
	};

	PluginsMenuConstructor constructor(g_plugins_menu);
	GetPlugInMgr().constructMenu(constructor);
}

void PluginsMenu_clear()
{
	m_nNextPlugInID = 0;

	GList *lst = g_list_find(gtk_container_get_children(GTK_CONTAINER(g_plugins_menu)),
	                         g_plugins_menu_separator._handle);
	while (lst->next) {
		g_plugins_menu.remove(ui::Widget::from(lst->next->data));
		lst = g_list_find(gtk_container_get_children(GTK_CONTAINER(g_plugins_menu)), g_plugins_menu_separator._handle);
	}
}

ui::MenuItem create_plugins_menu()
{
	// Plugins menu
	auto plugins_menu_item = new_sub_menu_item_with_mnemonic("_Plugins");
	auto menu = ui::Menu::from(gtk_menu_item_get_submenu(plugins_menu_item));
	/*if (g_Layout_enableOpenStepUX.m_value) {
	    menu_tearoff(menu);
	   }*/

	g_plugins_menu = menu;

	//TODO: some modules/plugins do not yet support refresh
#if 0
	create_menu_item_with_mnemonic( menu, "Refresh", makeCallbackF(Restart) );

	// NOTE: the seperator is used when doing a refresh of the list, everything past the seperator is removed
	g_plugins_menu_separator = menu_separator( menu );
#endif

	PluginsMenu_populate();

	return plugins_menu_item;
}
