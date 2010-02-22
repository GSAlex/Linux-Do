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
 * About.c - show about dialog
 *
 *  Created on: 2010-2-19
 *      Author: cai
 */

#include "Linuxdo.h"
#include "About.h"
#include "misc.h"

void LinuxDoIDE_show_about(GtkWidget* parent)
{
    static const gchar* authors[3]= { "microcai" , "alex" , 0 } ;
    GtkWidget * dialog;


    gtk_widget_disable(parent);
    dialog = GTK_WIDGET(g_object_new(GTK_TYPE_ABOUT_DIALOG,"version" , PACKAGE_VERSION , "authors" ,authors  , "website" , "http://github.com/GSAlex/Linux-Do" , NULL ,NULL));
//	gtk_widget_set_parent(dialog,parent);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_enable(parent);
    gtk_widget_destroy(dialog);
}
