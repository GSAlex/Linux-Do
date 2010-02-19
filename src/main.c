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


static void build_ui(LinuxDoIDE * ide)
{
	//build main window and loop, other window will be build within the create event of main window
	ide->main_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	ide->widget_vbox = GTK_BOX(gtk_vbox_new(0,0));
	ide->mainlayout  = GTK_PANED(gtk_hpaned_new());

	gtk_container_add(GTK_CONTAINER(ide->main_window),GTK_WIDGET(ide->widget_vbox));


	ide->statusbar = GTK_STATUSBAR(gtk_statusbar_new());
	ide->menubar = GTK_MENU_BAR(gtk_menu_bar_new());

	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->menubar),0,0,0);

	gtk_statusbar_push(ide->statusbar,0,_("Ready"));

	ide->menu.file = GTK_MENU_ITEM(gtk_menu_item_new_with_mnemonic(_("_File")));
	ide->menu.edit = GTK_MENU_ITEM(gtk_menu_item_new_with_mnemonic(_("_Edit")));

	gtk_menu_shell_append(GTK_MENU_SHELL(ide->menubar),GTK_WIDGET(ide->menu.file));
	gtk_menu_shell_append(GTK_MENU_SHELL(ide->menubar),GTK_WIDGET(ide->menu.edit));

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

	//	GtkWidget * widget_hpanel = gtk_hpaned_new();
	GtkWidget * bt1 = gtk_button_new_with_label("我菜鸟！！！");
//	GtkWidget * bt2 = gtk_button_new_with_label("code area");
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


//	ide->main_layout.mid = GTK_PANED(gtk_vpaned_new());
//	ide->main_layout.mid_layout.code = GTK_NOTEBOOK(gtk_notebook_new());

	ide->main_layout.mid_layout.code = GTK_NOTEBOOK(gtk_notebook_new());

	gtk_paned_add1(ide->main_layout.midlayout,GTK_WIDGET(ide->main_layout.mid_layout.code));
	gtk_paned_add2(ide->main_layout.midlayout,bt5);


	ide->main_layout.left_layout.tree = GTK_TREE_VIEW(gtk_tree_view_dir_new());

	gtk_notebook_append_page(ide->main_layout.left,GTK_WIDGET(ide->main_layout.left_layout.tree), gtk_label_new_with_mnemonic(_("Explorer")));
	gtk_notebook_append_page(ide->main_layout.left,bt1,	gtk_label_new_with_mnemonic(("菜鸟视图")));

	gtk_notebook_append_page(ide->main_layout.mid_layout.code,gtk_source_view_new(),gtk_label_new_with_mnemonic(_("Untitled")));

	gtk_window_set_title(ide->main_window,_("Linux-Do"));
	gtk_widget_show_all(GTK_WIDGET(ide->main_window));
}

static gboolean main_window_on_configure(GtkWidget *widget,
		GdkEventConfigure *event, gpointer user_data)
{
//	puts(__func__);
	LinuxDoIDE * ide = (LinuxDoIDE*) user_data;

	// 吼吼，更复杂都可以的啦

	gtk_paned_set_position(ide->main_layout.right,event->width-220);
	gtk_paned_set_position(ide->main_layout.midlayout,event->height-140);

}

static void openfile(TREEVIEW_DIR* obj  ,gchar * item, gpointer userdata)
{
	puts(item);
}

void connect_signals(LinuxDoIDE * ide)
{
	// resize
	g_signal_connect(G_OBJECT(ide->main_window),"configure-event",G_CALLBACK(main_window_on_configure),ide);
	g_signal_connect(G_OBJECT (ide->main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (ide->main_layout.left_layout.tree), "openfile", G_CALLBACK(openfile), NULL);
}

int main(int argc, char * argv[])
{
	LinuxDoIDE ide;

	g_set_prgname(PACKAGE_NAME);

	gtk_init(&argc, &argv);
	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);


	printf(_("Linux-DO start up\n"));

	build_ui(&ide);

	gtk_tree_view_dir_set_dir(ide.main_layout.left_layout.tree,".");

	connect_signals(&ide);
	gtk_main();
	return 0;
}
