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
 * autotools.c - scan files for use with autocompletion
 *
 *  Created on: 2010-3-15
 *      Author: cai
 */

#include "../Linuxdo.h"
#include "scaner.h"

static void auto_complete_scaner_class_init(AutoCompleteScanerClass * );
static void auto_complete_scaner_init(AutoCompleteScaner * );
static void auto_complete_scaner_finalize(GObject *object);

GType auto_complete_scaner_get_type ()
{
    static GType type;
    if ( g_once_init_enter(&type))
    {
		g_once_init_leave(&type,g_type_register_static_simple(G_TYPE_OBJECT,"auto_complete_scaner",
                          sizeof(AutoCompleteScanerClass),
                          (GClassInitFunc)auto_complete_scaner_class_init,
                          sizeof(AutoCompleteScaner),
                          (GInstanceInitFunc)auto_complete_scaner_init,0));
    }
    return type;
}

void auto_complete_scaner_class_init(AutoCompleteScanerClass * klass )
{
    klass->finalize = G_OBJECT_CLASS(klass)->finalize;
    G_OBJECT_CLASS(klass)->finalize = auto_complete_scaner_finalize;
}

void auto_complete_scaner_init(AutoCompleteScaner * obj)
{

}

void auto_complete_scaner_finalize(GObject *object)
{
    //chain
    AUTO_COMPLETE_SCANER_GET_CLASS(object)->finalize(object);
}

AutoCompleteScaner * auto_complete_scaner_new()
{
    return AUTO_COMPLETE_SCANER(g_object_new(G_TYPE_AUTOCOMPLETE,NULL));
}
