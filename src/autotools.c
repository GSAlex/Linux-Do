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

static void configure_resolved(IDE_AUTOTOOLS * obj , gpointer userdata);

GType ide_autotools_get_type ()
{
    static GType type;
    if ( g_once_init_enter(&type))
    {
		g_once_init_leave(&type,g_type_register_static_simple(G_TYPE_OBJECT,"IDE_AUTOTOOLS",
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

    klass->configure_resolved = configure_resolved;

    klass->configure_resolved_signal =  g_signal_new("configure-resolved",
   			G_TYPE_FROM_CLASS(klass), G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET(IDE_AUTOTOOLSClass,configure_resolved), 0,
   			0, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE,0 , NULL);
}

void ide_autotools_init(IDE_AUTOTOOLS * obj)
{
    obj->project_path = g_string_new("");
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

gboolean ide_autotools_set_configure_ac(IDE_AUTOTOOLS*obj,const gchar * configure_ac_path )
{
    GFile * file,*of;
    gchar * path,*ac;
    //通过指定的文件，自动找到包含 configure.ac 的文件夹，自动设置项目路径到此处。
    file = g_file_new_for_commandline_arg(configure_ac_path);

    if (!g_file_is_native(file))
    {
        g_warning("non-native file\n");
        g_object_unref(file);
        return FALSE ;
    }

    for ( of = NULL ; file ; of = file,file = g_file_get_parent(file),g_object_unref(of))
    {
        //查找当前目录下是否有 configure.ac 文件
        path = g_file_get_path(file);

        ac = g_strdup_printf("%s/configure.ac",path);

        if (g_file_test(ac,G_FILE_TEST_EXISTS))
        {
            g_free(ac);
            g_object_unref(file);
            obj->project_path = g_string_assign(obj->project_path,path);
            g_free(path);

            g_signal_emit_by_name(obj,"configure-resolved",NULL);

            return TRUE;
        }
        g_free(ac);
        g_free(path);
    }

    if(file)
    	g_object_unref(file);

    return FALSE;
}

void configure_resolved(IDE_AUTOTOOLS * obj , gpointer userdata)
{
	puts(__func__);
}
