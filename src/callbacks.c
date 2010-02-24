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
#include "About.h"
#include "misc.h"

void LinuxDoIDE_show_about_menu_callback(gpointer callback_data, guint callback_action)
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

}

void LinuxDoIDE_save_menu_callback(gpointer callback_data, guint callback_action)
{
    LinuxDoIDE * ide = (LinuxDoIDE*) callback_data;
    GtkNotebook * note = ide->main_layout.mid_layout.code ;

    IDE_EDITOR * editor;

    guint curpage = gtk_notebook_get_current_page(note);

    editor = gtk_notebook_get_editor(note,curpage);

    g_return_if_fail(editor->file);

    ide_editor_savefile(editor,editor->file->str);
}
