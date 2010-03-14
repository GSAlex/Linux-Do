/*
 * Editors.c
 *
 *  Created on: 2010-3-15
 *      Author: cai
 */

#include "Linuxdo.h"
#include "Editors.h"

static void gtk_editors_class_init(GTK_EDITORSClass * klass);
static void gtk_editors_init(GTK_EDITORS * obj);
static void gtk_editors_finalize(GObject*);
static gboolean gtk_editors_dbus_method_open(GTK_EDITORS * , gchar * file , GError **);
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
	static const DBusGMethodInfo dbus_glib_linuxdo_methods[] = {
	  { (GCallback) gtk_editors_dbus_method_open , g_marshal_BOOLEAN__STRING_POINTER, 0 },
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

	dbus_g_object_type_install_info( G_TYPE_FROM_CLASS(klass),dbus_glib_linuxdo_object_info);

	guint request_name_result;

	DBusGProxy * bus_proxy =  dbus_g_proxy_new_for_name_owner(klass->dbus_connection,
			DBUS_INTERFACE_DBUS,"/",DBUS_INTERFACE_DBUS,&err);

	dbus_g_proxy_call(bus_proxy, "RequestName", &err,
				G_TYPE_STRING, "org.gtk.LinuxDo.editor",
				G_TYPE_UINT, 0,
				G_TYPE_INVALID,
				G_TYPE_UINT, &request_name_result,
				G_TYPE_INVALID);

	klass->finalize = G_OBJECT_CLASS(klass)->finalize;
	G_OBJECT_CLASS(klass)->finalize = gtk_editors_finalize;
}

void gtk_editors_finalize(GObject*obj)
{
	GTK_EDITORS_GET_CLASS(obj)->finalize(G_OBJECT(obj));
}

void gtk_editors_init(GTK_EDITORS * obj)
{
	DBusGConnection * connection = 	GTK_EDITORS_GET_CLASS(obj)->dbus_connection ;

	dbus_g_connection_register_g_object(connection,"/org/gtk/LinuxDo",G_OBJECT(obj));
}

GTK_EDITORS* gtk_editors_new()
{
	return GTK_EDITORS(g_object_new (GTK_TYPE_EDITORS, NULL));
}

gboolean gtk_editors_dbus_method_open(GTK_EDITORS * obj, gchar * file , GError * * err)
{
	printf("open %s\n",file);
	*err = g_error_new(g_quark_from_string(PACKAGE_NAME),38,"not impl");
	return FALSE;
}

gboolean gtk_editors_dbus_method_close(GTK_EDITORS * obj , gchar * file , GError ** err)
{
	*err = g_error_new(g_quark_from_string(PACKAGE_NAME),-1,"not impl");
	return TRUE;
}
