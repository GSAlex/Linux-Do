
AM_CFLAGS = @GTK2_CFLAGS@ @GTKSOURCEVIEW2_CFLAGS@

AM_LDFLAGS = @GTK2_LIBS@  @GTKSOURCEVIEW2_LIBS@

bin_PROGRAMS = linuxdo

linuxdo_SOURCES =  main.c Linuxdo.h

test : $(top_srcdir)/po/LinuxDo.pot $(builddir)/usr/share/locale/	 
	$(builddir)/linuxdo 

$(builddir)/usr/share/locale/:
	cd ../po && $(MAKE) install DESTDIR=$(builddir)