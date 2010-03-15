/*
 * Editors.h
 *
 *  Created on: 2010-3-15
 *      Author: cai
 */

#ifndef EDITORS_H_
#define EDITORS_H_

#include <gtk/gtk.h>

#include "SourceView.h"

struct _GTK_EDITORS{
	GtkNotebook	parent;
};

typedef struct _GTK_EDITORSClass{
	GtkNotebookClass	parent_class;
	DBusGConnection	* 	dbus_connection;
	void (*dispose)(GObject *object);
	void (*finalize)(GObject *object);
}GTK_EDITORSClass;

#define GTK_TYPE_EDITORS	(gtk_editors_get_type ())
#define GTK_EDITORS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_EDITORS, GTK_EDITORS))
#define GTK_EDITORS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_EDITORS, GTK_EDITORSClass)))
#define GTK_IS_EDITORS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_EDITORS))
#define GTK_IS_EDITORS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_EDITORS))
#define GTK_EDITORS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_EDITORS, GTK_EDITORSClass))

GType gtk_editors_get_type() G_GNUC_CONST G_GNUC_INTERNAL;
GTK_EDITORS* gtk_editors_new() G_GNUC_INTERNAL G_GNUC_MALLOC;
gboolean gtk_editors_open(GTK_EDITORS * , gchar * file , GError **) G_GNUC_INTERNAL ;

IDE_EDITOR * gtk_editors_create_page(GTK_EDITORS* note, const gchar * label) G_GNUC_INTERNAL ;
IDE_EDITOR * gtk_notebook_get_editor(GTK_EDITORS * note, guint nth) G_GNUC_INTERNAL ;

#endif /* EDITORS_H_ */
