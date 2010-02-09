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
	GtkPaned *  mainlayout;
	struct{
		GtkNotebook  * left;
		GtkPaned  * right;
		struct{
			GtkNotebook * code;
			GtkWidget * support;
		}mid_layout;
		GtkPaned* midlayout;
	}main_layout;
	struct{
		GtkMenuItem * file;

	}menu;
	struct{
		//first group
		GtkToolItem * new;
		GtkToolItem * open;
		GtkToolItem * close;

		GtkToolItem * sep1;
		//second group
		GtkToolItem * cut;
		GtkToolItem * copy;
		GtkToolItem * past;


		//other
		GtkToolButton * buttons[20];

	}toolbaritem;

}LinuxDoIDE;

void build_ui(LinuxDoIDE * ide)
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

	gtk_menu_shell_append(GTK_MENU_SHELL(ide->menubar),GTK_WIDGET(ide->menu.file));

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


//	gtk_toolbar_insert_space(ide->toolbar,-1);


	gtk_box_pack_start(ide->widget_vbox,GTK_WIDGET(ide->mainlayout),1,1,1);
	gtk_box_pack_end(ide->widget_vbox,GTK_WIDGET(ide->statusbar),0,0,0);

	//	GtkWidget * widget_hpanel = gtk_hpaned_new();
	GtkWidget * bt1 = gtk_button_new_with_label("left area");
	GtkWidget * bt2 = gtk_button_new_with_label("code area");
	GtkWidget * bt3 = gtk_button_new_with_label("right area");
	GtkWidget * bt4 = gtk_button_new_with_label("test area");
	GtkWidget * bt5= gtk_button_new_with_label("support area");

	// main area
	ide->main_layout.left = GTK_NOTEBOOK(gtk_notebook_new());
	gtk_notebook_set_tab_pos(ide->main_layout.left,GTK_POS_BOTTOM);

	ide->main_layout.right = GTK_PANED(gtk_hpaned_new());
	gtk_paned_add1(ide->mainlayout,GTK_WIDGET(ide->main_layout.left));
	gtk_paned_add2(ide->mainlayout,GTK_WIDGET(ide->main_layout.right));

	ide->main_layout.midlayout = GTK_PANED(gtk_vpaned_new());

	gtk_paned_add1(ide->main_layout.right,GTK_WIDGET(ide->main_layout.midlayout));
	gtk_paned_add2(ide->main_layout.right,bt3);

//	ide->main_layout.mid = GTK_PANED(gtk_vpaned_new());
//	ide->main_layout.mid_layout.code = GTK_NOTEBOOK(gtk_notebook_new());

	ide->main_layout.mid_layout.code = GTK_NOTEBOOK(gtk_notebook_new());

	gtk_paned_add1(ide->main_layout.midlayout,GTK_WIDGET(ide->main_layout.mid_layout.code));
	gtk_paned_add2(ide->main_layout.midlayout,bt5);


	gtk_notebook_append_page(ide->main_layout.left,bt1,0);
	gtk_notebook_append_page(ide->main_layout.left,bt4,0);

	gtk_notebook_append_page(ide->main_layout.mid_layout.code,bt2,0);

	gtk_window_resize(ide->main_window,500,400);
	gtk_widget_show_all(GTK_WIDGET(ide->main_window));
}


int main(int argc, char * argv[])
{
	gtk_init(&argc, &argv);
	setlocale(LC_ALL, "");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);

	LinuxDoIDE ide;

	printf(_("Linux-DO start up\n"));

	build_ui(&ide);

	gtk_main();
	return 0;
}
