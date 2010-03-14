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
 * SourceView.h - the master editor
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
#define IDE_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), IDE_TYPE_EDITOR, IDE_EDITORClass))

GType ide_editor_get_type() G_GNUC_CONST G_GNUC_INTERNAL;
IDE_EDITOR* ide_editor_new() G_GNUC_INTERNAL G_GNUC_MALLOC;

gboolean ide_editor_openfile(IDE_EDITOR * ,const gchar * url) G_GNUC_INTERNAL;
gboolean ide_editor_savefile(IDE_EDITOR * editor, const gchar * url) G_GNUC_INTERNAL;
#endif /* SOURCEVIEW_H_ */
