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
#include "autotools.h"

static void gtk_tree_view_dir_init(TREEVIEW_DIR*);
static void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*);
static void gtk_tree_view_dir_dispose(TREEVIEW_DIR*);
static void gtk_tree_view_dir_finalize(TREEVIEW_DIR*);
static void gtk_tree_view_dir_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec);
static void gtk_tree_view_dir_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec);
static void gtk_tree_view_dir_dir_changed(TREEVIEW_DIR * obj);

static void gtk_tree_view_dir_active(GtkTreeView *tree_view,
                                     GtkTreePath *path, GtkTreeViewColumn *column,
                                     gpointer user_data);
static void gtk_tree_view_dir_expanded  (GtkTreeView *tree_view,
										 GtkTreeIter *iter,
										 GtkTreePath *path,
										 gpointer	user_data);
static void gtk_tree_view_dir_row_collapsed  (GtkTreeView *tree_view,
											  GtkTreeIter *iter,
											  GtkTreePath *path,
											  gpointer	user_data);

void gtk_tree_view_dir_settitle(IDE_AUTOTOOLS * obj, gpointer userdata);

enum{
	GTK_TREE_VIEW_DIR_PROP_MGR = 1,
	GTK_TREE_VIEW_DIR_PROP_DIR,
};


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

	gtk_tree_view_column_set_title(obj->col, _("CurrentDir"));

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
	g_signal_connect(G_OBJECT (obj),"test-expand-row",G_CALLBACK(gtk_tree_view_dir_expanded),obj);
	g_signal_connect(G_OBJECT (obj),"row-collapsed",G_CALLBACK(gtk_tree_view_dir_row_collapsed),obj);

}

void gtk_tree_view_dir_class_init(TREEVIEW_DIRClass*klass)
{
	GParamSpec * spec;

	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->set_property = gtk_tree_view_dir_set_property;
	gobject_class->get_property = gtk_tree_view_dir_get_property;

	klass->change_dir = gtk_tree_view_dir_dir_changed;

	klass->dispose = gobject_class->dispose;
	klass->finalize = gobject_class->finalize;
	gobject_class->dispose = ( void(*)(GObject*) )gtk_tree_view_dir_dispose;
	gobject_class->finalize = ( void(*)(GObject*) )gtk_tree_view_dir_finalize;

	klass->signals[TREEVIEW_DIR_SIGNAL_OPENFILE] = g_signal_new("openfile",
			G_TYPE_FROM_CLASS(klass), G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST, 0, 0,
			0, g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1 , G_TYPE_STRING , NULL);

	klass->signals[TREEVIEW_DIR_SIGNAL_CHANGE_DIR] = g_signal_new("changedir",
			G_TYPE_FROM_CLASS(klass), G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(TREEVIEW_DIRClass,change_dir), 0,0,
			g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1 , G_TYPE_STRING , NULL);

	spec = g_param_spec_object("mgr","mgr","autotools mgr",IDE_TYPE_AUTOTOOLS ,G_PARAM_READWRITE);

	g_object_class_install_property(gobject_class,GTK_TREE_VIEW_DIR_PROP_MGR,spec);

	spec = g_param_spec_string("current-dir","dir","the root dir of tree view","." ,G_PARAM_READWRITE);

	g_object_class_install_property(gobject_class,GTK_TREE_VIEW_DIR_PROP_DIR,spec);
}

void gtk_tree_view_dir_dispose(TREEVIEW_DIR*obj)
{
	IDE_AUTOTOOLS * mgr = obj->mgr;
	obj->mgr = NULL;
	if(mgr)
		g_object_unref(mgr);

	GTK_TREE_VIEW_DIR_GET_CLASS(obj)->dispose(G_OBJECT(obj));
}

void gtk_tree_view_dir_finalize(TREEVIEW_DIR*obj)
{
	g_string_free(obj->cur_dir, TRUE);
	GTK_TREE_VIEW_DIR_GET_CLASS(obj)->finalize(G_OBJECT(obj));
}

void gtk_tree_view_dir_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec)
{

	TREEVIEW_DIR *self = GTK_TREE_VIEW_DIR(object);

	switch (property_id) {

	case GTK_TREE_VIEW_DIR_PROP_MGR:

		self->mgr = g_value_dup_object(value);

		g_assert(IDE_IS_AUTOTOOLS(self->mgr));

		g_signal_connect(G_OBJECT(self->mgr),"configure-resolved",G_CALLBACK(gtk_tree_view_dir_settitle),self);

		break;
	case GTK_TREE_VIEW_DIR_PROP_DIR:
		self->cur_dir = g_string_assign(self->cur_dir,g_value_get_string(value));

		g_signal_emit_by_name(object,"changedir",NULL);

	break;
	default:

		/* We don't have any other property... */
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object,property_id,pspec);
	}
}

static void gtk_tree_view_dir_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec)
{

	TREEVIEW_DIR *self = GTK_TREE_VIEW_DIR(object);

	switch (property_id)
	{
	case GTK_TREE_VIEW_DIR_PROP_MGR:
		g_value_set_object(value,self->mgr);
		break;
	default:

		/* We don't have any other property... */
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object,property_id,pspec);
	}
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

	if(!deep)
	{
		gtk_tree_store_append(tree,&cur,root);

		gtk_tree_store_set(tree, &cur,0,GTK_STOCK_DIRECTORY,1,"FAKE",-1);

		return ;
	}

	dir = g_dir_open(dirname,0,NULL);

	if(!dir)
		return ;

	dirs = g_array_new(TRUE,TRUE,sizeof(gchar*));
	files = g_array_new(TRUE,TRUE,sizeof(gchar*));

	//同时要应用 ignore-pattern 的东西，吼吼

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

void gtk_tree_view_dir_dir_changed(TREEVIEW_DIR * obj)
{
	GtkTreeStore * tree;

	gchar * dir = obj->cur_dir->str ;

	tree = gtk_tree_store_new(2,G_TYPE_STRING,GTK_TYPE_STRING);
	//根据设置的pattern加入文件和文件夹 ：）
	append_dir_content(tree,NULL,dir,2);

	gtk_tree_view_set_model(GTK_TREE_VIEW(obj), GTK_TREE_MODEL(tree));
	g_object_unref(tree);
}

gboolean gtk_tree_view_dir_set_dir(TREEVIEW_DIR * obj,const gchar * dir)
{

	g_return_val_if_fail(GTK_IS_TREE_VIEW_DIR(obj),FALSE);

	g_object_set(obj,"current-dir",dir,NULL);
}

void gtk_tree_view_dir_active(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	GtkTreeIter iter;
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
			g_free(value);
		} while (gtk_tree_model_iter_parent(mode, &parent, &iter));

		if(g_file_test(filepath->str,G_FILE_TEST_IS_REGULAR))
		{
			g_signal_emit(G_OBJECT(user_data),
				GTK_TREE_VIEW_DIR_GET_CLASS(user_data)->signals[TREEVIEW_DIR_SIGNAL_OPENFILE],
				0, filepath->str, NULL);
		}

		g_string_free(filepath,TRUE);

	}
}

void gtk_tree_view_dir_expanded (GtkTreeView *tree_view,GtkTreeIter *itr,GtkTreePath *tree_path,gpointer user_data)
{
	TREEVIEW_DIR * tree = GTK_TREE_VIEW_DIR(user_data);

	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;

	model = gtk_tree_view_get_model(tree_view);

	g_return_if_fail(gtk_tree_model_get_iter(model,&iter,tree_path));

	//遍历现有的节点，为每个子节点删除孙节点数据

	GString * path;
	path  = g_string_new("");

	GtkTreeIter parent = *itr;

	//获得绝对路径

	do {

		iter = parent ;

		gtk_tree_model_get(model, &iter, 1, &value, -1);
		if(path->len)
			path = g_string_prepend_c(path, G_DIR_SEPARATOR);
		path = g_string_prepend(path, value);
		g_free(value);
	} while (gtk_tree_model_iter_parent(model, &parent, &iter));

	if(gtk_tree_model_iter_children(model,&iter,itr))
		while(gtk_tree_store_remove(GTK_TREE_STORE(model),&iter));

	//调用 append_dir_content

	append_dir_content(GTK_TREE_STORE(model),itr,path->str,2);

	g_string_free(path,TRUE);
}

void gtk_tree_view_dir_row_collapsed(GtkTreeView *tree_view,GtkTreeIter *itr,GtkTreePath *tree_path,gpointer	user_data)
{
	TREEVIEW_DIR * tree = GTK_TREE_VIEW_DIR(user_data);

	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;

	model = gtk_tree_view_get_model(tree_view);

//	g_return_if_fail(gtk_tree_model_get_iter(model,&iter,tree_path));

	gboolean is_valid;

	//为每个子节点调用 删掉孙节点.

	for (gtk_tree_model_iter_children(model,&iter,itr); is_valid; is_valid = gtk_tree_model_iter_next(model,&iter))
	{
		GtkTreeIter iter_child;

		if(gtk_tree_model_iter_children(model,&iter_child,&iter))
			while(gtk_tree_store_remove(GTK_TREE_STORE(model),&iter_child));

	}
}

void gtk_tree_view_dir_settitle(IDE_AUTOTOOLS * obj, gpointer userdata)
{
	TREEVIEW_DIR * tree_view = (TREEVIEW_DIR*) userdata;

	g_assert(obj->project_name);

	gtk_tree_view_column_set_title(tree_view->col, obj->project_name->str);

}
