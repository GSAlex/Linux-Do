/*
 * SourceView.c
 *
 *  Created on: 2010-2-20
 *      Author: cai
 */

#include "Linuxdo.h"
#include <string.h>
#include "SourceView.h"

static void ide_editor_class_init(IDE_EDITORClass * klass);
static void ide_editor_init(IDE_EDITOR * obj);
static void ide_editor_finalize(IDE_EDITOR*);

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
	klass->finalize = G_OBJECT_CLASS(klass)->finalize;

	G_OBJECT_CLASS(klass)->finalize = ( void(*)(GObject*) )ide_editor_finalize;
}

void ide_editor_finalize(IDE_EDITOR*obj)
{
	if(obj->file)
		g_string_free(obj->file,TRUE);
	IDE_EDITOR_CLASS_GET_CLASS(obj)->finalize(G_OBJECT(obj));
}

void ide_editor_init(IDE_EDITOR * obj)
{
	GtkSourceLanguage * lang = gtk_source_language_manager_get_language(
			IDE_EDITOR_CLASS_GET_CLASS(obj)->lmgr, "c");

	GtkSourceBuffer * buffer ;

	buffer = gtk_source_buffer_new_with_language(lang);

	gtk_text_view_set_buffer(GTK_TEXT_VIEW(obj),GTK_TEXT_BUFFER(buffer));

	g_object_unref(buffer);

	obj->buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj)));

	gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(obj),TRUE);


}

IDE_EDITOR* ide_editor_new()
{
	return IDE_EDITOR(g_object_new (IDE_TYPE_EDITOR, NULL));
}

gboolean ide_editor_openfile(IDE_EDITOR * editor, const gchar * url)
{
	GtkTextIter start;
	guint content_len;
	gchar * content;
	GMappedFile * mfile;

	mfile = g_mapped_file_new(url,FALSE,NULL);
	
	g_return_if_fail(mfile);

	editor->file = g_string_new(url);

	content_len = g_mapped_file_get_length(mfile);
	content = g_mapped_file_get_contents(mfile);

	gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(editor->buffer),&start);

	gtk_text_buffer_insert(GTK_TEXT_BUFFER(editor->buffer),&start,content,content_len);

	g_mapped_file_unref(mfile);

	gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(editor->buffer),FALSE);
}

gboolean ide_editor_savefile(IDE_EDITOR * editor, const gchar * url)
{
	GtkTextIter start,end;
	gchar * line;
	GtkTextBuffer *  buffer =  GTK_TEXT_BUFFER(editor->buffer);

	if (!gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(editor->buffer)))
		return FALSE;

	GFile * gfile =  g_file_new_for_commandline_arg(url);

	g_return_val_if_fail(gfile,FALSE);

	GOutputStream * out = G_OUTPUT_STREAM(g_file_replace(gfile,NULL,TRUE,G_FILE_CREATE_NONE,NULL,NULL));

	int i,linecount = gtk_text_buffer_get_char_count(buffer) ;

	gtk_text_buffer_get_iter_at_line(buffer,&end,0);

	for(i=1;i < linecount ; ++i)
	{
		start = end;

		gtk_text_buffer_get_iter_at_line(buffer,&end,i);
		line = gtk_text_buffer_get_slice(buffer,&start,&end,TRUE);
		g_output_stream_write(out,(const void*)line,strlen(line),NULL,NULL);

	//	g_print("%d line is %s\n",i,line);

	}

	g_output_stream_close(out,NULL,NULL);

	g_object_unref(gfile);


}
