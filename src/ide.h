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
 * ide.h - header file for  LinuxDoIDE
 *  Created on: 2010-2-19
 *      Author: cai
 */

#ifndef IDE_H_
#define IDE_H_

#include "TreeView.h"

typedef struct _LinuxDoIDE{
	GtkWindow * main_window;
	GtkBox  *	widget_vbox;
	GtkWidget * menubar;
	GtkStatusbar * statusbar;
	GtkToolbar * toolbar;
	GtkPaned *  mainlayout;
	struct{

		struct{
			TREEVIEW_DIR * tree;
			GtkScrolledWindow * tree_scroll;

		}left_layout;

		GtkNotebook  * left;
		GtkPaned  * right;
		struct{
			GtkNotebook * code;
			GtkWidget * support;
		}mid_layout;
		GtkPaned* midlayout;

	}main_layout;
//	struct{
//		GtkMenuItem * file;
//		GtkMenuItem * edit;
//
//	}menu;
	GtkItemFactory * menu;
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

#endif /* IDE_H_ */
