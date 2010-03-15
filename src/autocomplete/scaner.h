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
 * scaner.h - scan files
 *
 *  Created on: 2010-3-15
 *      Author: cai
 */

#ifndef AUTO_COMPLETE_SCANER_H_
#define AUTO_COMPLETE_SCANER_H_

#include <glib.h>
#include <glib-object.h>

typedef struct _IDE_AUTOTOOLS{
	GObject	parent;
}AutoCompleteScaner;


typedef struct _IDE_AUTOTOOLSClass{
	GObjectClass	parent_class;
	void (*dispose)(GObject *object);
	void (*finalize)(GObject *object);

	/*<signals>*/


	/*<handler>*/

	/*<munbers>*/

}AutoCompleteScanerClass;

#define G_TYPE_AUTOCOMPLETE		(auto_complete_scaner_get_type ())
#define AUTO_COMPLETE_SCANER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_AUTOCOMPLETE, AutoCompleteScaner))
#define AUTO_COMPLETE_SCANER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), G_TYPE_AUTOCOMPLETE, AutoCompleteScanerClass))
#define G_YTPE_IS_AUTO_COMPLETE_SCANER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_AUTOCOMPLETE))
#define G_YTPE_IS_AUTO_COMPLETE_SCANER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), G_TYPE_AUTOCOMPLETE))
#define AUTO_COMPLETE_SCANER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), G_TYPE_AUTOCOMPLETE, AutoCompleteScanerClass))

GType auto_complete_scaner_get_type () G_GNUC_CONST G_GNUC_INTERNAL;
AutoCompleteScaner * auto_complete_scaner_new() G_GNUC_MALLOC G_GNUC_INTERNAL ;

#endif /* AUTO_COMPLETE_SCANER_H_ */
