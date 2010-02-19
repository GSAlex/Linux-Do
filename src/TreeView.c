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
static void gtk_tree_view_dir_dispose(TREEVIEW_DIR*);
static void gtk_tree_view_dir_finalize(TREEVIEW_DIR*);

GType gtk_tree_view_dir_get_type()
{
	static GType	type = 0;

	if (type == 0)
	{
		type = g_type_register_static_simple(GTK_TYPE_TREE_VIEW,"TREEVIEW_DIR",
				sizeof(TREEVIEW_DIRClass),
				(GClassInitFunc)gtk_tree_view_dir_class_init,
				sizeof(TREEVIEW_DIR),
				(GInstanceInitFunc)gtk_tree_view_dir_init,0);
	}
	return type;
}


void gtk_tree_view_dir_init(TREEVIEW_DIR*obj)
{
	obj->cur_dir = g_string_new("");
	obj->col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(obj->col, _("Project View"));
	gtk_tree_view_append_column(GTK_TREE_VIEW(obj), obj->col);

	obj->renderer = gtk_cell_renderer_toggle_new();
	gtk_tree_view_column_pack_start(obj->col, obj->renderer, TRUE);
	gtk_tree_view_column_add_attribute(obj->col, obj->renderer, "text", 0);

	GtkTreeModel* model = GTK_TREE_MODEL(gtk_tree_store_new(1,GTK_TYPE_STRING));
	gtk_tree_view_set_model(GTK_TREE_VIEW(obj), model);
	g_object_unref(model);
}


void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

	klass->update_dir = NULL;
	klass->dispose = gobject_class->dispose;
	klass->finalize = gobject_class->finalize;
	gobject_class->dispose = ( void(*)(GObject*) )gtk_tree_view_dir_dispose;
	gobject_class->finalize = ( void(*)(GObject*) )gtk_tree_view_dir_finalize;
}

void gtk_tree_view_dir_dispose(TREEVIEW_DIR*obj)
{
	GTK_TREE_VIEW_DIR_GET_CLASS(obj)->dispose(G_OBJECT(obj));
}

void gtk_tree_view_dir_finalize(TREEVIEW_DIR*obj)
{
	g_string_free(obj->cur_dir,TRUE);
	GTK_TREE_VIEW_DIR_GET_CLASS(obj)->finalize(G_OBJECT(obj));
}

TREEVIEW_DIR * gtk_tree_view_dir_new()
{
	return GTK_TREE_VIEW_DIR(g_object_new(GTK_TYPE_TREE_VIEW_DIR,"enable-tree-lines",TRUE,0));
}


gboolean gtk_tree_view_dir_set_dir(TREEVIEW_DIR * obj,const gchar * dir)
{
	g_return_val_if_fail(GTK_IS_TREE_VIEW_DIR(obj),FALSE);
	obj->cur_dir = g_string_assign(obj->cur_dir,dir);

}

