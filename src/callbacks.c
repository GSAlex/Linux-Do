/*
 *  Linux Do - A new IDE for newbee
 *
 * Copyright (c) 2010-2010 microcai  <microcai@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * callbacks.c - call backs for many many widgets
 *
 *  Created on: 2010-2-21
 *      Author: cai
 */

#include "Linuxdo.h"
#include "callbacks.h"
#include "Editors.h"
#include "About.h"
#include "misc.h"
#include "xterm.h"

void LinuxDoIDE_show_about_menu_callback(GtkWidget * item , gpointer callback_data)
{
    LinuxDoIDE * ide = callback_data;
    LinuxDoIDE_show_about(GTK_WIDGET(ide->main_window));
}

gboolean main_window_on_configure(GtkWidget *widget,	GdkEventConfigure *event, gpointer user_data)
{
//	puts(__func__);
    LinuxDoIDE * ide = (LinuxDoIDE*) user_data;

    // 吼吼，更复杂都可以的啦

    gtk_paned_set_position(ide->main_layout.right,event->width-220);
    gtk_paned_set_position(ide->main_layout.midlayout,event->height-140);

    return FALSE;
}

void LinuxDoIDE_save_menu_callback(GtkWidget * item ,gpointer callback_data)
{
    LinuxDoIDE * ide = (LinuxDoIDE*) callback_data;
    GTK_EDITORS * note = ide->main_layout.mid_layout.code ;

    GeditView * editor;

    guint curpage = gtk_notebook_get_current_page(GTK_NOTEBOOK(note));

    editor = gtk_notebook_get_editor(note,curpage);




}

void LinuxDoIDE_openfile_callback(TREEVIEW_DIR* obj  ,gchar * item, gpointer userdata)
{
	LinuxDoIDE * ide = userdata;

	g_return_if_fail(g_file_test(item,G_FILE_TEST_IS_REGULAR));

	GTK_EDITORS * note = ide->main_layout.mid_layout.code;

	gtk_editors_open(note,item,NULL);
}

void savefile(GtkButton * bt , GeditView * editor)
{
//	gtk_notebook_remove_page(GTK_NOTEBOOK(editor->note),gtk_notebook_page_num(GTK_NOTEBOOK(editor->note),gtk_widget_get_parent(GTK_WIDGET(editor))));
}

void LinuxDoIDE_menu_build_cb(GtkWidget * item ,gpointer callback_data)
{
    LinuxDoIDE * ide = (LinuxDoIDE*) callback_data;

    VteTerminal *vte = VTE_TERMINAL(ide->main_layout.mid_layout.console);

    vte_terminal_feed_child(vte,"make\n",sizeof("make\n"));
}
