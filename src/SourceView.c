/*
 * SourceView.c
 *
 *  Created on: 2010-2-20
 *      Author: cai
 */

#include "Linuxdo.h"
#include "SourceView.h"

static void ide_editor_class_init(IDE_EDITORClass * klass);
static void ide_editor_class_finalize(IDE_EDITORClass * klass);
static void ide_editor_init(IDE_EDITOR * obj);

GType ide_editor_get_type()
{
	static volatile GType type = 0;

	static GTypeInfo typeinfo = { sizeof(IDE_EDITORClass),
			(GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL,
			(GClassInitFunc) ide_editor_class_init, 0, NULL,
			sizeof(IDE_EDITOR), 0, (GInstanceInitFunc) ide_editor_init, };

	if(g_once_init_enter(&type))
	{
		g_once_init_leave(&type,g_type_register_static(GTK_TYPE_SOURCE_VIEW,"IDE_EDITOR",&typeinfo,0));
	}
	return type;
}

void ide_editor_class_init(IDE_EDITORClass * klass)
{
	klass->lmgr = gtk_source_language_manager_new();
}

void ide_editor_init(IDE_EDITOR * obj)
{
	GtkSourceLanguage * lang = gtk_source_language_manager_get_language(
			IDE_EDITOR_CLASS_GET_CLASS(obj)->lmgr, "c");

	GtkSourceBuffer * buffer ;

	buffer = gtk_source_buffer_new_with_language(lang);

	gtk_text_view_set_buffer(GTK_TEXT_VIEW(obj),GTK_TEXT_BUFFER(buffer));

	g_object_unref(buffer);
	g_object_unref(lang);

	gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(obj),TRUE);

}

IDE_EDITOR* ide_editor_new()
{
	return IDE_EDITOR(g_object_new (IDE_TYPE_EDITOR, NULL));
}

gboolean ide_editor_openfile(IDE_EDITOR * editor, GFile * file)
{


}
