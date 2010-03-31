%{

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
	NODE_TYPE_FUNCTION , //最普遍的，包含的是函数声明
	NODE_TYPE_FUNCTION_BODY , //包含的是函数体
	NODE_TYPE_MACRO , //也很普遍，包含的是 MACRO 宏定义
	NODE_TYPE_STRUCT , //吼吼，包含的是，STRUCT 类型
	NODE_TYPE_UNION  , //联合体
	NODE_TYPE_VAR , //基本类型定义的变量名
	NODE_TYPE_TYPEDEF , // typedef 定义的东西

}SyntaxNodeType;

typedef struct _SyntaxNode{
	SyntaxNodeType type; //类型
	const gchar * syntax; //名称
	GNode	*	  header; //包含它的头文件
	guint		  line; //行号。
	GNode	*	  ret_type; //返回类型
}SyntaxNode;

/**
 * 解释
 *
 * 使用树来存储语法。对于 NODE_TYPE_INCLUDE 类型，叶子就是这个头文件的树
 * 对于 NODE_TYPE_SWITCH ， 会出现两个方向的分支就是2个子树，分别代表 #if 条件编译
 * 成立和不成了的语法树。 NODE_TYPE_FUNCTION 是叶子。包含了符合名和行号，以及包含
 * 这个函数的头文件。NODE_TYPE_FUNCTION_BODY 包含函数体指向。呵呵
 * NODE_TYPE_MACRO 是一个宏
 *
 */


static void auto_complete_scaner_class_init(AutoCompleteScanerClass * );
static void auto_complete_scaner_init(AutoCompleteScaner * );
static void auto_complete_scaner_finalize(GObject *object);

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

#define SCANER_STATE_NONE	0
#define SCANER_STATE_COMMENT	1
#define SCANER_STATE_IN	0
//#define SCANER_STATE_NONE	0
//#define SCANER_STATE_NONE	0

%}

%%

\/\/.*	{printf("%s is 注释\n",yytext);}

\/\*.*\*\/	{printf("/**/类型的注释哦\n");}

\/\*	{ printf("/**/开始！\n"); }
\*\/	{ printf("/**/结束！\n"); }

[\n\t ]+	;

;	{printf("一句结束\n");}

is |
are |
am |
go	{ printf("%s: is a verb\n",yytext);}

[a-zA-Z]+	{  printf("->%s<- unknow\n",yytext); }



%%

int yywrap()
{
	return 1;
}



void auto_complete_scaner_scanheader(AutoCompleteScaner * obj,const gchar * includedfile)
{
	yylex();
}

GType auto_complete_scaner_get_type ()
{
	//d
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

