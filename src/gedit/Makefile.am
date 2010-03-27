## Process this file with automake to produce Makefile.in

GLIB_MKENUMS = /usr/bin/glib-mkenums
GLIB_GENMARSHAL = /usr/bin/glib-genmarshal

noinst_LIBRARIES = libgedit.a

INCLUDES =								\
	-I$(top_srcdir)							\
	-I$(srcdir)							\
	-I$(top_srcdir)/src							\
	$(ALL_NEEDED_CFLAGS)							\
	-DDATADIR=\""$(datadir)"\"					\
	-DLIBDIR=\""$(libdir)"\"					

LIBS = @ALL_NEEDED_LIBS@

NOINST_H_FILES =			\
	gedit-language-manager.h	\
	gedit-style-scheme-manager.h	\
	gedit-plugin-info.h		\
	gedit-plugin-info-priv.h	\
	gedit-plugin-manager.h		\
	gedit-plugins-engine.h		\
	gedit-object-module.h		\
	gedit-ui.h			\
	gedit-window-private.h		\
	gedit-documents-panel.h		\
	gedit-document-loader.h		\
	gedit-gio-document-loader.h	\
	gedit-document-saver.h		\
	gedit-local-document-saver.h	\
	gedit-gio-document-saver.h	\
	gedit-history-entry.h		\
	gedit-print-job.h		\
	gedit-print-preview.h		\
	gedit-io-error-message-area.h	\
	gedit-spinner.h			\
	gedit-prefs-manager-private.h	\
	gedittextregion.h		\
	gedit-session.h			\
	gedit-dirs.h

INST_H_FILES =				\
	gedit-commands.h		\
	gedit-convert.h			\
	gedit-debug.h			\
	gedit-document.h 		\
	gedit-encodings.h		\
	gedit-encodings-option-menu.h	\
	gedit-file-chooser-dialog.h	\
	gedit-help.h 			\
	gedit-message-bus.h		\
	gedit-message-type.h		\
	gedit-message.h			\
	gedit-metadata-manager.h	\
	gedit-notebook.h		\
	gedit-panel.h			\
	gedit-plugin.h			\
	gedit-plugin-loader.h		\
	gedit-prefs-manager-app.h	\
	gedit-prefs-manager.h		\
	gedit-progress-message-area.h	\
	gedit-statusbar.h		\
	gedit-status-combo-box.h	\
	gedit-tab.h 			\
	gedit-utils.h 			\
	gedit-view.h 			\
	gedit-window.h

headerdir = $(prefix)/include/gedit-@GEDIT_API_VERSION@/gedit

header_DATA = 				\
	$(INST_H_FILES)	


POSIXIO_FILES = 			\
	gedit-local-document-saver.c	



libgedit_a_SOURCES = 			\
	$(POSIXIO_FILES)		\
	gedit-convert.c			\
	gedit-debug.c			\
	gedit-dirs.c			\
	gedit-document.c 		\
	gedit-document-loader.c		\
	gedit-gio-document-loader.c	\
	gedit-document-saver.c		\
	gedit-gio-document-saver.c	\
	gedit-documents-panel.c		\
	gedit-encodings.c		\
	gedit-file-chooser-dialog.c	\
	gedit-io-error-message-area.c	\
	gedit-language-manager.c	\
	gedit-message-bus.c		\
	gedit-message-type.c		\
	gedit-message.c			\
	gedit-metadata-manager.c	\
	gedit-object-module.c		\
	gedit-prefs-manager-app.c	\
	gedit-prefs-manager.c		\
	gedit-prefs-manager-private.h	\
	gedit-progress-message-area.c	\
	gedit-spinner.c			\
	gedit-statusbar.c		\
	gedit-status-combo-box.c	\
	gedit-style-scheme-manager.c	\
	gedit-tab.c 			\
	gedit-utils.c 			\
	gedit-view.c 			\
	gedit-window.c			\
	gedittextregion.c		\
	gedit-enum-types.c gedit-enum-types.h \
	$(NOINST_H_FILES)		\
	$(INST_H_FILES) 
	
EXTRA_DIST = 				\
	gedit-enum-types.h.template	\
	gedit-enum-types.c.template	\
	gedit-marshal.list		\
	gedit.rc

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)


BACON_DIR=$(srcdir)/../../libbacon/src/
BACON_FILES=bacon-message-connection.h bacon-message-connection.c

regenerate-built-sources:
	BACONFILES="$(BACON_FILES)" BACONDIR="$(BACON_DIR)" $(top_srcdir)/gedit/update-from-bacon.sh

-include $(top_srcdir)/git.mk
