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

#include "Linuxdo.h"
#include <glib.h>
#include "scaner.h"

typedef enum _SyntaxNodeType{
	NODE_TYPE_SWITCH =1 , // #if 这样的语法结构
	NODE_TYPE_INCLUDE , //包含的是一个另外被包含的文件的结构指针
	NODE_TYPE_FUNCTION , //最普遍的，包含的是函数
	NODE_TYPE_MACRO , //也很普遍，包含的是 MACRO 宏定义
	NODE_TYPE_STRUCT , //吼吼，包含的是，STRUCT 类型
	NODE_TYPE_UNION  , //联合体
	NODE_TYPE_VAR , //基本类型定义的变量名
	NODE_TYPE_TYPEDEF , // typedef 定义的东西

}SyntaxNodeType;

typedef struct _SyntaxNode{
	SyntaxNodeType type; //类型
	const gchar * syntax; //名称



}SyntaxNode;


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
	klass->system_headers = NULL;
    klass->finalize = G_OBJECT_CLASS(klass)->finalize;
    G_OBJECT_CLASS(klass)->finalize = auto_complete_scaner_finalize;
}

void auto_complete_scaner_init(AutoCompleteScaner * obj)
{
	obj->project_headers = NULL;
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

static void find_same(SyntaxNode * h, gchar ** header)
{
	if (!strcmp(h->syntax, *header))
	{
		*header = NULL;
	}
}

static gboolean header_is_in_list(GList * headers, const gchar * header)
{
	g_list_foreach(headers,(GFunc)find_same,&header);
	return header?FALSE:TRUE;
}

void auto_complete_scaner_scanfile(AutoCompleteScaner * obj,const gchar * includedfile,AutoCompleteScanerHeaderType type)
{
	AutoCompleteScanerClass * klass;

	switch(type)
	{
	case SCAN_TYPE_USER_HEADER:
		//在里头的话就得重新分析，有可能是用户更新了它
		break;
	case SCAN_TYPE_SYSTEM_HEADER:
		klass = AUTO_COMPLETE_SCANER_GET_CLASS(obj);
		//判断是不是已经在分析过的列表里头了。是的话就不用去分析
		if(!header_is_in_list(klass->system_headers,includedfile))
		{



		}
		break;
	}




}
