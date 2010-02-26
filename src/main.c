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
#include "TreeView.h"
#include "autotools.h"
#include "SourceView.h"
#include "ide.h"
#include "callbacks.h"
#include "misc.h"
#include <glib/gstdio.h>

#include "../icons/LinuxDo.icon.h"

static void build_ui(LinuxDoIDE * ide);

static void close_tab(gpointer callback_data, guint callback_action)
{
	LinuxDoIDE * ide = callback_data;
	GtkNotebook * note = ide->main_layout.mid_layout.code ;

	IDE_EDITOR * editor;

	if (gtk_notebook_get_n_pages(note) <= 1)
		return;
	guint cur = gtk_notebook_get_current_page(note);

	editor = gtk_notebook_get_editor(note,cur);

	ide_editor_savefile(editor,editor->file->str);

	gtk_notebook_remove_page(note,cur);
}

static void savefile(GtkButton * bt , IDE_EDITOR * editor)
{
	ide_editor_savefile(editor,editor->file->str);
	gtk_notebook_remove_page(editor->note,gtk_notebook_page_num(editor->note,gtk_widget_get_parent(GTK_WIDGET(editor))));
}

static IDE_EDITOR * ide_notebook_new_page(GtkNotebook* note, const gchar * label)
{
	IDE_EDITOR * source_editor;

	source_editor = ide_editor_new();

	source_editor->note = note;

	GtkWidget * scroll = gtk_scrolled_window_new(NULL,NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(scroll), GTK_WIDGET(source_editor));

	GtkWidget * title = gtk_hbox_new(0,0);

	GtkWidget * bt = GTK_WIDGET(gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE));

	gtk_widget_set_size_request(bt,18,18);

	gtk_box_pack_start(GTK_BOX(title),gtk_label_new_with_mnemonic(label),TRUE,0,0);

	gtk_box_pack_start(GTK_BOX(title),bt,TRUE,0,0);

	gtk_widget_show_all(title);

	gtk_notebook_append_page(note,GTK_WIDGET(scroll),title);

	gtk_widget_show_all(GTK_WIDGET(source_editor));

	gtk_widget_show_all(GTK_WIDGET(gtk_notebook_get_nth_page(note,gtk_notebook_get_n_pages(note)-1)));

	gtk_notebook_set_current_page(note,gtk_notebook_get_n_pages(note)-1);

	g_signal_connect(G_OBJECT(bt),"clicked",G_CALLBACK(savefile),source_editor);

	return source_editor;
}

static void openfile(TREEVIEW_DIR* obj  ,gchar * item, gpointer userdata)
{
	LinuxDoIDE * ide = userdata;
	IDE_EDITOR * source_editor;

	GtkNotebook* note = ide->main_layout.mid_layout.code;

	source_editor = ide_notebook_new_page(note,item);

	ide_editor_openfile(source_editor,item);
}

static void connect_signals(LinuxDoIDE * ide)
{
	// resize
	g_signal_connect(G_OBJECT(ide->main_window),"configure-event",G_CALLBACK(main_window_on_configure),ide);
	g_signal_connect(G_OBJECT (ide->main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (ide->main_layout.left_layout.tree), "openfile", G_CALLBACK(openfile), ide);
}

int main(int argc, char * argv[])
{
	LinuxDoIDE ide;

	gboolean init_project;
	gchar * basedir=NULL;

	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);

	GOptionEntry args[] =
	{
			{"init-project",'\0',0,G_OPTION_ARG_NONE,&init_project,_("do git init and build initial dir struct for use with autotools")},
			{"root",'\0',0,G_OPTION_ARG_STRING,&basedir,_("set project root dir"), N_("dir")},
			{0}
	};
	

	g_set_prgname(PACKAGE_NAME);

	g_assert( gtk_init_with_args(&argc, &argv,PACKAGE_STRING,args,NULL,NULL));

	g_set_application_name(_(PACKAGE_NAME));

	printf(_("Linux-DO start up\n"));

	build_ui(&ide);

	connect_signals(&ide);
	
	ide.project_mgr = ide_autotools_new();
	
	if(basedir)
	{
		g_chdir(basedir);
		g_free(basedir);
	}

	ide_autotools_set_configure_ac(ide.project_mgr,argc==2?argv[1]:".");

	g_object_set(ide.main_layout.left_layout.tree,"mgr",ide.project_mgr,NULL);
		
	if(argc == 2  )
	{
		g_chdir(ide.project_mgr->project_path->str);
	}
	
	g_object_set(ide.main_layout.left_layout.tree,"current-dir",".",NULL);
	
	if(argc == 2)
	{
		//打开文件，吼吼
		openfile(NULL,argv[1],&ide);
	}
	
	gtk_main();
	return 0;
}

static void build_ui(LinuxDoIDE * ide)
{
	GtkAccelGroup * accel;
	//build main window
	ide->main_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	accel = gtk_accel_group_new();

	gtk_window_add_accel_group(ide->main_window,accel);

	GtkItemFactoryEntry entry[] = {
			{  _("/_File/_New") , NULL, 0, 0 , "<StockItem>" , GTK_STOCK_NEW },
			{  _("/_File/_Open") , NULL, 0, 0 , "<StockItem>" , GTK_STOCK_OPEN },
			{  _("/_File/--") , NULL, 0, 0 , "<Separator>" , NULL },
			{  _("/_File/_Close") , NULL, (GtkItemFactoryCallback)close_tab, 0 , "<StockItem>" , GTK_STOCK_CLOSE },
			{  _("/_File/C_lose All") , "<control><shift>w", 0, 0 , "<StockItem>" , GTK_STOCK_CLOSE},
			{  _("/_File/--") , NULL, 0, 0 , "<Separator>" , NULL },
			{  _("/_File/_Save") , NULL,(GtkItemFactoryCallback)LinuxDoIDE_save_menu_callback, 0 , "<StockItem>" , GTK_STOCK_SAVE },
			{  _("/_File/Save _As") , NULL, 0, 0 , "<StockItem>" , GTK_STOCK_SAVE_AS},

			{  _("/_Help/_About") , "<control>h", (GtkItemFactoryCallback) LinuxDoIDE_show_about_menu_callback , (guint)ide->main_window , "<StockItem>" , GTK_STOCK_ABOUT },
	};

	GtkIconFactory * app = gtk_icon_factory_new();

	GdkPixbuf * pixbuf = gdk_pixbuf_new_from_inline(sizeof(LinuxDo_icon_pixbuf),LinuxDo_icon_pixbuf,FALSE,NULL) ;

	gtk_icon_theme_add_builtin_icon(PACKAGE_NAME,64,pixbuf);

	GtkIconSet * icon_set = gtk_icon_set_new_from_pixbuf(pixbuf);

	gtk_window_set_icon_name(ide->main_window,PACKAGE_NAME);

	g_object_unref(pixbuf);

	gtk_icon_factory_add(app,PACKAGE_NAME, icon_set);

	gtk_icon_set_unref(icon_set);

	g_object_unref(app);

	//build main layout, other window will be build within the create event of main window
	ide->widget_vbox = GTK_BOX(gtk_vbox_new(0,0));
	ide->mainlayout  = GTK_PANED(gtk_hpaned_new());

	gtk_container_add(GTK_CONTAINER(ide->main_window),GTK_WIDGET(ide->widget_vbox));

	ide->statusbar = GTK_STATUSBAR(gtk_statusbar_new());

	ide->menu = gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",accel);

	gtk_item_factory_create_items(ide->menu, sizeof(entry)/sizeof(GtkItemFactoryEntry) ,entry,ide);

	ide->menubar = GTK_WIDGET(gtk_item_factory_get_widget (ide->menu, "<main>")) ;// GTK_MENU_BAR(gtk_menu_bar_new());

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->menubar),0,0,0);

	gtk_statusbar_push(ide->statusbar,0,_("Ready"));

	gtk_statusbar_set_has_resize_grip(ide->statusbar,TRUE);

	ide->toolbar = GTK_TOOLBAR(gtk_toolbar_new());

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->toolbar),0,0,0);

	//Tool bar items

	ide->toolbaritem.new = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_NEW));
	ide->toolbaritem.open = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_OPEN));
	ide->toolbaritem.close = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE));

	ide->toolbaritem.sep1  = GTK_TOOL_ITEM(gtk_separator_tool_item_new());

	ide->toolbaritem.cut = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_CUT));
	ide->toolbaritem.copy = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_COPY));
	ide->toolbaritem.past = GTK_TOOL_ITEM(gtk_tool_button_new_from_stock(GTK_STOCK_PASTE));

	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.new),-1);
	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.open),-1);
	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.close),-1);

	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.sep1),-1);

	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.cut),-1);
	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.copy),-1);
	gtk_toolbar_insert(ide->toolbar,GTK_TOOL_ITEM(ide->toolbaritem.past),-1);


	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->mainlayout),1,1,1);
	gtk_box_pack_end(ide->widget_vbox,GTK_WIDGET(ide->statusbar),0,0,0);

	gtk_window_resize(ide->main_window,500,400);

	GtkWidget * bt1 = gtk_button_new_with_label("我菜鸟！！！");

	GtkWidget * bt3 = gtk_button_new_with_label("right area");
	GtkWidget * bt5= gtk_button_new_with_label("support area");

	// main area
	ide->main_layout.left = GTK_NOTEBOOK(gtk_notebook_new());
	gtk_notebook_set_tab_pos(ide->main_layout.left,GTK_POS_BOTTOM);

	ide->main_layout.right = GTK_PANED(gtk_hpaned_new());
	gtk_paned_add1(ide->mainlayout,GTK_WIDGET(ide->main_layout.left));
	gtk_paned_add2(ide->mainlayout,GTK_WIDGET(ide->main_layout.right));

	ide->main_layout.midlayout = GTK_PANED(gtk_vpaned_new());

	gtk_paned_add1(ide->main_layout.right,GTK_WIDGET(ide->main_layout.midlayout));
	gtk_paned_add2(ide->main_layout.right,bt3);//,FALSE,FALSE);

	ide->main_layout.mid_layout.code = GTK_NOTEBOOK(gtk_notebook_new());

	gtk_notebook_set_scrollable(ide->main_layout.mid_layout.code,TRUE);

	gtk_paned_add1(ide->main_layout.midlayout,GTK_WIDGET(ide->main_layout.mid_layout.code));
	gtk_paned_add2(ide->main_layout.midlayout,bt5);


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

	gtk_container_add(GTK_CONTAINER(scroll), GTK_WIDGET(ide_editor_new()));

	gtk_notebook_append_page(ide->main_layout.mid_layout.code,GTK_WIDGET(scroll),gtk_label_new_with_mnemonic(_("Untitled")));

	gtk_window_set_title(ide->main_window,_("Linux-Do"));

	gtk_widget_show_all(GTK_WIDGET(ide->statusbar));

	gtk_widget_show_all(GTK_WIDGET(ide->menubar));

	gtk_widget_show(GTK_WIDGET(ide->widget_vbox));

	gtk_widget_show(GTK_WIDGET(ide->main_window));

	/*让窗口快点显示出来*/
	gdk_flush();

	//然后在慢慢显示内部控件吼吼
	gtk_widget_show_all(GTK_WIDGET(ide->main_window));
}

