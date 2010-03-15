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
 * main.c - main source file
 */


#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "Linuxdo.h"
#include <glib/gstdio.h>
#include "TreeView.h"
#include "autotools.h"
#include "SourceView.h"
#include "Editors.h"
#include "ide.h"
#include "callbacks.h"
#include "misc.h"
#include "xterm.h"

#include "../icons/LinuxDo.icon.h"

static void build_ui(LinuxDoIDE * ide);
static void ide_window_set_title(IDE_AUTOTOOLS * , GtkWindow * window );
static void close_tab(gpointer callback_data, guint callback_action)
{
	LinuxDoIDE * ide = callback_data;
	GtkNotebook * note = GTK_NOTEBOOK(ide->main_layout.mid_layout.code);

	IDE_EDITOR * editor;

	if (gtk_notebook_get_n_pages(note) <= 1)
		return;
	guint cur = gtk_notebook_get_current_page(note);

	editor = gtk_notebook_get_editor(GTK_EDITORS(note),cur);

	ide_editor_savefile(editor,editor->file->str);

	gtk_notebook_remove_page(note,cur);
}

static void connect_signals(LinuxDoIDE * ide)
{
	// resize
	g_signal_connect(G_OBJECT(ide->main_window),"configure-event",G_CALLBACK(main_window_on_configure),ide);
	g_signal_connect(G_OBJECT (ide->main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (ide->main_layout.left_layout.tree), "openfile", G_CALLBACK(LinuxDoIDE_openfile_callback), ide);
}

static int create_std_autotools_project(const gchar * package_name,gboolean with_nls)
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

int main(int argc, char * argv[])
{
	LinuxDoIDE ide;

	gboolean init_project=FALSE;
	GError * err=NULL;
	gchar * basedir=NULL;
	gchar * package_name=NULL;

	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);
#ifdef G_OS_WIN32
	{
		char realpath_str[1024];
		bindtextdomain(GETTEXT_PACKAGE,realpath(argv[0],realpath_str));
	}
#endif
	g_type_init();

	GOptionEntry args[] =
	{
			{"init-project",'\0',0,G_OPTION_ARG_NONE,&init_project,_("do git init and build initial dir struct for use with autotools")},
			{"package-name",'\0',0,G_OPTION_ARG_STRING,&package_name,_("Valid when emit --init-project, specify the name of the project"),"name"},
			{"root",'\0',0,G_OPTION_ARG_STRING,&basedir,_("set project root dir"), N_("dir")},
			{0}
	};

	GFile * file = NULL;

	g_set_prgname(PACKAGE_NAME);

	if(G_UNLIKELY(!gtk_init_with_args(&argc, &argv,PACKAGE_STRING,args,NULL,&err)))
	{
		GOptionContext * context;
		context = g_option_context_new("");
		g_option_context_add_main_entries(context,args,PACKAGE_NAME);
		g_option_context_parse(context,&argc,&argv,NULL);
		g_option_context_free(context);
		//没有打开 DISPLAY,但是，其实在控制台下是可以允许 --init-project 的
		if(G_LIKELY(!init_project))
		{
			g_error("%s",err->message);
		}
	}

	g_set_application_name(_(PACKAGE_NAME));

	if(G_UNLIKELY(init_project))
	{
		if (basedir)
		{
			if (g_chdir(basedir))
			{
				g_error(_("can not change to dir %s"),basedir);
			}
			g_free(basedir);
		}

		return create_std_autotools_project(package_name,TRUE);
	}

	printf(_("Linux-DO start up\n"));

	ide.project_mgr = ide_autotools_new();

	build_ui(&ide);

	connect_signals(&ide);

	g_object_set(ide.main_layout.left_layout.tree,"mgr",ide.project_mgr,NULL);

	g_signal_connect(G_OBJECT(ide.project_mgr),"configure-resolved",G_CALLBACK(ide_window_set_title),ide.main_window);

	if(argc==2)
	{
		file = g_file_new_for_path(argv[1]);
	}

	if(basedir)
	{
		g_chdir(basedir);
		g_free(basedir);
	}

	ide_autotools_set_configure_ac(ide.project_mgr,argc==2?argv[1]:".");

	if(!basedir)
	{
		g_chdir(ide.project_mgr->project_path->str);
	}

	g_object_set(ide.main_layout.left_layout.tree,"current-dir",".",NULL);

	if(file)
	{
		//打开文件，吼吼

		GFile * pwd = g_file_new_for_path(".");

		gchar * path = g_file_get_relative_path(pwd, file);

		g_object_unref(pwd);

		g_object_unref(file);

		gtk_editors_open(ide.main_layout.mid_layout.code,path,NULL);

		g_free(path);
	}

	gtk_main();
	return 0;
}

static void build_ui(LinuxDoIDE * ide)
{
	GError * err = NULL;
	//build main window
	ide->main_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	static const gchar menubar [] = {
			"<ui>"
			  "<menubar name=\"MenuBar\" >"
			    "<menu name=\"FileMenu\" action = \"FileMenu\" >"
					"<menuitem action = \"FileNew\" />"
					"<menuitem action = \"FileOpen\" />"
					"<separator/>"
					"<menuitem action = \"FileClose\" />"
					"<menuitem action = \"FileCloseAll\" />"
					"<separator/>"
					"<menuitem action = \"FileSave\" />"
					"<menuitem action = \"FileSaveAs\" />"
					"<menuitem action = \"FileQuit\" />"
			    "</menu>"
			    "<menu name=\"ProjectMenu\" action=\"ProjectMenu\">"
			    "	<menuitem action = \"ProjectBuild\" />"
			    "</menu>"
				"<menu name=\"HelpMenu\" action = \"HelpMenu\" >"
					"<menuitem action = \"HelpAbout\" />"
				"</menu>"
			  "</menubar>"
			  "<toolbar name=\"TOOLBAR\">"
				  "<toolitem action=\"FileNew\"/>"
				  "<toolitem action=\"FileOpen\"/>"
				  "<toolitem action=\"FileClose\"/>"
				  "<separator/>"
				  "<toolitem action=\"ProjectBuild\"/>"
				  "<separator/>"
				  "<toolitem action=\"FileQuit\"/>"
			  "</toolbar>"
			"</ui>"
	};

	const GtkActionEntry actions[] = {
			{"FileMenu" , GTK_STOCK_FILE, _("_File") },
			{ "FileNew", GTK_STOCK_NEW, _("_New"), "<control>N", _("Create New File") },//, G_CALLBACK(itemPressed)},
			{ "FileOpen", GTK_STOCK_OPEN,_("_Open"),"<control>O", _("Open A File") },//, G_CALLBACK(itemPressed)},
			{ "FileClose", GTK_STOCK_CLOSE, _("_Close"),"<control>W", _("Close File") } , // G_CALLBACK(gtk_main_quit)},
			{ "FileCloseAll", GTK_STOCK_CLOSE, _("C_lose All"),"<control><shift>W", _("Close All Files") } , // G_CALLBACK(gtk_main_quit)},
			{ "FileSave", GTK_STOCK_SAVE, _("_Save"), "<control>S", _("Save File") ,  G_CALLBACK(LinuxDoIDE_save_menu_callback)},
			{ "FileSaveAs", GTK_STOCK_SAVE_AS, _("Save _As"), "<control><shift>S", _("Save File As ...") }, // G_CALLBACK(itemPressed)},
			{ "FileQuit", GTK_STOCK_QUIT, _("_Quit"), "<control>Q", _("QUIT") ,  G_CALLBACK(gtk_main_quit)},

			{ "ProjectMenu", NULL, _("_Project")},
			{ "ProjectBuild", NULL , _("_Build"), "F2", _("Build project") ,  G_CALLBACK(LinuxDoIDE_menu_build_cb)},

			{ "HelpMenu", GTK_STOCK_HELP, _("_Help") },
			{ "HelpAbout", GTK_STOCK_ABOUT, _("_About") , "<control>H" , _("_About") , G_CALLBACK(LinuxDoIDE_show_about_menu_callback) }
		};

	GtkUIManager * uimgr = gtk_ui_manager_new();

	gtk_ui_manager_add_ui_from_string(uimgr, menubar, sizeof(menubar) - 1, &err);

	GtkActionGroup* actionGroup = gtk_action_group_new("Actions");

	gtk_action_group_add_actions(actionGroup, actions, G_N_ELEMENTS(actions), ide);

	gtk_ui_manager_insert_action_group(uimgr,actionGroup,0);

	gtk_window_add_accel_group(ide->main_window,gtk_ui_manager_get_accel_group(uimgr));

	GdkPixbuf * pixbuf = gdk_pixbuf_new_from_inline(sizeof(LinuxDo_icon_pixbuf)-1,LinuxDo_icon_pixbuf,FALSE,NULL) ;

	GtkIconFactory * app = gtk_icon_factory_new();

	GtkIconSet * icon_set = gtk_icon_set_new_from_pixbuf(pixbuf);

	gtk_icon_theme_add_builtin_icon(PACKAGE_NAME,64,pixbuf);
	gtk_icon_factory_add(app,PACKAGE_NAME, icon_set);
	gtk_icon_set_unref(icon_set);
	gtk_icon_factory_add_default(app);
	g_object_unref(app);

	gtk_window_set_icon_name(ide->main_window,PACKAGE_NAME);
	gtk_window_set_icon(ide->main_window,pixbuf);

	g_object_unref(pixbuf);

	//build main layout, other window will be build within the create event of main window
	ide->widget_vbox = GTK_BOX(gtk_vbox_new(0,0));
	ide->mainlayout  = GTK_PANED(gtk_hpaned_new());

	ide->menubar = gtk_ui_manager_get_widget(uimgr,"/MenuBar"); // GTK_WIDGET(gtk_item_factory_get_widget (ide->menu, "<main>")) ;// GTK_MENU_BAR(gtk_menu_bar_new());

	ide->toolbar = gtk_ui_manager_get_widget(uimgr,"/TOOLBAR"); //GTK_TOOLBAR(gtk_toolbar_new());

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->menubar),0,0,0);

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->toolbar),0,0,0);

	gtk_container_add(GTK_CONTAINER(ide->main_window),GTK_WIDGET(ide->widget_vbox));

	ide->statusbar = GTK_STATUSBAR(gtk_statusbar_new());

	gtk_statusbar_push(ide->statusbar,0,_("Ready"));

	gtk_statusbar_set_has_resize_grip(ide->statusbar,TRUE);

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->mainlayout),1,1,1);

	gtk_box_pack_end(ide->widget_vbox,GTK_WIDGET(ide->statusbar),0,0,0);

	gtk_window_resize(ide->main_window,500,400);

	GtkWidget * bt1 = gtk_button_new_with_label("我菜鸟！！！");

	GtkWidget * bt3 = gtk_button_new_with_label("right area");
	GtkWidget * xterm = gtk_scrolled_window_new(NULL,NULL);

	ide->main_layout.mid_layout.console = gtk_xterm_new();

	gtk_container_add(GTK_CONTAINER(xterm),ide->main_layout.mid_layout.console);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(xterm),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	// main area
	ide->main_layout.left = GTK_NOTEBOOK(gtk_notebook_new());
	gtk_notebook_set_tab_pos(ide->main_layout.left,GTK_POS_BOTTOM);

	ide->main_layout.right = GTK_PANED(gtk_hpaned_new());
	gtk_paned_add1(ide->mainlayout,GTK_WIDGET(ide->main_layout.left));
	gtk_paned_add2(ide->mainlayout,GTK_WIDGET(ide->main_layout.right));

	ide->main_layout.midlayout = GTK_PANED(gtk_vpaned_new());

	gtk_paned_add1(ide->main_layout.right,GTK_WIDGET(ide->main_layout.midlayout));

	gtk_paned_add2(ide->main_layout.right,bt3);//,FALSE,FALSE);

	ide->main_layout.mid_layout.code = gtk_editors_new();

	gtk_notebook_set_scrollable(GTK_NOTEBOOK(ide->main_layout.mid_layout.code),TRUE);

	gtk_paned_add1(ide->main_layout.midlayout,GTK_WIDGET(ide->main_layout.mid_layout.code));
	gtk_paned_add2(ide->main_layout.midlayout,xterm);


	ide->main_layout.left_layout.tree_scroll = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL,NULL));

	ide->main_layout.left_layout.tree = gtk_tree_view_dir_new();

	gtk_scrolled_window_set_policy(ide->main_layout.left_layout.tree_scroll,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(ide->main_layout.left_layout.tree_scroll), GTK_WIDGET(ide->main_layout.left_layout.tree));

	gtk_notebook_append_page(ide->main_layout.left,
			GTK_WIDGET(ide->main_layout.left_layout.tree_scroll),
			gtk_label_new_with_mnemonic(_("Explorer")));

	gtk_notebook_append_page(ide->main_layout.left,bt1,	gtk_label_new_with_mnemonic(("菜鸟视图")));


	GtkWidget * scroll = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	GtkWidget * untitled =  GTK_WIDGET(ide_editor_new());

	gtk_container_add(GTK_CONTAINER(scroll),untitled);

 	gtk_notebook_append_page(GTK_NOTEBOOK(ide->main_layout.mid_layout.code),GTK_WIDGET(scroll),gtk_label_new_with_mnemonic(_("Untitled")));

	gtk_window_set_title(ide->main_window,_("Linux-Do"));

	gtk_widget_show_all(GTK_WIDGET(ide->statusbar));

	gtk_widget_show_all(GTK_WIDGET(ide->menubar));

	gtk_widget_show(GTK_WIDGET(ide->widget_vbox));

	gtk_widget_show(GTK_WIDGET(ide->main_window));

	/*让窗口快点显示出来*/
	gdk_flush();

	//然后在慢慢显示内部控件吼吼
	gtk_widget_show_all(GTK_WIDGET(ide->main_window));

	gtk_widget_grab_focus(untitled);

	gdk_flush();

}

void ide_window_set_title(IDE_AUTOTOOLS * obj, GtkWindow * window )
{
	gchar * title = g_strdup_printf(_("Linux-Do - %s"),obj->project_name->str);

	gtk_window_set_title(window,title);

	g_free(title);

}
