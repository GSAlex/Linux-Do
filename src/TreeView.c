/*
 * TreeVuiew.c
 *
 *  Created on: 2010-2-19
 *      Author: cai
 */

#include "Linuxdo.h"
#include "TreeView.h"

static void gtk_tree_view_dir_init(TREEVIEW_DIR*);
static void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*);

GType gtk_tree_view_dir_get_type()
{
	static GType	type = 0;

	static const GTypeInfo type_info =
	{ sizeof(TREEVIEW_DIRClass), (GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_tree_view_dir_init, NULL, NULL,
			sizeof(TREEVIEW_DIR), 0,
			(GInstanceInitFunc) gtk_tree_view_dir_class_init, };

	if (type == 0)
	{
		type = g_type_register_static(GTK_TYPE_TREE_VIEW,"TREEVIEW_DIR",&type_info,0);
	}
	return type;
}


void gtk_tree_view_dir_init(TREEVIEW_DIR*obj)
{
	obj->cur_dir = g_string_new("");
}


void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*klass)
{

}

TREEVIEW_DIR * gtk_tree_view_dir_new()
{
	g_object_new(GTK_TYPE_TREE_VIEW_DIR,0);
}
