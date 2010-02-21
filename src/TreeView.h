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
 * TreeView.h - view dir as tree
 *
 *  Created on: 2010-2-18
 *      Author: cai
 */




#ifndef TREEVIEW_H_
#define TREEVIEW_H_

#include <gtk/gtk.h>

typedef struct _TREEVIEW_DIR TREEVIEW_DIR;
typedef struct _TREEVIEW_DIRClass TREEVIEW_DIRClass;

enum{

	/*
	 * TREEVIEW_DIR_SIGNAL_DBCLICKITEM
	 *
	 * void openfile (TREEVIEW_DIR* obj  ,gchar * item, gpointer userdata)
	 */

	TREEVIEW_DIR_SIGNAL_OPENFILE,


	TREEVIEW_DIRCLASS_MAXSIGNAL
};

#define GTK_TYPE_TREE_VIEW_DIR		(gtk_tree_view_dir_get_type ())
#define GTK_TREE_VIEW_DIR(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TREE_VIEW_DIR, TREEVIEW_DIR))
#define GTK_TREE_VIEW_DIR_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TREE_VIEW_DIR, TREEVIEW_DIRClass))
#define GTK_IS_TREE_VIEW_DIR(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TREE_VIEW_DIR))
#define GTK_IS_TREE_VIEW_DIR_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TREE_VIEW_DIR))
#define GTK_TREE_VIEW_DIR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TREE_VIEW_DIR, TREEVIEW_DIRClass))

struct _TREEVIEW_DIR {
	GtkTreeView parent;
	GString * cur_dir; // current dir
	GtkTreeViewColumn *col;
//	GtkWidget *view;
};

struct _TREEVIEW_DIRClass
{
	GtkTreeViewClass parent_class;
	void (*update_dir)(TREEVIEW_DIR*);
	void (*dispose)(GObject *object);
	void (*finalize)(GObject *object);
	gint				signals[TREEVIEW_DIRCLASS_MAXSIGNAL];


};

GType gtk_tree_view_dir_get_type() G_GNUC_CONST;

TREEVIEW_DIR * gtk_tree_view_dir_new();
gboolean gtk_tree_view_dir_set_dir(TREEVIEW_DIR * obj,const gchar * dir);

#endif /* TREEVIEW_H_ */
