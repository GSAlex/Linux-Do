
INCLUDES = @ALL_NEEDED_CFLAGS@ 

LIBS = @ALL_NEEDED_LIBS@ 

EXTRA_DIST = marshller.c marshller.h marshller.define

bin_PROGRAMS = linuxdo

linuxdo_SOURCES = misc.h Linuxdo.h ide.h callbacks.h TreeView.h About.h SourceView.h autotools.h marshller.h xterm.h
linuxdo_SOURCES += main.c linuxdo.c callbacks.c TreeView.c About.c SourceView.c autotools.c marshller.c xterm.c

#linuxdo_EXTRA_DEPENDENCIES = $(srcdir)/marshller.h

$(srcdir)/marshller.c : marshller.define $(srcdir)/marshller.h
	cd $(srcdir) && glib-genmarshal --body --prefix=g_cclosure_marshal marshller.define > marshller.c

$(srcdir)/marshller.h: marshller.define
	cd $(srcdir) && glib-genmarshal --header --prefix=g_cclosure_marshal marshller.define > marshller.h
