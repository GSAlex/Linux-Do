SUBDIRS = m4 doc icons po src 


test : all $(top_srcdir)/po/LinuxDo.pot $(top_builddir)/usr/share/locale/	 
	$(top_builddir)/src/linuxdo 

$(top_builddir)/usr/share/locale/:
	cd po && $(MAKE) install DESTDIR=..