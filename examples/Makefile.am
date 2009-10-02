
SUBDIRS = before matrix runtime-mop verbose verbose2 wrapper
EXTRA_DIST = Readme Makefile.global.in Makefile.global

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.global $(DESTDIR)$(pkgdatadir)/$(subdir)
