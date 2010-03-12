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
 * xterm.h - pseudo terminals emulation
 *
 *  Created on: 2010-3-2
 *      Author: cai
 */

#ifndef XTERM_H_
#define XTERM_H_

#include <gtk/gtk.h>
#include <vte/vte.h>

typedef struct _GtkXterm
{
	VteTerminal parent;

} GtkXterm;

typedef struct _GtkXtermClass
{
	VteTerminalClass parent_class;
	void (*finalize)(GObject *object);
} GtkXtermClass;

#define GTK_TYPE_XTERM		(gtk_xterm_get_type ())
#define GTK_XTERM(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_XTERM, GtkXterm))
#define GTK_XTERM_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_XTERM, GtkXtermClass))
#define GTK_IS_XTERM(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_XTERM))
#define GTK_IS_XTERM_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_XTERM))
#define GTK_XTERM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_XTERM, GtkXtermClass))


GType gtk_xterm_get_type() G_GNUC_CONST ;
GtkWidget * gtk_xterm_new();

#endif /* XTERM_H_ */
