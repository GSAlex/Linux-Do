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
 * TreeVuiew.c - view dir as a tree
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
static void gtk_tree_view_dir_active(GtkTreeView *tree_view,
                                     GtkTreePath *path, GtkTreeViewColumn *column,
                                     gpointer user_data);
static void gtk_tree_view_dir_expanded  (GtkTreeView *tree_view,
										 GtkTreeIter *iter,
										 GtkTreePath *path,
										 gpointer	user_data);

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
	GtkCellRenderer * renderer_icon, * renderer_text;
	GtkTreeModel* model;

	GtkWidget * hide_header;

	obj->cur_dir = g_string_new("");
	obj->col = gtk_tree_view_column_new();

	gtk_tree_view_column_set_title(obj->col, _("Project View"));

//	hide_header = gtk_button_new();

//	gtk_widget_set_size_request(hide_header,0,0);

//	gtk_tree_view_column_set_widget(obj->col,hide_header);

	gtk_tree_view_append_column(GTK_TREE_VIEW(obj), obj->col);

	renderer_icon = gtk_cell_renderer_pixbuf_new();
	renderer_text = gtk_cell_renderer_text_new();


	gtk_tree_view_column_pack_start(obj->col, renderer_icon, FALSE);
	gtk_tree_view_column_pack_start(obj->col, renderer_text, FALSE);

	gtk_tree_view_column_add_attribute(obj->col, renderer_icon, "stock-id", 0);
	gtk_tree_view_column_add_attribute(obj->col, renderer_text, "text", 1);

	model = GTK_TREE_MODEL(gtk_tree_store_new(2,GTK_TYPE_STRING,GTK_TYPE_STRING));
	gtk_tree_view_set_model(GTK_TREE_VIEW(obj), model);
	g_object_unref(model);

	g_signal_connect(G_OBJECT (obj),"row-activated",G_CALLBACK(gtk_tree_view_dir_active),obj);
	g_signal_connect(G_OBJECT (obj),"row-expanded",G_CALLBACK(gtk_tree_view_dir_expanded),obj);
}


void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

	klass->update_dir = NULL;
	klass->dispose = gobject_class->dispose;
	klass->finalize = gobject_class->finalize;
	gobject_class->dispose = ( void(*)(GObject*) )gtk_tree_view_dir_dispose;
	gobject_class->finalize = ( void(*)(GObject*) )gtk_tree_view_dir_finalize;
	klass->signals[TREEVIEW_DIR_SIGNAL_OPENFILE] = g_signal_new("openfile",
			GTK_TYPE_TREE_VIEW_DIR, G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST, 0, 0,
			0, g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1 , G_TYPE_STRING , NULL);
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

static void append_dir_content(GtkTreeStore * tree,GtkTreeIter * root , const gchar * dirname,int deep)
{
	const gchar * name ;
	GDir * dir;
	GArray * dirs, * files;
	gchar	*filename;
	GtkTreeIter cur;
	
	if(!deep)return ;

	dir = g_dir_open(dirname,0,NULL);

	g_return_if_fail(dir);

	dirs = g_array_new(TRUE,TRUE,sizeof(gchar*));
	files = g_array_new(TRUE,TRUE,sizeof(gchar*));


	while ((name = g_dir_read_name(dir)))
	{
		filename = g_strdup_printf("%s/%s",dirname,name);
		if (g_file_test(filename, G_FILE_TEST_IS_DIR))
		{
			g_free(filename);
			filename = g_strdup(name);
			dirs = g_array_append_val(dirs, filename);
		}else if( g_file_test(filename, G_FILE_TEST_EXISTS))
		{
			g_free(filename);
			filename = g_strdup(name);
			files = g_array_append_val(files, filename);
		}
		else g_free(filename);
	}

	g_array_sort(dirs,(GCompareFunc)g_strcmp0);
	g_array_sort(files,(GCompareFunc)g_strcmp0);

	//插入目录先
	int i;
	for(i=0;i < dirs->len;++i)
	{
		filename =	((char **)dirs->data)[i];
		gtk_tree_store_append(tree,&cur,root);

		gtk_tree_store_set(tree, &cur,0,GTK_STOCK_DIRECTORY,1,filename,-1);

		gchar * newdir = g_strdup_printf("%s/%s",dirname,filename);

		g_free(filename);
		append_dir_content(tree,&cur,newdir,deep -1 );
		g_free(newdir);

	}
	g_array_free(dirs,TRUE);

	//插入文件，吼吼
	for(i=0;i < files->len;++i)
	{
		filename =	((char **)files->data)[i];
		gtk_tree_store_append(tree,&cur,root);
		gtk_tree_store_set(tree, &cur,0,GTK_STOCK_FILE,1,filename,-1);

		g_free(filename);
	}
	g_array_free(files,TRUE);
	closedir(dir);
}


gboolean gtk_tree_view_dir_set_dir(TREEVIEW_DIR * obj,const gchar * dir)
{
	g_return_val_if_fail(GTK_IS_TREE_VIEW_DIR(obj),FALSE);
	obj->cur_dir = g_string_assign(obj->cur_dir,dir);

	GtkTreeStore * tree;

	tree = gtk_tree_store_new(2,G_TYPE_STRING,GTK_TYPE_STRING);
	//根据设置的pattern加入文件和文件夹 ：）
	append_dir_content(tree,NULL,dir,2);

	gtk_tree_view_set_model(GTK_TREE_VIEW(obj), GTK_TREE_MODEL(tree));
	g_object_unref(tree);

}

void gtk_tree_view_dir_active(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{

	puts(__func__);
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;

	GtkTreeModel * mode;

	mode = gtk_tree_view_get_model(tree_view);

	g_return_if_fail(gtk_tree_model_get_iter(mode,&iter,path));

	if(gtk_tree_model_iter_has_child(mode,&iter))
	{
		gtk_tree_view_expand_row(tree_view,path,0);

	}else
	{
		GString * filepath;
		filepath  = g_string_new("");

		GtkTreeIter parent = iter;


		do {

			iter = parent ;

			gtk_tree_model_get(mode, &iter, 1, &value, -1);
			if(filepath->len)
				filepath = g_string_prepend_c(filepath, G_DIR_SEPARATOR);
			filepath = g_string_prepend(filepath, value);

		} while (gtk_tree_model_iter_parent(mode, &parent, &iter));

		g_signal_emit(G_OBJECT(user_data),
				GTK_TREE_VIEW_DIR_GET_CLASS(user_data)->signals[TREEVIEW_DIR_SIGNAL_OPENFILE],
				0, filepath->str, NULL);

		g_string_free(filepath,TRUE);

	}
}

void gtk_tree_view_dir_expanded (GtkTreeView *tree_view,GtkTreeIter *iter,GtkTreePath *path,gpointer user_data)
{
	TREEVIEW_DIR * tree = GTK_TREE_VIEW_DIR(user_data);
	
	//遍历现有的节点，为每个子节点生成孙节点数据
	
	
	
}