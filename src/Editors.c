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
 * Editors.c
 *
 *  Created on: 2010-3-15
 *      Author: cai
 */
#include <errno.h>
#include "Linuxdo.h"
#include "Editors.h"
#include "SourceView.h"
#include "callbacks.h"

static void gtk_editors_class_init(GTK_EDITORSClass * klass);
static void gtk_editors_init(GTK_EDITORS * obj);
static void gtk_editors_finalize(GObject*);
static gboolean gtk_editors_dbus_method_close(GTK_EDITORS * , gchar * file , GError **);

GType gtk_editors_get_type()
{
	static volatile GType type = 0;

	static GTypeInfo typeinfo = { sizeof(GTK_EDITORSClass),
			(GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_editors_class_init, 0, NULL,
			sizeof(GTK_EDITORS), 0, (GInstanceInitFunc) gtk_editors_init, };

	if(g_once_init_enter(&type))
	{
		g_once_init_leave(&type,g_type_register_static(GTK_TYPE_NOTEBOOK,"GTK_EDITORS",&typeinfo,0));
	}
	return type;
}


void gtk_editors_class_init(GTK_EDITORSClass * klass)
{
	klass->finalize = G_OBJECT_CLASS(klass)->finalize;
	G_OBJECT_CLASS(klass)->finalize = gtk_editors_finalize;

	static const DBusGMethodInfo dbus_glib_linuxdo_methods[] = {
	  { (GCallback) gtk_editors_open , g_marshal_BOOLEAN__STRING_POINTER, 0 },
	  { (GCallback) gtk_editors_dbus_method_close , g_marshal_BOOLEAN__STRING_POINTER, 44 },
	};

	static const DBusGObjectInfo dbus_glib_linuxdo_object_info[1] = {
			  0,
			  dbus_glib_linuxdo_methods,
			  2,
			"org.gtk.LinuxDo.editor\0Open\0S\0filename\0I\0s\0\0org.gtk.LinuxDo.editor\0Close\0S\0filename\0I\0s\0\0\0",
			"\0\0org.gtk.LinuxDo.editor\0Changed\0\0",
			"\0"
			};

	GError * err = NULL;

	//查看并建立 dbus 服务
	klass->dbus_connection = dbus_g_bus_get(DBUS_BUS_SESSION,&err);

	if(err)
	{
		g_warning("Unable to connetct to dbus");
		g_error_free(err);
		return ;
	}

	dbus_g_object_type_install_info( G_TYPE_FROM_CLASS(klass),dbus_glib_linuxdo_object_info);

	guint request_name_result;

	DBusGProxy * dbus_proxy =  dbus_g_proxy_new_for_name_owner(klass->dbus_connection,
			DBUS_SERVICE_DBUS,DBUS_PATH_DBUS,DBUS_INTERFACE_DBUS,&err);

	dbus_g_proxy_call(dbus_proxy, "RequestName", &err,
				G_TYPE_STRING, "org.gtk.LinuxDo.editor",
				G_TYPE_UINT, 0,
				G_TYPE_INVALID,
				G_TYPE_UINT, &request_name_result,
				G_TYPE_INVALID);

	g_object_unref(dbus_proxy);
}

void gtk_editors_finalize(GObject*obj)
{
	DBusGConnection * connection = 	GTK_EDITORS_GET_CLASS(obj)->dbus_connection;

	puts(__func__);

	if(connection)
//		dbus_g_connection_unregister_g_object(connection,obj);

	GTK_EDITORS_GET_CLASS(obj)->finalize(G_OBJECT(obj));
}

void gtk_editors_init(GTK_EDITORS * obj)
{
	DBusGConnection * connection = 	GTK_EDITORS_GET_CLASS(obj)->dbus_connection ;

	g_return_if_fail(connection);

	dbus_g_connection_register_g_object(connection,"/org/LinuxDo",G_OBJECT(obj));
}

GTK_EDITORS* gtk_editors_new()
{
	return GTK_EDITORS(g_object_new (GTK_TYPE_EDITORS, NULL));
}

gboolean gtk_editors_open(GTK_EDITORS * note, gchar * file , GError * * err)
{
	IDE_EDITOR * source_editor;

	if(g_file_test(file,G_FILE_TEST_IS_REGULAR))
	{

	source_editor = gtk_editors_create_page(note,file);

	ide_editor_openfile(source_editor,file);
	return TRUE;
	}
	if(err)
		*err = g_error_new(g_quark_from_string(PACKAGE_NAME),EINVAL,"not regular file");
	return FALSE;
}

gboolean gtk_editors_dbus_method_close(GTK_EDITORS * obj , gchar * file , GError ** err)
{
	*err = g_error_new(g_quark_from_string(PACKAGE_NAME),38,"not impl");
	return FALSE;
}

IDE_EDITOR * gtk_notebook_get_editor(GTK_EDITORS * note, guint nth)
{
    GList * list;
    IDE_EDITOR * editor;

    GtkWidget * curpage = gtk_notebook_get_nth_page(GTK_NOTEBOOK(note),nth);

    list = gtk_container_get_children(GTK_CONTAINER(curpage));

    editor = IDE_EDITOR(g_list_first(list)->data);

    g_list_free(list);

    return editor;
}


IDE_EDITOR * gtk_editors_create_page(GTK_EDITORS* note, const gchar * label)
{
	IDE_EDITOR * source_editor;

	source_editor = ide_editor_new();

	source_editor->note = note;

	GtkWidget * scroll = gtk_scrolled_window_new(NULL,NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(scroll), GTK_WIDGET(source_editor));

	GtkWidget * title = gtk_hbox_new(0,0);

	GtkWidget * bt = GTK_WIDGET(gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE));

	gtk_widget_set_size_request(bt,18,18);

	gtk_box_pack_start(GTK_BOX(title),gtk_label_new_with_mnemonic(label),TRUE,0,0);

	gtk_box_pack_start(GTK_BOX(title),bt,TRUE,0,0);

	gtk_widget_show_all(title);

	gtk_notebook_append_page(GTK_NOTEBOOK(note),GTK_WIDGET(scroll),title);

	gtk_widget_show_all(GTK_WIDGET(source_editor));

	gtk_widget_show_all(GTK_WIDGET(gtk_notebook_get_nth_page(GTK_NOTEBOOK(note),gtk_notebook_get_n_pages(GTK_NOTEBOOK(note))-1)));

	gtk_notebook_set_current_page(GTK_NOTEBOOK(note),gtk_notebook_get_n_pages(GTK_NOTEBOOK(note))-1);

	g_signal_connect(G_OBJECT(bt),"clicked",G_CALLBACK(savefile),source_editor);

	return source_editor;
}
