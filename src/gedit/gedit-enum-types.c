
/* Generated data (by glib-mkenums) */

#include "gedit-enum-types.h"

/* enumerations from "gedit-convert.h" */
#include "gedit-convert.h"

GType
gedit_convert_error_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GEnumValue values[] = {
			{ GEDIT_CONVERT_ERROR_AUTO_DETECTION_FAILED,
			  "GEDIT_CONVERT_ERROR_AUTO_DETECTION_FAILED",
			  "failed" },
			{ 0, NULL, NULL }
		};
		the_type = g_enum_register_static (
				g_intern_static_string ("GeditConvertError"),
				values);
	}
	return the_type;
}

/* enumerations from "gedit-debug.h" */
#include "gedit-debug.h"

GType
gedit_debug_section_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GFlagsValue values[] = {
			{ GEDIT_NO_DEBUG,
			  "GEDIT_NO_DEBUG",
			  "no-debug" },
			{ GEDIT_DEBUG_VIEW,
			  "GEDIT_DEBUG_VIEW",
			  "debug-view" },
			{ GEDIT_DEBUG_SEARCH,
			  "GEDIT_DEBUG_SEARCH",
			  "debug-search" },
			{ GEDIT_DEBUG_PRINT,
			  "GEDIT_DEBUG_PRINT",
			  "debug-print" },
			{ GEDIT_DEBUG_PREFS,
			  "GEDIT_DEBUG_PREFS",
			  "debug-prefs" },
			{ GEDIT_DEBUG_PLUGINS,
			  "GEDIT_DEBUG_PLUGINS",
			  "debug-plugins" },
			{ GEDIT_DEBUG_TAB,
			  "GEDIT_DEBUG_TAB",
			  "debug-tab" },
			{ GEDIT_DEBUG_DOCUMENT,
			  "GEDIT_DEBUG_DOCUMENT",
			  "debug-document" },
			{ GEDIT_DEBUG_COMMANDS,
			  "GEDIT_DEBUG_COMMANDS",
			  "debug-commands" },
			{ GEDIT_DEBUG_APP,
			  "GEDIT_DEBUG_APP",
			  "debug-app" },
			{ GEDIT_DEBUG_SESSION,
			  "GEDIT_DEBUG_SESSION",
			  "debug-session" },
			{ GEDIT_DEBUG_UTILS,
			  "GEDIT_DEBUG_UTILS",
			  "debug-utils" },
			{ GEDIT_DEBUG_METADATA,
			  "GEDIT_DEBUG_METADATA",
			  "debug-metadata" },
			{ GEDIT_DEBUG_WINDOW,
			  "GEDIT_DEBUG_WINDOW",
			  "debug-window" },
			{ GEDIT_DEBUG_LOADER,
			  "GEDIT_DEBUG_LOADER",
			  "debug-loader" },
			{ GEDIT_DEBUG_SAVER,
			  "GEDIT_DEBUG_SAVER",
			  "debug-saver" },
			{ 0, NULL, NULL }
		};
		the_type = g_flags_register_static (
				g_intern_static_string ("GeditDebugSection"),
				values);
	}
	return the_type;
}

/* enumerations from "gedit-document.h" */
#include "gedit-document.h"

GType
gedit_search_flags_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GFlagsValue values[] = {
			{ GEDIT_SEARCH_DONT_SET_FLAGS,
			  "GEDIT_SEARCH_DONT_SET_FLAGS",
			  "dont-set-flags" },
			{ GEDIT_SEARCH_ENTIRE_WORD,
			  "GEDIT_SEARCH_ENTIRE_WORD",
			  "entire-word" },
			{ GEDIT_SEARCH_CASE_SENSITIVE,
			  "GEDIT_SEARCH_CASE_SENSITIVE",
			  "case-sensitive" },
			{ 0, NULL, NULL }
		};
		the_type = g_flags_register_static (
				g_intern_static_string ("GeditSearchFlags"),
				values);
	}
	return the_type;
}

GType
gedit_document_save_flags_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GFlagsValue values[] = {
			{ GEDIT_DOCUMENT_SAVE_IGNORE_MTIME,
			  "GEDIT_DOCUMENT_SAVE_IGNORE_MTIME",
			  "ignore-mtime" },
			{ GEDIT_DOCUMENT_SAVE_IGNORE_BACKUP,
			  "GEDIT_DOCUMENT_SAVE_IGNORE_BACKUP",
			  "ignore-backup" },
			{ GEDIT_DOCUMENT_SAVE_PRESERVE_BACKUP,
			  "GEDIT_DOCUMENT_SAVE_PRESERVE_BACKUP",
			  "preserve-backup" },
			{ 0, NULL, NULL }
		};
		the_type = g_flags_register_static (
				g_intern_static_string ("GeditDocumentSaveFlags"),
				values);
	}
	return the_type;
}

/* enumerations from "gedit-prefs-manager.h" */
#include "gedit-prefs-manager.h"

GType
gedit_toolbar_setting_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GEnumValue values[] = {
			{ GEDIT_TOOLBAR_SYSTEM,
			  "GEDIT_TOOLBAR_SYSTEM",
			  "system" },
			{ GEDIT_TOOLBAR_ICONS,
			  "GEDIT_TOOLBAR_ICONS",
			  "icons" },
			{ GEDIT_TOOLBAR_ICONS_AND_TEXT,
			  "GEDIT_TOOLBAR_ICONS_AND_TEXT",
			  "icons-and-text" },
			{ GEDIT_TOOLBAR_ICONS_BOTH_HORIZ,
			  "GEDIT_TOOLBAR_ICONS_BOTH_HORIZ",
			  "icons-both-horiz" },
			{ 0, NULL, NULL }
		};
		the_type = g_enum_register_static (
				g_intern_static_string ("GeditToolbarSetting"),
				values);
	}
	return the_type;
}

/* enumerations from "gedit-tab.h" */
#include "gedit-tab.h"

GType
gedit_tab_state_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GEnumValue values[] = {
			{ GEDIT_TAB_STATE_NORMAL,
			  "GEDIT_TAB_STATE_NORMAL",
			  "state-normal" },
			{ GEDIT_TAB_STATE_LOADING,
			  "GEDIT_TAB_STATE_LOADING",
			  "state-loading" },
			{ GEDIT_TAB_STATE_REVERTING,
			  "GEDIT_TAB_STATE_REVERTING",
			  "state-reverting" },
			{ GEDIT_TAB_STATE_SAVING,
			  "GEDIT_TAB_STATE_SAVING",
			  "state-saving" },
			{ GEDIT_TAB_STATE_PRINTING,
			  "GEDIT_TAB_STATE_PRINTING",
			  "state-printing" },
			{ GEDIT_TAB_STATE_PRINT_PREVIEWING,
			  "GEDIT_TAB_STATE_PRINT_PREVIEWING",
			  "state-print-previewing" },
			{ GEDIT_TAB_STATE_SHOWING_PRINT_PREVIEW,
			  "GEDIT_TAB_STATE_SHOWING_PRINT_PREVIEW",
			  "state-showing-print-preview" },
			{ GEDIT_TAB_STATE_GENERIC_NOT_EDITABLE,
			  "GEDIT_TAB_STATE_GENERIC_NOT_EDITABLE",
			  "state-generic-not-editable" },
			{ GEDIT_TAB_STATE_LOADING_ERROR,
			  "GEDIT_TAB_STATE_LOADING_ERROR",
			  "state-loading-error" },
			{ GEDIT_TAB_STATE_REVERTING_ERROR,
			  "GEDIT_TAB_STATE_REVERTING_ERROR",
			  "state-reverting-error" },
			{ GEDIT_TAB_STATE_SAVING_ERROR,
			  "GEDIT_TAB_STATE_SAVING_ERROR",
			  "state-saving-error" },
			{ GEDIT_TAB_STATE_GENERIC_ERROR,
			  "GEDIT_TAB_STATE_GENERIC_ERROR",
			  "state-generic-error" },
			{ GEDIT_TAB_STATE_CLOSING,
			  "GEDIT_TAB_STATE_CLOSING",
			  "state-closing" },
			{ GEDIT_TAB_STATE_EXTERNALLY_MODIFIED_NOTIFICATION,
			  "GEDIT_TAB_STATE_EXTERNALLY_MODIFIED_NOTIFICATION",
			  "state-externally-modified-notification" },
			{ GEDIT_TAB_NUM_OF_STATES,
			  "GEDIT_TAB_NUM_OF_STATES",
			  "num-of-states" },
			{ 0, NULL, NULL }
		};
		the_type = g_enum_register_static (
				g_intern_static_string ("GeditTabState"),
				values);
	}
	return the_type;
}

/* enumerations from "gedit-window.h" */
#include "gedit-window.h"

GType
gedit_window_state_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GFlagsValue values[] = {
			{ GEDIT_WINDOW_STATE_NORMAL,
			  "GEDIT_WINDOW_STATE_NORMAL",
			  "normal" },
			{ GEDIT_WINDOW_STATE_SAVING,
			  "GEDIT_WINDOW_STATE_SAVING",
			  "saving" },
			{ GEDIT_WINDOW_STATE_PRINTING,
			  "GEDIT_WINDOW_STATE_PRINTING",
			  "printing" },
			{ GEDIT_WINDOW_STATE_LOADING,
			  "GEDIT_WINDOW_STATE_LOADING",
			  "loading" },
			{ GEDIT_WINDOW_STATE_ERROR,
			  "GEDIT_WINDOW_STATE_ERROR",
			  "error" },
			{ GEDIT_WINDOW_STATE_SAVING_SESSION,
			  "GEDIT_WINDOW_STATE_SAVING_SESSION",
			  "saving-session" },
			{ 0, NULL, NULL }
		};
		the_type = g_flags_register_static (
				g_intern_static_string ("GeditWindowState"),
				values);
	}
	return the_type;
}


/* Generated data ends here */

