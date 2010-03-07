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
 * xterm.c - pseudo terminals emulation
 *
 *  Created on: 2010-3-2
 *      Author: cai
 */

#include "Linuxdo.h"
#include "xterm.h"

static void gtk_xterm_class_init(GtkXtermClass * klass);
static void gtk_xterm_init(GtkXterm * obj);
static void gtk_xterm_finalize(GObject*);
static void gtk_xterm_on_child_exit(GtkXterm *, gchar *text,guint size, gpointer user_data);


GType gtk_xterm_get_type()
{
	static volatile GType type = 0;

	static GTypeInfo typeinfo = { sizeof(GtkXtermClass),
			(GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_xterm_class_init, 0, NULL,
			sizeof(GtkXterm), 0, (GInstanceInitFunc) gtk_xterm_init, };

	if(g_once_init_enter(&type))
	{
		g_once_init_leave(&type,g_type_register_static(VTE_TYPE_TERMINAL,"GTK_XTERM",&typeinfo,0));
	}
	return type;
}

void gtk_xterm_class_init(GtkXtermClass * klass)
{
	klass->finalize = G_OBJECT_CLASS(klass)->finalize;
	G_OBJECT_CLASS(klass)->finalize = gtk_xterm_finalize;
}

void gtk_xterm_init(GtkXterm * obj)
{
	g_signal_connect(G_OBJECT (obj),"child-exited",G_CALLBACK(gtk_xterm_on_child_exit),obj);
	vte_terminal_fork_command(VTE_TERMINAL(obj),NULL,NULL,NULL,NULL,FALSE,FALSE,FALSE);
}

void gtk_xterm_finalize(GObject*obj)
{
	GTK_XTERM_GET_CLASS(obj)->finalize(obj);
}

void gtk_xterm_on_child_exit(GtkXterm *obj, gchar *text,guint size, gpointer user_data)
{
	vte_terminal_fork_command(VTE_TERMINAL(obj),NULL,NULL,NULL,NULL,FALSE,FALSE,FALSE);

}

GtkWidget * gtk_xterm_new()
{
	return g_object_new(GTK_TYPE_XTERM,"encoding","UTF-8",NULL);
}
