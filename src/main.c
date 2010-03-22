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

	ide.project_mgr = ide_autotools_new();

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

		return ide_autotools_create_tmpl(ide.project_mgr,package_name,TRUE);
	}

	printf(_("Linux-DO start up\n"));

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
