SUBDIRS = doc icons po src

EXTRA_DIST = m4 


test : $(top_builddir)/src/linuxdo $(top_srcdir)/po/LinuxDo.pot $(top_builddir)/usr/share/locale/	 
	$(top_builddir)/src/linuxdo 

$(top_builddir)/usr/share/locale/:
	cd po && $(MAKE) install DESTDIR=..

$(top_builddir)/src/linuxdo : $(top_srcdir)/src/* $(top_srcdir)/icons/*
	cd src && $(MAKE) linuxdo
