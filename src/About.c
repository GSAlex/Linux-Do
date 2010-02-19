/*
 * About.c
 *
 *  Created on: 2010-2-19
 *      Author: cai
 */


#include "Linuxdo.h"
#include "About.h"


void LinuxDoIDE_show_about(GtkWidget * parent)
{
	static const gchar* authors[3]= { "microcai" , "alex" , 0 } ;

	gtk_show_about_dialog(0 ,"version" , PACKAGE_VERSION , "authors" ,authors  , "website" , "http://github.com/GSAlex/Linux-Do" , NULL ,NULL );

}
