/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gedit-encodings-option-menu.c
 * This file is part of gedit
 *
 * Copyright (C) 2003-2005 - Paolo Maggi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA.
 */
 
/*
 * Modified by the gedit Team, 2003-2005. See the AUTHORS file for a 
 * list of people on the gedit Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <gedit/gedit-encodings-option-menu.h>
#include <gedit/gedit-prefs-manager.h>
#include <gedit/dialogs/gedit-encodings-dialog.h>

#define ENCODING_KEY "Enconding"

static void	  update_menu 		(GeditEncodingsOptionMenu       *option_menu);

/* Properties */
enum {
	PROP_0,
	PROP_SAVE_MODE
};

#define GEDIT_ENCODINGS_OPTION_MENU_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object),	\
							GEDIT_TYPE_ENCODINGS_OPTION_MENU,	\
							GeditEncodingsOptionMenuPrivate))

struct _GeditEncodingsOptionMenuPrivate
{
	gint activated_item;

	gboolean save_mode;
};


G_DEFINE_TYPE(GeditEncodingsOptionMenu, gedit_encodings_option_menu, GTK_TYPE_OPTION_MENU)

static void
gedit_encodings_option_menu_set_property (GObject 	*object, 
					  guint 	 prop_id,
			    		  const GValue *value, 
					  GParamSpec	*pspec)
{
	GeditEncodingsOptionMenu *om;

	g_return_if_fail (GEDIT_IS_ENCODINGS_OPTION_MENU (object));

    	om = GEDIT_ENCODINGS_OPTION_MENU (object);

	switch (prop_id)
	{
	case PROP_SAVE_MODE:
		om->priv->save_mode = g_value_get_boolean (value);

		update_menu (om);		
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gedit_encodings_option_menu_get_property (GObject 	*object, 
					  guint 	 prop_id,
			    		  GValue 	*value, 
					  GParamSpec	*pspec)
{
	GeditEncodingsOptionMenu *om;

	g_return_if_fail (GEDIT_IS_ENCODINGS_OPTION_MENU (object));

	om = GEDIT_ENCODINGS_OPTION_MENU (object);

	switch (prop_id)
	{	
	case PROP_SAVE_MODE:
		g_value_set_boolean (value, om->priv->save_mode);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gedit_encodings_option_menu_class_init (GeditEncodingsOptionMenuClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->set_property = gedit_encodings_option_menu_set_property;
	object_class->get_property = gedit_encodings_option_menu_get_property;

	g_object_class_install_property (object_class,
					 PROP_SAVE_MODE,
					 g_param_spec_boolean ("save-mode",
							       ("Save Mode"),
							       ("Save Mode"),
							       FALSE,
							       G_PARAM_READWRITE | 
							       G_PARAM_CONSTRUCT |
							       G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof(GeditEncodingsOptionMenuPrivate));
}

static void
dialog_response_cb (GtkDialog                *dialog,
                    gint                      response_id,
                    GeditEncodingsOptionMenu *option_menu)
{
	update_menu (option_menu);

	gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
add_or_remove (GtkMenuItem              *menu_item,
	       GeditEncodingsOptionMenu *option_menu)
{
	if (GTK_IS_RADIO_MENU_ITEM (menu_item))
	{
		option_menu->priv->activated_item = 
			gtk_option_menu_get_history (GTK_OPTION_MENU (option_menu));
	}
	else
	{
		GtkWidget *dialog;

		GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (option_menu));
	
		if (!GTK_WIDGET_TOPLEVEL (toplevel))
			toplevel = NULL;	
		
		gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu),
					     option_menu->priv->activated_item);

		dialog = gedit_encodings_dialog_new();

		if (toplevel != NULL)
		{
			GtkWindowGroup *wg;

			gtk_window_set_transient_for (GTK_WINDOW (dialog),
						      GTK_WINDOW (toplevel));

			wg = gtk_window_get_group (GTK_WINDOW (toplevel));
			if (wg == NULL)
			{
				wg = gtk_window_group_new ();
				gtk_window_group_add_window (wg,
							     GTK_WINDOW (toplevel));
			}

			gtk_window_group_add_window (wg,
						     GTK_WINDOW (dialog));
		}

		gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
		
		g_signal_connect (dialog,
				  "response",
				  G_CALLBACK (dialog_response_cb),
				  option_menu);

		gtk_widget_show (dialog);
	}
}

static void
update_menu (GeditEncodingsOptionMenu *option_menu)
{
	GtkWidget *menu;
	GtkWidget *menu_item;
	GSList    *group = NULL;
	GSList	  *encodings, *list;
	gchar     *str;

	const GeditEncoding *utf8_encoding;
	const GeditEncoding *current_encoding;

	menu = gtk_menu_new ();

	encodings = list = gedit_prefs_manager_get_shown_in_menu_encodings ();

	utf8_encoding = gedit_encoding_get_utf8 ();
	current_encoding = gedit_encoding_get_current ();

	if (!option_menu->priv->save_mode)
	{
		menu_item = gtk_radio_menu_item_new_with_label (group, _("Auto Detected"));
		group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menu_item));

		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
		gtk_widget_show (menu_item);

		g_signal_connect (menu_item,
				  "activate",
				  G_CALLBACK (add_or_remove),
				  option_menu);

		menu_item = gtk_separator_menu_item_new ();

		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
		gtk_widget_show (menu_item);
	}

	if (current_encoding != utf8_encoding)
		str = gedit_encoding_to_string (utf8_encoding);
	else
		str = g_strdup_printf (_("Current Locale (%s)"), 
				       gedit_encoding_get_charset (utf8_encoding));

	menu_item = gtk_radio_menu_item_new_with_label (group, str);

	g_signal_connect (menu_item,
			  "activate",
			  G_CALLBACK (add_or_remove),
			  option_menu);

	group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menu_item));

	g_object_set_data (G_OBJECT (menu_item), ENCODING_KEY, (gpointer)utf8_encoding);

	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
	gtk_widget_show (menu_item);

	g_free (str);

	if ((utf8_encoding != current_encoding) && 
	    (current_encoding != NULL))
	{
		str = g_strdup_printf (_("Current Locale (%s)"), 
				       gedit_encoding_get_charset (current_encoding));

		menu_item = gtk_radio_menu_item_new_with_label (group, str);

		g_signal_connect (menu_item,
				  "activate",
				  G_CALLBACK (add_or_remove),
				  option_menu);

		group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menu_item));

		g_object_set_data (G_OBJECT (menu_item), 
				   ENCODING_KEY, (gpointer)current_encoding);

		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
		gtk_widget_show (menu_item);

		g_free (str);
	}

	while (list != NULL)
	{
		const GeditEncoding *enc;
		
		enc = (const GeditEncoding *)list->data;

		if ((enc != current_encoding) && 
		    (enc != utf8_encoding) && 
		    (enc != NULL))
		{
			str = gedit_encoding_to_string (enc);

			menu_item = gtk_radio_menu_item_new_with_label (group, str);

			g_signal_connect (menu_item,
					  "activate",
					  G_CALLBACK (add_or_remove),
					  option_menu);

			group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menu_item));

			g_object_set_data (G_OBJECT (menu_item), ENCODING_KEY, (gpointer)enc);

			gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
			gtk_widget_show (menu_item);

			g_free (str);
		}
		
		list = g_slist_next (list);
	}

	g_slist_free (encodings);

	if (gedit_prefs_manager_shown_in_menu_encodings_can_set ())
	{
		menu_item = gtk_separator_menu_item_new ();
	      	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
		gtk_widget_show (menu_item);

		menu_item = gtk_menu_item_new_with_mnemonic (_("Add or _Remove..."));

		g_signal_connect (menu_item,
				  "activate",
				  G_CALLBACK (add_or_remove),
				  option_menu);
	
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
		gtk_widget_show (menu_item);
	}

	gtk_option_menu_set_menu (GTK_OPTION_MENU (option_menu), menu);
}

static void
gedit_encodings_option_menu_init (GeditEncodingsOptionMenu *menu)
{
	menu->priv = GEDIT_ENCODINGS_OPTION_MENU_GET_PRIVATE (menu);
}

GtkWidget *
gedit_encodings_option_menu_new (gboolean save_mode)
{
	return g_object_new (GEDIT_TYPE_ENCODINGS_OPTION_MENU, 
			     "save_mode", save_mode,
			     NULL);
}

const GeditEncoding *
gedit_encodings_option_menu_get_selected_encoding (GeditEncodingsOptionMenu *menu)
{
	GtkWidget *active_widget;

	GtkOptionMenu *option_menu;

	g_return_val_if_fail (GEDIT_IS_ENCODINGS_OPTION_MENU (menu), NULL);

	option_menu = GTK_OPTION_MENU (menu);
	g_return_val_if_fail (option_menu != NULL, NULL);
	
	if (option_menu->menu)
    	{
      		active_widget = gtk_menu_get_active (GTK_MENU (option_menu->menu));

	      	if (active_widget != NULL)
		{
			const GeditEncoding *ret;

			ret = (const GeditEncoding *)g_object_get_data (G_OBJECT (active_widget), 
									ENCODING_KEY);

			return ret;
		}
	}

	return NULL;
}

void
gedit_encodings_option_menu_set_selected_encoding (GeditEncodingsOptionMenu *menu,
						   const GeditEncoding      *encoding)
{
	GtkOptionMenu *option_menu;
	GList *list;
	GList *l;
	gint i;

	g_return_if_fail (GEDIT_IS_ENCODINGS_OPTION_MENU (menu));

	option_menu = GTK_OPTION_MENU (menu);
	g_return_if_fail (option_menu != NULL);

	list = gtk_container_get_children (GTK_CONTAINER (option_menu->menu));
	i = 0;
	for (l = list; l != NULL; l = g_list_next (l))
	{
		GtkWidget *menu_item;
		const GeditEncoding *enc;

		menu_item = GTK_WIDGET (l->data);

		enc = (const GeditEncoding *)g_object_get_data (G_OBJECT (menu_item), 
								ENCODING_KEY);

		if (enc == encoding)
		{
			gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (menu_item), TRUE);
			gtk_option_menu_set_history (GTK_OPTION_MENU (menu), i);
			break;
		}

		++i;
	}

	g_list_free (list);
}
