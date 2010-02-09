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

typedef struct _LinuxDoIDE{
	GtkWindow * main_window;
	GtkBox  *	widget_vbox;
	GtkMenuBar * menubar;
	GtkStatusbar * statusbar;
	GtkToolbar * toolbar;
	GtkHPaned *  mainlayout;
	struct{


	}main_layout;
	struct{
		GtkMenuItem * file;

	}menu;
	struct{
		//first group
		GtkToolButton * new;
		GtkToolButton * open;
		GtkToolButton * close;

		//other
		GtkToolButton * buttons[20];

	}toolbaritem;

}LinuxDoIDE;

void build_ui(LinuxDoIDE * ide)
{

}


int main(int argc, char * argv[])
{
	gtk_init(&argc, &argv);
	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);

	LinuxDoIDE ide;

	printf(_("Linux-DO start up\n"));

	//build main window and loop, other window will be build within the create event of main window
	ide.main_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	ide.widget_vbox = GTK_BOX(gtk_vbox_new(0,0));

	gtk_container_add(GTK_CONTAINER(ide.main_window),GTK_WIDGET(ide.widget_vbox));

	ide.statusbar = GTK_STATUSBAR(gtk_statusbar_new());
	ide.menubar = GTK_MENU_BAR(gtk_menu_bar_new());

//	GtkWidget * widget_hpanel = gtk_hpaned_new();

	GtkWidget * bt1 = gtk_button_new_with_label("real area");


	gtk_box_pack_start(ide.widget_vbox,GTK_WIDGET(ide.menubar),0,0,0);



	gtk_box_pack_end(ide.widget_vbox,GTK_WIDGET(ide.statusbar),0,0,0);

	gtk_statusbar_push(ide.statusbar,2,_("Ready"));

	ide.menu.file = GTK_MENU_ITEM(gtk_menu_item_new_with_mnemonic(_("_File")));

	gtk_menu_shell_append(GTK_MENU_SHELL(ide.menubar),GTK_WIDGET(ide.menu.file));

	ide.toolbar = GTK_TOOLBAR(gtk_toolbar_new());

	gtk_box_pack_start(ide.widget_vbox,GTK_WIDGET(ide.toolbar),0,0,0);

	ide.toolbaritem.new = GTK_TOOL_BUTTON(gtk_tool_button_new_from_stock(GTK_STOCK_NEW));

	gtk_toolbar_insert(ide.toolbar,ide.toolbaritem.new,-1);

	gtk_box_pack_start(ide.widget_vbox,GTK_WIDGET(bt1),1,1,1);

//	gtk_paned_add2(GTK_PANED(widget_vpanel),statubar);
//	gtk_paned_add1(GTK_PANED(widget_vpanel),bt1);

//	GtkWidget * statubar = gtk_statusbar_new();


//	GTK_PANED(widget_vpanel)->child1 = gtk_button_new_with_label("hello1_c1");

//	gtk_container_add(GTK_CONTAINER(GTK_PANED(widget_vpanel)->child1),bt1);

//	gtk_container_add(GTK_CONTAINER(GTK_PANED(widget_vpanel)->child2),bt2);

	//	gtk_container_add(GTK_CONTAINER(widget_main),statubar);

	gtk_widget_show_all(GTK_WIDGET(ide.main_window));


	gtk_main();
	return 0;
}
