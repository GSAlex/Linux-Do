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
 * callbacks.h - call backs for many many widgets
 *
 *  Created on: 2010-2-21
 *      Author: cai
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "ide.h"
#include "SourceView.h"
#include "TreeView.h"

void LinuxDoIDE_show_about_menu_callback(GtkWidget * item , gpointer callback_data) G_GNUC_INTERNAL;
gboolean main_window_on_configure(GtkWidget *widget,	GdkEventConfigure *event, gpointer user_data) G_GNUC_INTERNAL;
void LinuxDoIDE_save_menu_callback(GtkWidget * item ,gpointer callback_data) G_GNUC_INTERNAL;

void LinuxDoIDE_openfile_callback(TREEVIEW_DIR* obj  ,gchar * item, gpointer userdata) G_GNUC_INTERNAL;
void savefile(GtkButton * bt , IDE_EDITOR * editor) G_GNUC_INTERNAL;
#endif /* CALLBACKS_H_ */
