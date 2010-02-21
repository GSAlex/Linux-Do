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
#include "Linuxdo.h"
#include "misc.h"

IDE_EDITOR * gtk_notebook_get_editor(GtkNotebook * note, guint nth)
{
	GList * list;
	IDE_EDITOR * editor;
	
	GtkWidget * curpage = gtk_notebook_get_nth_page(note,nth);
	
	list = gtk_container_get_children(GTK_CONTAINER(curpage));
	
	editor = IDE_EDITOR(g_list_first(list)->data);
	
	g_list_free(list);
	
	return editor;
}
