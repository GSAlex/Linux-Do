/*
 * SourceView.h
 *
 *  Created on: 2010-2-20
 *      Author: cai
 */

#ifndef SOURCEVIEW_H_
#define SOURCEVIEW_H_

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagemanager.h>


typedef struct _IDE_EDITOR{
	GtkSourceView	parent;
	GtkSourceBuffer * buffer;
	GString			* file; // non-NULL if associate with a file
	GtkNotebook		* note;
}IDE_EDITOR;

typedef struct _IDE_EDITORClass{
	GtkSourceViewClass	parent_class;
	GtkSourceLanguageManager * lmgr;
	void (*dispose)(GObject *object);
	void (*finalize)(GObject *object);
}IDE_EDITORClass;

#define IDE_TYPE_EDITOR		(ide_editor_get_type ())
#define IDE_EDITOR(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), IDE_TYPE_EDITOR, IDE_EDITOR))
#define IDE_EDITOR_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), IDE_TYPE_EDITOR, IDE_EDITORClass))
#define IDE_IS_EDITOR(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), IDE_TYPE_EDITOR))
#define IDE_IS_EDITOR_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), IDE_TYPE_EDITOR))
#define IDE_EDITOR_CLASS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), IDE_TYPE_EDITOR, IDE_EDITORClass))

GType ide_editor_get_type() G_GNUC_CONST;
IDE_EDITOR* ide_editor_new();

gboolean ide_editor_openfile(IDE_EDITOR * ,const gchar * url);
gboolean ide_editor_savefile(IDE_EDITOR * editor, const gchar * url);
#endif /* SOURCEVIEW_H_ */