/*****************************************************************************
 * Copyright (C) Shivkumar Patil patilsr15.it@coep.ac.in
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "display.h"

/*creat menubar, add menuItems, append submenuItems 
* give shortcut keys to it
*/
void menubar_display(void) {
	//creats a accelrator group from which shortcut keys can be assigned
	accelGroup = gtk_accel_group_new(); 
	gtk_window_add_accel_group(GTK_WINDOW(e.window), accelGroup);

	/*creats a menubar*/
	menubar = gtk_menu_bar_new();
	fileMenu = gtk_menu_new();
	editMenu = gtk_menu_new();
	searchMenu = gtk_menu_new();
	toolsMenu = gtk_menu_new();
	helpMenu = gtk_menu_new();

	/*get file menu items*/	
	//mnemonics gives shortcut key to widget
	fileMenuItem = gtk_menu_item_new_with_mnemonic("_File");
	newMenuItem = gtk_menu_item_new_with_label("New");
	openMenuItem = gtk_menu_item_new_with_label("Open");
	saveMenuItem = gtk_menu_item_new_with_label("Save");
	saveAsMenuItem = gtk_menu_item_new_with_label("Save As..");
	quitMenuItem = gtk_menu_item_new_with_label("Quit");
	
	/*get edit menu items*/
	//mnemonics gives shortcut key to widget
	editMenuItem = gtk_menu_item_new_with_mnemonic("_Edit");
	cutMenuItem = gtk_menu_item_new_with_label("Cut"); 
	copyMenuItem = gtk_menu_item_new_with_label("Copy"); 
	pasteMenuItem = gtk_menu_item_new_with_label("Paste"); 
	deleteMenuItem = gtk_menu_item_new_with_label("Delete");
	selectallMenuItem = gtk_menu_item_new_with_label("Select All");
	preferencesMenuItem = gtk_menu_item_new_with_label("Preferences");

	/*get search menu items */
	//mnemonics gives shortcut key to widget
	searchMenuItem = gtk_menu_item_new_with_mnemonic("_Search");
	findMenuItem = gtk_menu_item_new_with_label("Find");
	findandreplaceMenuItem = gtk_menu_item_new_with_label("Find and Replace");
	
	/*get tools menu items */
	//mnemonics gives shortcut key to widget
	toolsMenuItem = gtk_menu_item_new_with_mnemonic("_Tools");
	datentimeMenuItem = gtk_menu_item_new_with_label("Insert Date and Time...");
	gotolineMenuItem = gtk_menu_item_new_with_label("Go to line ..");
	
	/* get help menu items */
	//mnemonics gives shortcut key to widget
	help = gtk_menu_item_new_with_mnemonic("_Help");
	helpMenuItem = gtk_menu_item_new_with_label("Help");
	aboutMenuItem = gtk_menu_item_new_with_label("About");
	
	/*append File subMenu items to file menu item and file menu item to menubar*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuItem), fileMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());//separator  creats separator line
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveAsMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMenuItem);

	/*append Edit subMenu items to edit menu item and edit menu item to menubar*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(editMenuItem), editMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), cutMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), copyMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), pasteMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), deleteMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), selectallMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), preferencesMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), editMenuItem);

	/*append search subMenu items to search menu item and search menu item to menubar*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(searchMenuItem), searchMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(searchMenu), findMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(searchMenu), findandreplaceMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), searchMenuItem);
	
	/*append search subMenu items to search menu item and search menu item to menubar*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(toolsMenuItem), toolsMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(toolsMenu), datentimeMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(toolsMenu), gotolineMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), toolsMenuItem);
	
	/*append search subMenu items to search menu item and search menu item to menubar*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), helpMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), aboutMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);

	/*shortcut key access for file submenu items
	* these key are taken from accelgroup 
	*/
	gtk_widget_add_accelerator(newMenuItem, "activate", accelGroup, GDK_t, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(openMenuItem, "activate", accelGroup, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(saveMenuItem, "activate", accelGroup, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(saveAsMenuItem, "activate", accelGroup, GDK_s, GDK_CONTROL_MASK |GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(quitMenuItem, "activate", accelGroup, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	
	/*shortcut keys for edit menu*/
	gtk_widget_add_accelerator(cutMenuItem, "activate", accelGroup, GDK_x, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(copyMenuItem, "activate", accelGroup, GDK_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(pasteMenuItem, "activate", accelGroup, GDK_v, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(deleteMenuItem, "activate", accelGroup, GDK_d, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(selectallMenuItem, "activate", accelGroup, GDK_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	/*shortcut keys for search menu */
	gtk_widget_add_accelerator(findMenuItem, "activate", accelGroup, GDK_f, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(findandreplaceMenuItem, "activate", accelGroup, GDK_h, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
		
	/*shortcut keys for tools menu */
	gtk_widget_add_accelerator(datentimeMenuItem, "activate", accelGroup, GDK_i, GDK_SHIFT_MASK |GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(gotolineMenuItem, "activate", accelGroup, GDK_i, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	/*shortcut keys for help */
	gtk_widget_add_accelerator(helpMenuItem, "activate", accelGroup, GDK_KEY_F1, 0, GTK_ACCEL_VISIBLE);
}

void toolbar_display(void) {
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	
	//tool buttons 
	newToolButton = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	openToolButton = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	saveToolButton = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	
	/*append tool buttons in toolbars
	* last argument gives position of button in bar: 0 = first , -1 = end 
	*/
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), openToolButton, 0);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), newToolButton, -1);	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), saveToolButton, -1);
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 5);
}

