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

#ifndef __COMMAND_H
#define __COMMAND_H

//this struct is for findandReplace function
typedef struct value {
	GtkWidget *searchValue;
	GtkWidget *replaceValue;
}val;

//this struct handles main window widgets
typedef struct editor {
	GtkWidget *window;
	GtkWidget *textV;
	GtkTextBuffer *textB;
	GtkTextIter iter;
}editor;

editor e;

//creats a GdkAtom which can store selection field in a clipboard
GdkAtom selection = GDK_SELECTION_CLIPBOARD;
//filename  is stored in fname
char fname[1024];
//flag checks whether file has name or not
int flag;

void save(char *filename);
void save_file(GtkWidget *widget, gpointer data);
void save_file_as(GtkWidget *widget, gpointer data);
gboolean is_modified(void);
gboolean save_changes(char *oldfile);
void statusbar_display(GtkTextBuffer *widget, GtkStatusbar *statusBar);
void new_mark(GtkTextBuffer *widget, GtkTextIter *iter, GtkTextMark *mark, gpointer data);
void user_file(char *path);
void file_open(GtkWidget *widget, gpointer data);
void new_file_open(GtkWidget *widget, gpointer data);
void cut_section(GtkWidget *widget, gpointer data);
void copy_section(GtkWidget *widget, gpointer data);
void paste_section(GtkWidget *widget, gpointer data);
void delete_section(GtkWidget *widget, gpointer data);
void select_all_section(GtkWidget *widget, gpointer data);
void date_n_time(GtkWidget *widget, gpointer data);
void search_window(GtkWidget *widget, gpointer data);
void search_text(GtkWidget *widget, GtkWidget *value);
void search_next(GtkWidget *widget, GtkWidget *value);
void search_previous(GtkWidget *widget, GtkWidget *value);
void search_replace_window(GtkWidget *widget, gpointer data);
void Replace_text(GtkWidget *widget, val *);
void ReplaceAll_text(GtkWidget *widget, val *v);
void go_to_line_window(GtkWidget *widget);
void go_to_line(GtkWidget *widget, GtkWidget *value);
void preferences_window(GtkWidget *widget);
void font_ok(GtkWidget *widget, GtkWidget *fontDialog);
void font_apply(GtkWidget *widget, GtkWidget *fontDialog);
void font_cancel(GtkWidget *widget, GtkWidget *fontDialog);
void help_info(GtkWidget *widget);
void about_info(GtkWidget *widget);
void clear_screen(void);
void error_dialog(char *str);
void quit_window(GtkWidget *widget);

#endif 
