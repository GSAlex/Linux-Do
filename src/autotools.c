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
#include <glib/gutils.h>
#include <string.h>
#include <errno.h>
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
   			G_TYPE_FROM_CLASS(klass), G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST, G_STRUCT_OFFSET(IDE_AUTOTOOLSClass,configure_resolved), 0,
   			0, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE,0 , NULL);
}

void ide_autotools_init(IDE_AUTOTOOLS * obj)
{
    obj->project_path = g_string_new("");
    obj->project_name = g_string_new(_("Unknow"));
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

//Get PACKAGE_NAME and
void configure_resolved(IDE_AUTOTOOLS * obj , gpointer userdata)
{

	GError *	err=NULL;
	GMappedFile * ac;

	const gchar * ac_content ;

	gchar * ac_file = g_strdup_printf("%s" G_DIR_SEPARATOR_S "configure.ac" , 	obj->project_path->str);

	ac = g_mapped_file_new(ac_file,FALSE,&err);

	g_free(ac_file);

	if(!ac)
	{
		g_signal_stop_emission_by_name(obj,"configure-resolved");
		g_warning("configure.ac can't open\n");
		return;
	}

	ac_content = g_mapped_file_get_contents(ac);

	char name[80]={""};

//	sscanf(ac_content,"AC_INIT(\\[%[^\\]^\\n]\\],)\n",name);

	gchar * ac_init = strstr(ac_content,"AC_INIT(");

	sscanf(ac_init,"AC_INIT(%[^,]\n",name);

	gchar * look_define = g_strdup_printf("m4_define(%s",name);

	gchar * real_package_name = strstr(ac_content,look_define);

	g_free(look_define);

	if(real_package_name)
	{
//		puts("got real ");
		sscanf(real_package_name,"m4_define(%*[^,],%[^)])\n",name);
	}

	g_mapped_file_unref(ac);

//	g_print("got %s\n",name);

//	strip_name(name);

	//只选[]中间的东西

//	g_print("got %s\n",name);

	obj->project_name = g_string_assign(obj->project_name,name);
}

int ide_autotools_create_tmpl(IDE_AUTOTOOLS* obj, const gchar * package_name,gboolean with_nls)
{
	int	ret;
	FILE * am;
	FILE * ac;
	FILE * po_LINGUAS, * po_makebars;
	//建立 configure
	ac = fopen("configure.ac","w");

	if(!ac)
	{
		g_error(_("can not create file configure.ac : %s"), g_strerror(errno));
	}
	// 获取项目名字
	if(!package_name)
	{
		package_name = g_new0(char,256);
		printf("Please input the name of the project:");
		g_assert(scanf("%s",(gchar*)package_name)==1);
	}

	//建立标准目录结构

	mkdir("m4",0777);
	mkdir("src",0777);
	mkdir("doc",0777);
	mkdir("icons",0777);
	if(with_nls)
		mkdir("po",0777);

	//建立标准 configure.ac

	fprintf(ac,"\n# %s - %s",package_name,"PACKAGE_DESCRIBTION");
	fprintf(ac,
			"\n\n# This program is free software; you can redistribute it and/or modify\n"
			"# it under the terms of the GNU General Public License as published by\n"
			"# the Free Software Foundation; either version 2, or (at your option)\n"
			"# any later version.\n#\n"
			"# This program is distributed in the hope that it will be useful,\n"
			"# but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"# GNU General Public License for more details.\n#\n"
			"# You should have received a copy of the GNU General Public License\n"
			"# along with this program; if not, write to the Free Software\n"
			"# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n\n");

	fprintf(ac,"m4_define([package_name], [%s])\n\n",package_name);
	fprintf(ac,"AC_INIT([package_name], [%s], [%s], [package_name])\n","0.0.1","BUG REPORT URL");
	fprintf(ac,"AM_INIT_AUTOMAKE([-Wall])\n");
	fprintf(ac,"# AC_GNU_SOURCE\n\nAC_CONFIG_HEADERS([config.h])\nAC_CONFIG_MACRO_DIR([m4])\n\n");
	fprintf(ac, "AC_PROG_CC\nAC_HEADER_STDC\n\n%cAC_PREFIX_DEFAULT(/usr)\n\n",with_nls?'\n':'#');

	if (with_nls)
	{
		fprintf(ac,
				"# define GETTEXT_* variables\n"
				"GETTEXT_PACKAGE=\"$PACKAGE_NAME\"\n"
				"AC_SUBST(GETTEXT_PACKAGE)\n"
				"AC_SUBST(prefix)\n"
				"AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,\"$GETTEXT_PACKAGE\", [Define to the read-only architecture-independent data directory.])\n"
				"AM_GNU_GETTEXT([external])\n"
				"AM_GNU_GETTEXT_VERSION(0.17.0)\n\n");
	}

	fprintf(ac,
			"# OUTPUT files\n"
			"AC_CONFIG_FILES(%s"
			"src/Makefile\n"
			"icons/Makefile\n"
			"doc/Makefile\n"
			"Makefile\n)\n\n"
			"AC_OUTPUT\n", with_nls?"po/Makefile.in\n":"");

	fclose(ac);

	// Makefile.am

	am = fopen("Makefile.am","w");

	fprintf(am,"EXTRA_DIST = m4 \n\n");

	fprintf(am,"SUBDIRS  = icons src doc %s\n\n",with_nls?"po":"");

	fclose(am);

	// icons/Makefile

	am = fopen("icons/Makefile.am","w");

	fclose(am);

	//doc/Makefile

	am = fopen("doc/Makefile.am","w");

	fclose(am);

	// src/Makefile

	am = fopen("src/Makefile.am","w");

	fclose(am);

	if(with_nls)
	{
		//po/LINGUAS

		po_LINGUAS = fopen("po/LINGUAS", "w");

		fclose(po_LINGUAS);

		//po/Makevars

		po_makebars = fopen("po/Makevars", "w");

		fprintf(po_makebars, "DOMAIN = $(PACKAGE)\n"
			"# These two variables depend on the location of this directory.\n"
			"subdir = po\n"
			"top_builddir = ..\n\n"
			"# These options get passed to xgettext.\n"
			"XGETTEXT_OPTIONS = --keyword=_ --keyword=N_\n");

		fprintf(po_makebars,
				"COPYRIGHT_HOLDER = Free Software Foundation, Inc.\n");
		fprintf(po_makebars, "MSGID_BUGS_ADDRESS =\n");
		fprintf(po_makebars, "EXTRA_LOCALE_CATEGORIES =\n");

		fclose(po_makebars);

		//po/POTFILES.in

		FILE * pots = fopen("po/POTFILES.in","a");

		fclose(pots);

	}

	FILE * autogen = fopen("autogen.sh","w");

	fprintf(autogen,"#!/bin/sh\n"
			"set -e\n"
			"set -x\n");

	if(with_nls)
	{
		fprintf(autogen,"autopoint\n#libtoolize --automake --copy\n");
	}

	fprintf(autogen,"aclocal -I m4\n"
			"autoheader\n"
			"automake --add-missing --copy\n"
			"autoconf\n"
			"export CFLAGS=\"-g -O0 -DDEBUG\"\n"
			"export CXXFLAGS=\"$CFLAGS\"");

	fclose(autogen);

	chmod("autogen.sh",0755);

	//调用 git init

	ret = system("git init");
	ret = system("git add .");
	//加入 .gitignore
	FILE * gitignore = fopen(".gitignore","a");

	fputs(".*\n*~\n*.[oa]\n*.la\n*.lo\n*.k*\n*.out\nMakefile\nMakefile.in\n",gitignore);
	fputs("config*\n!configure.ac\ndepcomp\nautom4*\n",gitignore);
	fputs("install\nmissing\n*stamp*\n*.pot\n*.gmo\n*.mo\n",gitignore);
	fputs("po/*.sed\npo/Makefile*\npo/*.temp*\npo/en*.header\npo/*.sin\n",gitignore);
	fputs("po/Rules-quot\n",gitignore);

	fclose(gitignore);

	ret = system("git add -f .gitignore");

	ret = system("git commit -a -m 'Init project with LinuxDo'");

	return ret;
}
