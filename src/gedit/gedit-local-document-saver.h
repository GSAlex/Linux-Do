/*
 * gedit-local-document-saver.h
 * This file is part of gedit
 *
 * Copyright (C) 2005 - Paolo Maggi 
 * Copyrhing (C) 2007 - Paolo Maggi, Steve Frécinaux
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA.
 */
 
/*
 * Modified by the gedit Team, 2005-2007. See the AUTHORS file for a
 * list of people on the gedit Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifndef __GEDIT_LOCAL_DOCUMENT_SAVER_H__
#define __GEDIT_LOCAL_DOCUMENT_SAVER_H__

#include <gedit/gedit-document-saver.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GEDIT_TYPE_LOCAL_DOCUMENT_SAVER              (gedit_local_document_saver_get_type())
#define GEDIT_LOCAL_DOCUMENT_SAVER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), GEDIT_TYPE_LOCAL_DOCUMENT_SAVER, GeditLocalDocumentSaver))
#define GEDIT_LOCAL_DOCUMENT_SAVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), GEDIT_TYPE_LOCAL_DOCUMENT_SAVER, GeditLocalDocumentSaverClass))
#define GEDIT_IS_LOCAL_DOCUMENT_SAVER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), GEDIT_TYPE_LOCAL_DOCUMENT_SAVER))
#define GEDIT_IS_LOCAL_DOCUMENT_SAVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GEDIT_TYPE_LOCAL_DOCUMENT_SAVER))
#define GEDIT_LOCAL_DOCUMENT_SAVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), GEDIT_TYPE_LOCAL_DOCUMENT_SAVER, GeditLocalDocumentSaverClass))

typedef struct _GeditLocalDocumentSaverPrivate GeditLocalDocumentSaverPrivate;

/*
 * Main object structure
 */
typedef struct _GeditLocalDocumentSaver GeditLocalDocumentSaver;

struct _GeditLocalDocumentSaver
{
	GeditDocumentSaver saver;

	/*< private > */
	GeditLocalDocumentSaverPrivate *priv;
};

/*
 * Class definition
 */
typedef GeditDocumentSaverClass GeditLocalDocumentSaverClass;

/*
 * Public methods
 */
GType 		 	 gedit_local_document_saver_get_type	(void) G_GNUC_CONST;

G_END_DECLS

#endif  /* __GEDIT_DOCUMENT_SAVER_H__  */
