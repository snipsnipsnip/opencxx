
HTML_FILES = base.html class.html env.html green-pin.gif green.gif \
    index.html instance-of.gif logo.gif man.html member.html mop.html \
    occ.gif overview.html ptree.html red-pin.gif red.gif typeinfo.html \
    yellow-pin.gif yellow.gif

prefix = @prefix@
top_srcdir = @top_srcdir@
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs

EXTRA_DIST = $(HTML_FILES)

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/opencxx/html
	for f in $(HTML_FILES); do \
		$(INSTALL_DATA) $(top_srcdir)/html/$$f $(DESTDIR)$(datadir)/opencxx/html/$$f; \
	done

uninstall-local:
	for f in $(HTML_FILES); do \
		rm -rf $(DESTDIR)$(datadir)/opencxx/html; \
	done
