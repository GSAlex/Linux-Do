/*
 * TreeView.h
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
	TREEVIEW_DIR_SIGNAL_DBCLICKITEM,
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
	GtkCellRenderer *renderer;
//	GtkWidget *view;
};

struct _TREEVIEW_DIRClass
{
	GtkTreeViewClass	parent_class;
	gint				signals[TREEVIEW_DIRCLASS_MAXSIGNAL];

};

GType gtk_tree_view_dir_get_type() G_GNUC_CONST;

TREEVIEW_DIR * gtk_tree_view_dir_new();

#endif /* TREEVIEW_H_ */
