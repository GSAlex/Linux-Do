/*
 * ide.h
 *
 *  Created on: 2010-2-19
 *      Author: cai
 */

#ifndef IDE_H_
#define IDE_H_

#include "TreeView.h"

typedef struct _LinuxDoIDE{
	GtkWindow * main_window;
	GtkBox  *	widget_vbox;
	GtkMenuBar * menubar;
	GtkStatusbar * statusbar;
	GtkToolbar * toolbar;
	GtkPaned *  mainlayout;
	struct{

		struct{
			TREEVIEW_DIR * tree;
			GtkTreeStore * tree_model;

		}left_layout;

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
		GtkMenuItem * edit;

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

#endif /* IDE_H_ */
