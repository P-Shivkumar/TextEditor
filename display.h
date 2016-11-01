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
#ifndef __DISPLAY_H
#define __DISPLAY_H

GtkWidget *menubar;
GtkWidget *fileMenu, *fileMenuItem, *quitMenuItem, *newMenuItem, *openMenuItem, *saveMenuItem, *saveAsMenuItem;
GtkWidget *toolbar;
GtkToolItem *newToolButton, *openToolButton, *saveToolButton;
GtkWidget *editMenu, *editMenuItem, *selectallMenuItem;
GtkWidget *copyMenuItem, *pasteMenuItem, *cutMenuItem, *deleteMenuItem, *preferencesMenuItem;
GtkWidget *toolsMenu, *toolsMenuItem, *datentimeMenuItem, *gotolineMenuItem;
GtkWidget *searchMenu, *searchMenuItem, *findMenuItem, *findandreplaceMenuItem;
GtkWidget *helpMenu, *help, *helpMenuItem, *aboutMenuItem;
GtkAccelGroup *accelGroup;

void menubar_display(void);
void toolbar_display(void);

#endif
