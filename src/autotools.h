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
 * autotools.h - resolve needed things by on disk autotools file -  header file
 *
 *  Created on: 2010-2-23
 *      Author: cai
 */

#ifndef AUTOTOOLS_H_
#define AUTOTOOLS_H_

#include <gtk/gtk.h>

typedef struct _IDE_AUTOTOOLS{
	GObject	parent;
	GString * project_path;
	GString * project_name; // from AC_INIT
	GString * project_version;  // from AC_INIT
	GString * git_ignore; // from .gitignore
}IDE_AUTOTOOLS;

typedef struct _IDE_AUTOTOOLSClass{
	GObjectClass	parent_class;
	void (*dispose)(GObject *object);
	void (*finalize)(GObject *object);

	/*<signals>*/

	guint	configure_resolved_signal;

	/*<handler>*/

	void (*configure_resolved)(IDE_AUTOTOOLS * obj , gpointer userdata);

	/*<munbers>*/

}IDE_AUTOTOOLSClass;

#define IDE_TYPE_AUTOTOOLS		(ide_autotools_get_type ())
#define IDE_AUTOTOOLS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), IDE_TYPE_AUTOTOOLS, IDE_AUTOTOOLS))
#define IDE_AUTOTOOLS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), IDE_TYPE_AUTOTOOLS, IDE_AUTOTOOLSClass))
#define IDE_IS_AUTOTOOLS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), IDE_TYPE_AUTOTOOLS))
#define IDE_IS_AUTOTOOLS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), IDE_TYPE_AUTOTOOLS))
#define IDE_AUTOTOOLS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), IDE_TYPE_AUTOTOOLS, IDE_AUTOTOOLSClass))

GType ide_autotools_get_type () G_GNUC_CONST G_GNUC_INTERNAL;
IDE_AUTOTOOLS * ide_autotools_new() G_GNUC_MALLOC G_GNUC_INTERNAL ;
gboolean ide_autotools_set_configure_ac(IDE_AUTOTOOLS*obj,const gchar * configure_ac_path ) G_GNUC_INTERNAL;
int ide_autotools_create_tmpl(IDE_AUTOTOOLS* obj, const gchar * package_name,gboolean with_nls);

#endif /* AUTOTOOLS_H_ */
