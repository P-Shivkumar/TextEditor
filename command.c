
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
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "command.h"

gboolean is_modified(void) {
	/*check wether the buffer gets modified or not
	*/
	if(gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(e.textB)))
		return TRUE;
	else 
		return FALSE;
}

void error_dialog(char *str) {
	GtkWidget *dialog;
	/*creat a msg box saying error, gtk_dialog_run functions get
	* the signal from box and if non zero signal then destroy the box
	*/
	dialog = gtk_message_dialog_new(GTK_WINDOW(e.window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", str);
	if(gtk_dialog_run(GTK_DIALOG(dialog))) {
		gtk_widget_destroy(dialog);
	}
}

void clear_screen(void) {
	/*get first and last node of buffer 
	* select the region and delete it
	*/
	GtkTextIter first, last;
	gtk_text_buffer_get_start_iter(e.textB, &first);
	gtk_text_buffer_get_end_iter(e.textB, &last);
	gtk_text_buffer_select_range(e.textB, &first, &last);
	gtk_text_buffer_delete_selection(e.textB, TRUE, TRUE);//last two arguments checks wether buffer is editable or not
}

gboolean save_changes(char *oldfile) {
	/*creat a msg box asking question wether to save earlier file?
	* if response is yes then save and delete box
	* if no then do nothing to buffer and delete box
	* if cancel delete box and return false(haven't changed)
	*/
	int response;
	GtkWidget *msg_dialog;
	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(e.window), 0, GTK_MESSAGE_QUESTION,GTK_BUTTONS_NONE, "Do you want to save changes to earlier file?");
	gtk_dialog_add_buttons(GTK_DIALOG(msg_dialog), GTK_STOCK_YES, GTK_RESPONSE_YES, GTK_STOCK_NO, GTK_RESPONSE_NO, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL); 
	response = gtk_dialog_run(GTK_DIALOG(msg_dialog));
	if(response == GTK_RESPONSE_YES) {
		save(oldfile);
		gtk_widget_destroy(msg_dialog);
		return TRUE;
	}
	else if(response == GTK_RESPONSE_NO) {
		gtk_widget_destroy(msg_dialog);
		return TRUE;
	}
	gtk_widget_destroy(msg_dialog);
	return FALSE;
}

void statusbar_display(GtkTextBuffer *widget, GtkStatusbar *statusBar) {
	gint y, x, coller_id = 0;
	/*statusbar work in LIFO fashion 
	* pop msg, given last time in statusbar
	* get new line and column no.
	* and push it to statusbar 
	*/
	/*caller_id is unique id given to msg so that we can recognize it
	*/
	gtk_statusbar_pop(statusBar, coller_id);
	gtk_text_buffer_get_iter_at_mark(widget, &e.iter, gtk_text_buffer_get_insert(widget));
	y = gtk_text_iter_get_line(&e.iter);
	x = gtk_text_iter_get_line_offset(&e.iter);
	//creat a string to push to statusbar
	char *new_msg = g_strdup_printf("Line : %d, Col :%d", y + 1, x + 1);
	gtk_statusbar_push(statusBar, coller_id, new_msg);
	free(new_msg);
}

void new_mark(GtkTextBuffer *widget, GtkTextIter *iter, GtkTextMark *mark, gpointer data) {
	//this is by default signal handler function it works whenever mark is changed
	statusbar_display(widget, GTK_STATUSBAR(data));
}

void user_file(char *path) {
	int fd, x;
	char ch;
	/*open file given by user as cmd line argument
	*/
	if((fd = open(path, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
		perror("File cannot open");
		error_dialog("SORRY.. File is not opening");
	}
	else {
		/* Read a character and insert at cursor
		*/
		while((x = read(fd, &ch, 1))) {
			gtk_text_buffer_insert_at_cursor(e.textB, &ch, 1);
		}
		strcpy(fname,path);
		gtk_text_buffer_set_modified(e.textB, FALSE);//we haven't modified the file yet so set modifier to false
		gtk_text_buffer_get_iter_at_offset(e.textB, &e.iter, 0);
		gtk_text_buffer_place_cursor(e.textB, &e.iter);//place cursor at (0,0)
		gtk_window_set_title(GTK_WINDOW(e.window), fname);//set title of window as file-name
		flag = 0;//as we have filename
	}
	close(fd);
}

void file_open(GtkWidget *widget, gpointer data) {
	GtkWidget *choosefile;
	int fd, x;
	char ch;
	/*creat box to open file chooser menu, gtk_dialog_run get the response of buttons
	* if response is yes then open the file 
	* else destroy the box and return 
	*/
	choosefile = gtk_file_chooser_dialog_new("File Selection", GTK_WINDOW(e.window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_APPLY, NULL);
	if(gtk_dialog_run(GTK_DIALOG(choosefile)) == GTK_RESPONSE_APPLY) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choosefile));
		/*check wether earlier file changed, if yes save earlier file*/
		if(is_modified()) {
			if(save_changes(fname) == FALSE) { //if cancel then destroy file choosing box and return
				gtk_widget_destroy(choosefile);
				return;
			}
		}
		/*now clear screen and open file 
		* Read a character and insert at cursor
		*/
		clear_screen();
		strcpy(fname, filename);
		if((fd = open(fname, O_RDONLY)) == -1) {
			perror("Error in opening file");
			error_dialog("SORRY.. File is not opening");
		}
		while((x = read(fd, &ch, 1))) {
			gtk_text_buffer_insert_at_cursor(e.textB, &ch, 1);
		}
		gtk_window_set_title(GTK_WINDOW(e.window), fname);
		gtk_text_buffer_get_iter_at_offset(e.textB, &e.iter, 0);
		gtk_text_buffer_place_cursor(e.textB, &e.iter);
		gtk_text_buffer_set_modified(e.textB, FALSE);
		flag = 0;//as we have filename
		g_free(filename);
	}
	gtk_widget_destroy(choosefile);
	close(fd);
}

void new_file_open(GtkWidget *widget, gpointer data) {
	/*check wether earlier file is modified if 
	* yes then save it 
	*/
	if(is_modified()) {
		if(save_changes(fname) == FALSE) {
			return ;
		}
	}
	/*clear screen and set title as Untitled doc
	*/
	clear_screen();
	gtk_window_set_title(GTK_WINDOW(e.window), "Untitled Document");
	gtk_text_buffer_get_iter_at_offset(e.textB, &e.iter, 0);
	gtk_text_buffer_place_cursor(e.textB, &e.iter);
	gtk_text_buffer_set_modified(e.textB, FALSE);
	flag = 1;//as we don't have filename
}
void save_file(GtkWidget *widget, gpointer data) {
	save(fname);
}

void save(char *filename) {
	int fd;
	GtkWidget *choosefile;
	/*flag = 1 represents file has no name 
	* if filename is unknown creat file saving box(GTK_FILE_CHOOSER_ACTION_SAVE)
	* and get filename first
	* if save cancel then return
	*/
	if(flag == 1) {
		choosefile = gtk_file_chooser_dialog_new("Save As..", GTK_WINDOW(e.window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_APPLY, NULL);
		if(gtk_dialog_run(GTK_DIALOG(choosefile)) == GTK_RESPONSE_APPLY) {
			strcpy(filename, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choosefile)));
			gtk_window_set_title(GTK_WINDOW(e.window), filename);
			gtk_widget_destroy(choosefile);
		}
		else {
			gtk_widget_destroy(choosefile); 
			return;
		}
	}
	/*open the file in write only mode either by creating it or by truncating its lengt to zero bytes
	*/
	if((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {   
		perror("error in opening:\n");
		error_dialog("SORRY.. File is not opening");
	}
	else {
		GtkTextIter first, last, tmp;
		/*get first last nodes of buffer 
		* take a tmp node and initilize it to first node
		* move tmp node to next line and copy that line from first node to tmp node 
		* converts string from UTF-8 to locale and forward first node to tmp node 
		* loop will continue till first node and last node matches
		*/
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(e.textB), &first, &last);
		tmp = first;
		
		while(!gtk_text_iter_equal(&first, &last)) {
			gchar *line, *slice;
			gsize bytes_read, bytes_write;
			gtk_text_iter_forward_line(&tmp);
			line = gtk_text_iter_get_slice(&first, &tmp);
			/*character set conversion function */
			slice = g_locale_from_utf8(line, -1, &bytes_read, &bytes_write, NULL);
			write(fd, slice, bytes_write);
			g_free(line);
			g_free(slice);
			first = tmp;
			gtk_text_buffer_set_modified(e.textB, FALSE);
		}
		gtk_text_buffer_set_modified(e.textB, FALSE);
		flag = 0;//as we have filename
		if(filename != fname) {
			g_free(fname);
			strcpy(fname, filename);	
		}
	}
}

void save_file_as(GtkWidget *widget, gpointer data) {
	int fd;
	GtkWidget *choosefile;
	/*considering file has know name we creat the box to save file 
	* by taking filename and rest is similar to save
	*/
	choosefile = gtk_file_chooser_dialog_new("Save As..", GTK_WINDOW(e.window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_APPLY, NULL);
	if(gtk_dialog_run(GTK_DIALOG(choosefile)) == GTK_RESPONSE_APPLY) {
			strcpy(fname, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choosefile)));
			gtk_window_set_title(GTK_WINDOW(e.window), fname);
			gtk_widget_destroy(choosefile);
		}
	else {
		gtk_widget_destroy(choosefile);
		return;
	}
	if((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {    
		perror("error in opening:\n");
		error_dialog("SORRY.. File is not opening");
	}
	else {
		GtkTextIter first, last, tmp;
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(e.textB), &first, &last);
		tmp = first;
		while(!gtk_text_iter_equal(&first, &last)) {
			gchar *line, *slice;
			gsize bytes_read, bytes_write;
			gtk_text_iter_forward_line(&tmp);
			line = gtk_text_iter_get_slice(&first, &tmp);
			/*character set conversion function */
			slice = g_locale_from_utf8(line, -1, &bytes_read, &bytes_write, NULL); 		
			write(fd, slice, bytes_write);
			g_free(line);	
			g_free(slice);
			first = tmp;
			gtk_text_buffer_set_modified(e.textB, FALSE);
		}
		flag = 0;//as we have filename
	}
}

/*in cut, copy, paste, delete store the selected part of buffer in a clipboard (here selection)
* and work on that part using predefined function
*/

void cut_section(GtkWidget *widget, gpointer data) {
	gtk_text_buffer_cut_clipboard(e.textB, gtk_clipboard_get(selection), TRUE);
}

void copy_section(GtkWidget *widget, gpointer data) {
	gtk_text_buffer_copy_clipboard(e.textB, gtk_clipboard_get(selection));
}

void paste_section(GtkWidget *widget, gpointer data) {
	gtk_text_buffer_paste_clipboard(e.textB, gtk_clipboard_get(selection), NULL, TRUE);
}

void delete_section(GtkWidget *widget, gpointer data) {
	gtk_text_buffer_delete_selection(e.textB, TRUE, TRUE);
}

void select_all_section(GtkWidget *widget, gpointer data) {
	GtkTextIter first, last;
	/*get first and last node of buffer and select the region in between
	*/
	gtk_text_buffer_get_start_iter(e.textB, &first);
	gtk_text_buffer_get_end_iter(e.textB, &last);
	gtk_text_buffer_select_range(e.textB, &first, &last);
}

void date_n_time(GtkWidget *widget, gpointer data) {
	time_t now = time(NULL);//get time 
	char buff[32];
	//convert time to string format predefined function
	strftime(buff, 32, "%Y-%m-%d %H:%M:%s", localtime(&now));
	gtk_text_buffer_insert_at_cursor(e.textB, buff, strlen(buff) - 1);
}

void search_window(GtkWidget *widget, gpointer data) {
	GtkWidget *searchWin, *searchVbox, *searchHbox;
	GtkWidget *search, *next, *previous, *value;
	
	//creat window to search text
	searchWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(searchWin), "search..");
	//creat box to arrange the buttons and entry field nicely on window
	searchVbox = gtk_vbox_new(FALSE, 5);
	searchHbox = gtk_hbox_new(FALSE, 5);

	//creat buttons search next and  previous 
	search = gtk_button_new_from_stock(GTK_STOCK_FIND);
	next = gtk_button_new_with_label(">");
	previous = gtk_button_new_with_label("<");
	
	//creat a entry box where user can enter data
	value = gtk_entry_new();

	//add buttons, entry field to search window
	gtk_container_add(GTK_CONTAINER(searchWin), searchVbox);
	gtk_box_pack_start(GTK_BOX(searchVbox), searchHbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchHbox), value, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(searchHbox), search, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchHbox), previous, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchHbox), next, FALSE, FALSE, 0);
	
	//functions to work on when search next previous button clicked
	g_signal_connect(G_OBJECT(search), "clicked", G_CALLBACK(search_text), value);
	g_signal_connect(G_OBJECT(next), "clicked", G_CALLBACK(search_next), value);	
	g_signal_connect(G_OBJECT(previous), "clicked", G_CALLBACK(search_previous), value);
	
	gtk_widget_show_all(searchWin);	
}

void search_text(GtkWidget *widget, GtkWidget *value) {
	GtkTextIter match_start, match_end;
	/*take the text to be searched 
	* set the node to start of buffer
	* search for the text and if match found highlight it 
	* and make a mark at first and last point of search (helpful for next search and previous search)
	*/
	char *text = (char *)gtk_entry_get_text((GtkEntry *)value);
	gtk_text_buffer_get_iter_at_offset(e.textB, &e.iter, 0);
	if(gtk_text_iter_forward_search(&e.iter, text, 0, &match_start, &match_end, NULL) ) {
		gtk_text_buffer_select_range(e.textB, &match_start, &match_end);
		gtk_text_buffer_create_mark(e.textB, "end", &match_end, TRUE);//mark is identified by name end
		gtk_text_buffer_create_mark(e.textB, "first", &match_start, FALSE);//mark is identified by name first
	}
}

void search_next(GtkWidget *widget, GtkWidget *value) {
	GtkTextIter match_start, match_end;
	GtkTextMark *mark1;
	/*get the mark at end of first search (if no first mark then return)
	* and searches forward till next search found
	* if match found highlight it and make mark
	*/
	mark1 = gtk_text_buffer_get_mark(e.textB, "end");
	if(mark1 == NULL) {
		return;
	}
	gtk_text_buffer_get_iter_at_mark(e.textB, &e.iter, mark1);
	char *text = (char *)gtk_entry_get_text((GtkEntry *)value);
	if(gtk_text_iter_forward_search(&e.iter, text, 0, &match_start, &match_end, NULL) ) {
		gtk_text_buffer_select_range(e.textB, &match_start, &match_end);
		gtk_text_buffer_create_mark(e.textB, "end", &match_end, TRUE);
		gtk_text_buffer_create_mark(e.textB, "first", &match_start, FALSE);
	}
}

void search_previous(GtkWidget *widget, GtkWidget *value) {
	GtkTextIter match_start, match_end;
	GtkTextMark *mark1;
	/*get the mark at start of first search(if no first mark then return)
	* and searches backward till previous search found
	* if match found highlight it and make mark
	*/
	mark1 = gtk_text_buffer_get_mark(e.textB, "first");
	if(mark1 == NULL) {
		return;
	}
	gtk_text_buffer_get_iter_at_mark(e.textB, &e.iter, mark1);
	char *text = (char *)gtk_entry_get_text((GtkEntry *)value);
	if(gtk_text_iter_backward_search(&e.iter, text, 0, &match_start, &match_end, NULL) ) {
		gtk_text_buffer_select_range(e.textB, &match_start, &match_end);
		gtk_text_buffer_create_mark(e.textB, "end", &match_end, TRUE);
		gtk_text_buffer_create_mark(e.textB, "first", &match_start, FALSE);
	}
}

void search_replace_window(GtkWidget *widget, gpointer data) {
	GtkWidget *searchReplaceWin, *searchReplaceVbox;
	GtkWidget *search, *searchNext, *Replace, *ReplaceAll;
	val  *v1;

	/*creat a window to work on search and replace functions
	* creat a box to arrange the buttons, entry field
	*/
	searchReplaceWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(searchReplaceWin), "Search And Replace..");
	gtk_window_set_default_size(GTK_WINDOW(searchReplaceWin), 500, 200);
	gtk_window_set_position(GTK_WINDOW(searchReplaceWin), GTK_WIN_POS_CENTER);
	searchReplaceVbox = gtk_vbox_new(FALSE, 10);
	
	v1 = (val *)malloc(sizeof(val));
	/*creat search entry field and
	* replace entry field using gtk_entry_new()
	*/
	v1->searchValue = gtk_entry_new();
	v1->replaceValue = gtk_entry_new();
	
	/*creat buttons and set there sizes*/
	search = gtk_button_new_from_stock(GTK_STOCK_FIND);
	searchNext = gtk_button_new_with_label("Find Next");
	Replace = gtk_button_new_with_label("Replace");
	ReplaceAll = gtk_button_new_with_label("Replace All");
	gtk_widget_set_size_request(search, 70, 30);
	gtk_widget_set_size_request(Replace, 70, 30);
	gtk_widget_set_size_request(ReplaceAll, 70, 30);

	//arrange all the widgets in box and add box in the search-replace window
	gtk_container_add(GTK_CONTAINER(searchReplaceWin), searchReplaceVbox);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), v1->searchValue, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), search, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), searchNext, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), v1->replaceValue, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), Replace, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(searchReplaceVbox), ReplaceAll, FALSE, FALSE, 0);
	
	//connect buttons to the function that work on it
	g_signal_connect(G_OBJECT(search), "clicked", G_CALLBACK(search_text), v1->searchValue);
	g_signal_connect(G_OBJECT(searchNext), "clicked", G_CALLBACK(search_next), v1->searchValue);
	g_signal_connect(G_OBJECT(Replace), "clicked", G_CALLBACK(Replace_text), v1);
	g_signal_connect(G_OBJECT(ReplaceAll), "clicked", G_CALLBACK(ReplaceAll_text), v1);	
	gtk_widget_show_all(searchReplaceWin);
}

void Replace_text(GtkWidget *widget, val *v) {
	GtkTextIter match_start, match_end;
	char *textR, *textS; 
	/*get the text from search entry field and replace entry field
	* search for the  textS and if found delete the earlier text 
	* and insert new textR
	*/
	textS = (char *)gtk_entry_get_text((GtkEntry *)v->searchValue);
	textR = (char *)gtk_entry_get_text((GtkEntry *)v->replaceValue);
	if(gtk_text_iter_forward_search (&e.iter, textS, 0, &match_start, &match_end, NULL)) {
		gtk_text_buffer_select_range (e.textB, &match_start, &match_end);
		gtk_text_buffer_create_mark (e.textB, "end", &match_end, FALSE);
		gtk_text_buffer_delete(e.textB, &match_start, &match_end);
		gtk_text_buffer_insert(e.textB, &match_start, textR, strlen(textR));
	}
}

void ReplaceAll_text(GtkWidget *widget, val *v) {
	GtkTextIter match_start, match_end;
	char *textR, *textS;
	gtk_text_buffer_get_iter_at_offset(e.textB, &e.iter, 0);
	/*get the text from search entry field and replace entry field
	* search for the  textS and if found delete the earlier text 
	* and insert new textR 
	* loop will continue till no more search text is found
	*/
	textS = (char *)gtk_entry_get_text((GtkEntry *)v->searchValue);
	textR = (char *)gtk_entry_get_text((GtkEntry *)v->replaceValue);
	while(gtk_text_iter_forward_search(&e.iter, textS, 0, &match_start, &match_end, NULL)) {
		gtk_text_buffer_select_range (e.textB, &match_start, &match_end);
		gtk_text_buffer_create_mark (e.textB, "end", &match_end, FALSE);
		gtk_text_buffer_delete(e.textB, &match_start, &match_end);
		gtk_text_buffer_insert(e.textB, &match_start, textR, strlen(textR));
	}
}

void go_to_line_window(GtkWidget *widget) {
	GtkWidget *lineWin, *lineHbox;
	GtkWidget *enter, *value;
	
	/*creat a window for input
	* creat a box to arreng the widgets
	* creat OK button, and entry field
	*/
	lineWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(lineWin), "Go to line..");
	gtk_window_set_default_size(GTK_WINDOW(lineWin), 50, 10);
	lineHbox = gtk_hbox_new(FALSE, 10);
	enter = gtk_button_new_from_stock(GTK_STOCK_OK);
	value = gtk_entry_new();
	
	/*arreng the widgets*/
	gtk_container_add(GTK_CONTAINER(lineWin), lineHbox);
	gtk_box_pack_start(GTK_BOX(lineHbox), value, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lineHbox), enter, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(enter), "clicked", G_CALLBACK(go_to_line), value);
	gtk_widget_show_all(lineWin);
}

void go_to_line(GtkWidget *widget, GtkWidget *value) {
	char str[128];
	GtkTextMark *mark1;
	int i = 0, line_no = 0, total_lines;
	/*get string from the entry field
	* convert string into integer 
	*/
	strcpy(str, (char *)gtk_entry_get_text((GtkEntry *)value));
	while(str[i] != '\0') {
		if(isdigit(str[i])) { //takes digit from 0 to 9
			line_no = line_no * 10 + str[i] -'0';
			i++;
		}
		else {
			error_dialog("SORRY...Please Enter Positive Integer Value. ");
			return;
		}
	}
	/*get total line count from buffer
	* if entered line number exceeds the line number	
	* give error msg
	* else go to given line number
	*/
	total_lines = gtk_text_buffer_get_line_count(e.textB);
	if(total_lines < line_no) {
		error_dialog("SORRY...Given Line Number Doesn't exist...");
		return;
	}
	gtk_text_buffer_get_iter_at_line(e.textB, &e.iter, line_no - 1);
	gtk_text_buffer_place_cursor(e.textB, &e.iter);
	gtk_text_buffer_create_mark(e.textB, "line", &e.iter, TRUE);
	mark1 = gtk_text_buffer_get_mark(e.textB, "line");
	gtk_text_view_scroll_mark_onscreen((GtkTextView *)e.textV, mark1);
}

void preferences_window(GtkWidget *widget) {
	GtkWidget *fontDialog, *fontApply;
	GtkWidget *fontOk, *fontCancel;
	/*creat font selection box
	* creat buttons
	*/
	fontDialog = gtk_font_selection_dialog_new("font preference");
	fontOk = GTK_FONT_SELECTION_DIALOG(fontDialog)->ok_button;
	fontCancel = GTK_FONT_SELECTION_DIALOG(fontDialog)->cancel_button;
	fontApply = GTK_FONT_SELECTION_DIALOG(fontDialog)->apply_button;
	
	g_signal_connect(G_OBJECT(fontOk), "clicked", G_CALLBACK(font_ok), fontDialog);
	g_signal_connect(G_OBJECT(fontCancel), "clicked", G_CALLBACK(font_cancel), fontDialog);
	g_signal_connect(G_OBJECT(fontApply), "clicked", G_CALLBACK(font_apply), fontDialog);
	
	gtk_widget_show_all(fontDialog);
}

void font_ok(GtkWidget *widget, GtkWidget *fontDialog) {
	/*if ok clicked 
	* get the description of font 
	* and modify the font of text View accordingly
	* and destroy the box
	*/
	PangoFontDescription *description = pango_font_description_from_string(gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(fontDialog)));
	gtk_widget_modify_font(e.textV, description);
	gtk_widget_destroy(fontDialog);
}

void font_apply(GtkWidget *widget, GtkWidget *fontDialog) {
	/*if apply clicked 
	* get the description of font 
	* and modify the font of text View accordingly
	* but don't destroy the box
	*/
	PangoFontDescription *description = pango_font_description_from_string(gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(fontDialog)));
	gtk_widget_modify_font(e.textV, description);
}

void font_cancel(GtkWidget *widget , GtkWidget *fontDialog) {
	gtk_widget_destroy(fontDialog);
}

void quit_window(GtkWidget *widget) {
	if(is_modified()) {
		int response;
		GtkWidget *msg_dialog;
		msg_dialog = gtk_message_dialog_new(GTK_WINDOW(e.window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,GTK_BUTTONS_NONE, "Do you Want to save changes before quitting?");
		gtk_dialog_add_buttons(GTK_DIALOG(msg_dialog), GTK_STOCK_YES, GTK_RESPONSE_YES, GTK_STOCK_NO, GTK_RESPONSE_NO, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL); 
		response = gtk_dialog_run(GTK_DIALOG(msg_dialog));
		if(response == GTK_RESPONSE_YES) {
			save(fname);
			gtk_widget_destroy(msg_dialog);
			gtk_main_quit();
			return;
		}
		else if(response == GTK_RESPONSE_NO) {
			gtk_widget_destroy(msg_dialog);
			gtk_main_quit();
			return;
		}
		gtk_widget_destroy(msg_dialog);
		return;
	}
	else {
		gtk_main_quit();
	}
}

void help_info(GtkWidget *widget) {
	GtkWidget *helpWin, *scroll;
	GtkWidget *info;
	char text[8192];
		
	sprintf(text, "MY TEXT EDITOR USING GTK+ 2.0\n\n\nSR.NO.      OPERATION\t\t\t\tMETHOD\n1.\t\t\tOpen a file\t\t\tclick on toolbar icon'open'(ctrl+o)->select file->click open\n2.\t\t\tcreat a new file\t\tclick on toolbar icon'new'(ctrl+t)\n3.\t\t\tSave a file\t\t\t\tclick on toolbar icon'save'(ctrl+s)->select folder->type filename->click save\n4.\t\t\tSave file as..\t\t\tclick on 'save as..' in edit menu(shift+ctrl+s)->select folder->type filename-> click save\n5.\t\t\tQuit window\t\t\tclick on quit in edit menu(ctrl+q)\n6.\t\t\tCut a selection\t\tselect a section->click on cut in edit menu(ctrl+x)\n7.\t\t\tCopy a selection\t\tselect a section->click on copy in edit menu(ctrl+c)\n8.\t\t\tPaste a selection\t\tset the cursor to paste->click on paste in edit menu(ctrl+v)\n9.\t\t\tDelete a selection\tselect a section->click on delete in edit menu(ctrl+d)\n10.\t\t\tSelect All\t\t\t\tclick on select all in edit menu(ctrl+a)\n11.\t\t\tset Preferences\t\tclick on preferences in edit menu->set the font and size -> click apply to check (OR) click OK to confirm\n12.\t\t\tFind text \t\t\t\tclick find in search menu(ctrl+f)->type text in window->click find\n13.\t\t\tFind and Replace\t\t\tclick on find and replace in search  menu(ctrl+h)->type text to search and text to replace->click replace\n14.\t\t\tInsert date and time\t\tset cursor->click on 'insert date and time in tools menu\n15.\t\t\tGo to line\t\t\t\tclick Go to line in tools menu(ctrl+i)->enter line no.->click find\n\n\n\nNote	: Quit the window using quit in file menu or ctrl+q for safe close");
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	
	helpWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(helpWin), "My Editor Using GTK+2.0");
	gtk_window_set_default_size(GTK_WINDOW(helpWin), 400, 400);
	gtk_window_set_position(GTK_WINDOW(helpWin), GTK_WIN_POS_CENTER);

	info = gtk_label_new(text);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow *)scroll, info);
	gtk_container_add(GTK_CONTAINER(helpWin), scroll);	
	gtk_widget_show_all(helpWin);
}

void about_info(GtkWidget *widget) {
	GtkWidget *aboutWin, *info;
	char text[1024];
	sprintf(text, "\t\tMy Text Editor\n\t\tusing GTK+2.0\n\n\nIt is a good looking editor wchich can handle \ncreated by: Shivkumar Raghuram Patil\n\n\n\n ALL RIGHTS ARE RESERVED\n"); 
	aboutWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title(GTK_WINDOW(aboutWin), "My Editor Using GTK+2.0");
	gtk_window_set_default_size(GTK_WINDOW(aboutWin), 400, 400);
	gtk_window_set_position(GTK_WINDOW(aboutWin), GTK_WIN_POS_CENTER);
	info = gtk_label_new(text);
	gtk_container_add(GTK_CONTAINER (aboutWin), info);
	gtk_widget_show_all(aboutWin);
}

