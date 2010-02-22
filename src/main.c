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
#include "ide.h"
#include "SourceView.h"
#include "callbacks.h"
#include "misc.h"

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

static gboolean find_project_base_dir(gchar * pathin, gchar * pathout)
{
	GFile * file,*of;
	gchar * path,*ac;
	//通过指定的文件，自动找到包含 configure.ac 的文件夹，自动设置项目路径到此处。
	file = g_file_new_for_commandline_arg(pathin);
	
	if(!g_file_is_native(file))
	{
		g_object_unref(file);
		return FALSE;
	}
	
	for( of = NULL ; file ; of = file,file = g_file_get_parent(file),g_object_unref(of))
	{
		//查找当前目录下是否有 configure.ac 文件
		 path = g_file_get_path(file);
		 
		 ac = g_strdup_printf("%s/configure.ac",path); 
		 
		 if(g_file_test(ac,G_FILE_TEST_EXISTS))
		 {
			 g_free(ac);
			 g_object_unref(file);
			 g_stpcpy(pathout,path);
			 g_free(path);
			 return TRUE;			 
		 }
		 g_free(ac);
		 g_free(path);
	}
	g_object_unref(file);
	return FALSE;
}

static gchar	  basedir[255] = ".";

static gboolean set_dir(gpointer ptr)
{
	gtk_tree_view_dir_set_dir(GTK_TREE_VIEW_DIR(ptr),basedir);
	return FALSE;
}

int main(int argc, char * argv[])
{
	LinuxDoIDE ide;

	gboolean init_project;

	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);

	GOptionEntry args[] =
	{
			{"init-project",0,0,G_OPTION_ARG_NONE,&init_project,_("do git init and build initial dir struct for use with autotools")},
			{"root",0,0,G_OPTION_ARG_STRING,basedir,_("set project root dir"), N_("dir")},
			{0}
	};
	

	g_set_prgname(PACKAGE_NAME);

	g_assert( gtk_init_with_args(&argc, &argv,PACKAGE_STRING,args,NULL,NULL));

	if(argc == 2)
	{
		find_project_base_dir(argv[1],basedir);
	}

	g_set_application_name(_(PACKAGE_NAME));

	printf(_("Linux-DO start up\n"));

	build_ui(&ide);

	connect_signals(&ide);

	g_idle_add(set_dir,ide.main_layout.left_layout.tree);
	
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
	gtk_window_set_icon_from_file(ide->main_window,APPICONDIR"/LinuxDo.svg",NULL);
	gtk_widget_show_all(GTK_WIDGET(ide->main_window));
}

