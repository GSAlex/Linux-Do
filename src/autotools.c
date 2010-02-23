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
 * autotools.c - resolve needed things by on disk autotools file
 *
 *  Created on: 2010-2-23
 *      Author: cai
 */

#include "Linuxdo.h"
#include "autotools.h"

static void ide_autotools_class_init(IDE_AUTOTOOLSClass * );
static void ide_autotools_init(IDE_AUTOTOOLS * );
static void ide_autotools_finalize(GObject *object);
GType ide_autotools_get_type ()
{
    static GType type;
    if ( g_once_init_enter(&type))
    {
        g_once_init_leave(&type,g_type_register_static_simple(GTK_TYPE_TREE_VIEW,"IDE_AUTOTOOLS",
                          sizeof(IDE_AUTOTOOLSClass),
                          (GClassInitFunc)ide_autotools_class_init,
                          sizeof(IDE_AUTOTOOLS),
                          (GInstanceInitFunc)ide_autotools_init,0));
    }
    return type;
}

void ide_autotools_class_init(IDE_AUTOTOOLSClass * klass )
{
	klass->finalize = G_OBJECT_CLASS(klass)->finalize;
	G_OBJECT_CLASS(klass)->finalize = ide_autotools_finalize;
}

void ide_autotools_init(IDE_AUTOTOOLS * obj)
{
	
}

void ide_autotools_finalize(GObject *object)
{
	
	//chain 
	IDE_AUTOTOOLS_GET_CLASS(object)->finalize(object);
}

IDE_AUTOTOOLS * ide_autotools_new()
{
	return IDE_AUTOTOOLS(g_object_new(IDE_TYPE_AUTOTOOLS,NULL));
}

void ide_autotools_set_configure_ac(IDE_AUTOTOOLS*obj,const gchar * configure_ac_path )
{
		
}
