SUBDIRS=gedit

FLEX=@PROG_FLEX@

BISON=@PROG_BISON@

GLIB_GENMARSHAL=@GLIB_GENMARSHAL@

INCLUDES = @ALL_NEEDED_CFLAGS@ 

LIBS = @ALL_NEEDED_LIBS@ gedit/libgedit.a

EXTRA_DIST = scaner.flex  scaner.bison  marshller.define

bin_PROGRAMS = linuxdo

autocompletion_SRC = $(srcdir)/scaner.tab.c $(srcdir)/scaner.tab.h $(srcdir)/scaner.lex.c $(srcdir)/scaner.lex.h


DISTCLEANFILES = $(autocompletion_SRC) marshller.c marshller.h

linuxdo_SOURCES = Linuxdo.h ide.h callbacks.h TreeView.h About.h autotools.h marshller.h xterm.h Editors.h
linuxdo_SOURCES += main.c callbacks.c TreeView.c About.c autotools.c marshller.c xterm.c Editors.c scaner.c

linuxdo_SOURCES += $(autocompletion_SRC)

$(srcdir)/scaner.lex.c $(srcdir)/scaner.lex.h : $(srcdir)/scaner.flex  $(srcdir)/scaner.tab.h
	cd $(srcdir) && $(FLEX)	-o scaner.lex.c scaner.flex
$(srcdir)/scaner.tab.c $(srcdir)/scaner.tab.h : $(srcdir)/scaner.bison
	cd $(srcdir) && $(BISON) $< --defines=scaner.tab.h -o scaner.tab.c 

autocomplete/libautocomplete.a : autocomplete/*
	cd autocomplete && $(MAKE)

$(srcdir)/marshller.c : marshller.define
	$(GLIB_GENMARSHAL) --body --prefix=g_marshal $< > $@

$(srcdir)/marshller.h: marshller.define
	$(GLIB_GENMARSHAL) --header --prefix=g_marshal $< > $@

test : linuxdo $(top_builddir)/usr/share/locale/ $(top_srcdir)/po/LinuxDo.pot
	./linuxdo --locale=$(top_builddir)/usr/share/locale/

$(top_srcdir)/po/LinuxDo.pot:
	cd 	$(top_builddir)/po && $(MAKE) 

$(top_builddir)/usr/share/locale/:
	cd $(top_builddir)/po && $(MAKE) install DESTDIR=$(top_builddir)
		
