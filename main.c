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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "command.c"
#include "display.c"
#include "display.h"

int main(int argc, char *argv[]) {
	GtkWidget *statusBar;
	GtkWidget *scrollWindow;
	GtkWidget *vbox;
	
	/*gtk initialised here*/
	gtk_init(&argc, &argv);
	
	/*creat scroll window with adjustment set to NULL and policy to automatic
	* then created actual window where I am going to work 
	* given title , size, initial position w.r.t. screen
	*/
	scrollWindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	e.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(e.window), "Untitled Document");
	gtk_window_set_default_size(GTK_WINDOW(e.window), 650, 500);
	gtk_window_set_position(GTK_WINDOW(e.window), GTK_WIN_POS_CENTER);

	/*creat vbox to manage different widgets like menubar, toolbar etc.position 
	* on my window, these are not placed homogeniously on window
	*/
	vbox = gtk_vbox_new(FALSE, 0);
		
	/*Text View
	* this is the widget where i can write the text 
	* the various operation on text like cut, copy, paste are done using Text Buffer
	* we then add text view widget to scroll window to get scrollability
	*/
	e.textV = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(e.textV), GTK_WRAP_WORD);
	e.textB = gtk_text_view_get_buffer(GTK_TEXT_VIEW(e.textV));
	gtk_widget_grab_focus(e.textV);
	gtk_container_add(GTK_CONTAINER(scrollWindow), e.textV);
	
	/*creats statusbar */
	statusBar = gtk_statusbar_new();
	
	/*creats menuitems and add it to menubar*/
	menubar_display();

	/*creats toolbuttons and add it to toolbar*/
	toolbar_display();
	
	/*arreng all the widgets in a box
	* 2nd, 3rd, 4th arguments are for expandability, fillness and padding of widget
	*/
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), scrollWindow, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), statusBar, FALSE, FALSE, 3);

	/*add the box in main window */
	gtk_container_add(GTK_CONTAINER(e.window), vbox);
	
	if(argc == 1) {
		gtk_text_buffer_get_start_iter(e.textB, &e.iter);
		gtk_text_buffer_place_cursor(e.textB, &e.iter);
		gtk_text_buffer_set_modified(e.textB, FALSE);
		flag = 1; 
	}
	else if(argc == 2) {
		user_file(argv[1]);
	}

	/*signals from various buttons on menubar, toolbar are connected 
	* to functions that work on it
	* 1st argument- button from which signal emitted 
	* 2nd argument- signal name
	* 3rd argument- function to work on signal
	* 4th argument- data (if any) to pass to function
	*/
	g_signal_connect(G_OBJECT(newMenuItem), "activate", G_CALLBACK(new_file_open), NULL);
	g_signal_connect(G_OBJECT(openMenuItem), "activate", G_CALLBACK(file_open), NULL);	
	g_signal_connect(G_OBJECT(saveMenuItem), "activate", G_CALLBACK(save_file), NULL);
	g_signal_connect(G_OBJECT(saveAsMenuItem), "activate", G_CALLBACK(save_file_as), NULL);
	g_signal_connect(G_OBJECT(quitMenuItem), "activate", G_CALLBACK(quit_window), NULL);
	g_signal_connect(G_OBJECT(cutMenuItem), "activate", G_CALLBACK(cut_section), NULL);	
	g_signal_connect(G_OBJECT(copyMenuItem), "activate", G_CALLBACK(copy_section), NULL);	
	g_signal_connect(G_OBJECT(pasteMenuItem), "activate", G_CALLBACK(paste_section), NULL);
	g_signal_connect(G_OBJECT(deleteMenuItem), "activate", G_CALLBACK(delete_section), NULL);
	g_signal_connect(G_OBJECT(selectallMenuItem), "activate", G_CALLBACK(select_all_section), NULL);
	g_signal_connect(G_OBJECT(preferencesMenuItem), "activate", G_CALLBACK(preferences_window), NULL);
	g_signal_connect(G_OBJECT(findMenuItem), "activate", G_CALLBACK(search_window), NULL);
	g_signal_connect(G_OBJECT(findandreplaceMenuItem), "activate", G_CALLBACK(search_replace_window), NULL);
	g_signal_connect(G_OBJECT(datentimeMenuItem), "activate", G_CALLBACK(date_n_time), NULL);
	g_signal_connect(G_OBJECT(gotolineMenuItem), "activate", G_CALLBACK(go_to_line_window), NULL);
	g_signal_connect(G_OBJECT(helpMenuItem), "activate", G_CALLBACK(help_info), NULL);
	g_signal_connect(G_OBJECT(aboutMenuItem), "activate", G_CALLBACK(about_info), NULL);
	g_signal_connect(G_OBJECT(newToolButton), "clicked", G_CALLBACK(new_file_open), NULL);	
	g_signal_connect(G_OBJECT(openToolButton), "clicked", G_CALLBACK(file_open), NULL);	
	g_signal_connect(G_OBJECT(saveToolButton), "clicked", G_CALLBACK(save_file), NULL);	
	g_signal_connect_object(G_OBJECT(e.textB), "mark-set", G_CALLBACK(new_mark), statusBar, 0);
	g_signal_connect(G_OBJECT(e.textB), "changed", G_CALLBACK(statusbar_display), statusBar);
	g_signal_connect(G_OBJECT(e.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	/*show the window with argument*/
	gtk_widget_show_all(e.window);
	/*window starts here */
	gtk_main();
	return 0;
}

