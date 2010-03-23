
INCLUDES = @ALL_NEEDED_CFLAGS@ 

LIBS = @ALL_NEEDED_LIBS@

EXTRA_DIST = marshller.c marshller.h marshller.define

bin_PROGRAMS = linuxdo

autocompletion_SRC = scaner.h scaner.c

linuxdo_SOURCES = misc.h Linuxdo.h ide.h callbacks.h TreeView.h About.h SourceView.h autotools.h marshller.h xterm.h Editors.h
linuxdo_SOURCES += main.c linuxdo.c callbacks.c TreeView.c About.c SourceView.c autotools.c marshller.c xterm.c Editors.c
linuxdo_SOURCES += $(autocompletion_SRC)

autocomplete/libautocomplete.a : autocomplete/*
	cd autocomplete && $(MAKE)

$(srcdir)/marshller.c : marshller.define $(srcdir)/marshller.h
	cd $(srcdir) && glib-genmarshal --body --prefix=g_marshal marshller.define > marshller.c

$(srcdir)/marshller.h: marshller.define
	cd $(srcdir) && glib-genmarshal --header --prefix=g_marshal marshller.define > marshller.h
