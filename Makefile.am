## Makefile.am -- Process this file with automake to produce Makefile.in
#-------------------------------------------------------------------------
#@beginlicenses@
#@license{contributors}{2001-2003}@
#
#  Permission to use, copy, distribute and modify this software and its  
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appears in all copies and that both that copyright
#  notice and this permission notice appear in supporting documentation.
# 
#  Other Contributors (see file AUTHORS) make(s) no representations about the suitability of this
#  software for any purpose. It is provided "as is" without express or implied
#  warranty.
#  
#  Copyright (C) 2001-2003 Other Contributors (see file AUTHORS)
#
#@endlicenses@
#-------------------------------------------------------------------------
AUTOMAKE_OPTIONS = subdir-objects

abs_top_builddir = @abs_top_builddir@
abs_builddir = @abs_builddir@
srcdir = @srcdir@
libgc = @libgc@
enable_gc = @enable_gc@

SUBDIRS = buildinfo libltdl opencxx scripts html testsuite examples

# Libtool specific
# ================

#
# "(...) the shell variable LIBTOOL_DEPS, that you can
# use to automatically update the libtool script if it becomes
# out-of-date." [libtool.info]
#
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

bin_SCRIPTS	= occ2

bin_PROGRAMS	= occ
occ_SOURCES	= MetaclassPreregistration.cc
occ_LDADD	= libocc.la
occ_LDFLAGS	= -export-dynamic
INCLUDES	= -I$(top_srcdir) $(INCLTDL)

lib_LTLIBRARIES	= libocc.la
libocc_la_SOURCES = \
		empty_libocc.cc
libocc_la_LIBADD = \
		opencxx/libocc_mop.la	\
		$(LIBLTDL) 		\
		$(libgc)

include_HEADERS	= mop.h

dist-hook:
	for dir in comp exec; do \
	    if ! test -d $(distdir)/testsuite/$${dir} ; then \
	      mkdir $(distdir)/testsuite/$${dir}; \
	    fi; \
	    for ext in mc cc gold; do \
	        cp -p $(srcdir)/testsuite/$${dir}/*.$${ext} \
	              $(distdir)/testsuite/$${dir}; \
	    done; \
	    cp -p $(srcdir)/testsuite/$${dir}/HOWTO-*.txt \
	          $(distdir)/testsuite/$${dir}; \
	done

install-data-local:
	if test -d $(libdir)/pkgconfig; then \
	  mkdir -p $(DESTDIR)$(libdir)/pkgconfig; \
	  cp -p opencxx.pc $(DESTDIR)$(libdir)/pkgconfig; fi
	cd $(DESTDIR)$(pkgincludedir) \
	    && rm -f gc-related \
	    && ln -s gc-related-gc-$(enable_gc) gc-related
uninstall-local:
	if test -d $(libdir)/pkgconfig; then \
	  if test -f $(libdir)/pkgconfig/opencxx.pc; then \
	    rm -f $(libdir)/pkgconfig/opencxx.pc; fi fi
	if test -d $(includedir)/opencxx/parser; then \
	  rm -f $(includedir)/opencxx/parser; fi
	if test -d $(includedir)/opencxx; then \
	  rm -f $(includedir)/opencxx; fi
	rm -rf $(datadir)/opencxx
libltdl/libltdl.la :
	cd libltdl && $(MAKE) libltdl.la

###################################################################

update-licenses:
	python tools/update-licenses.py \
	`ls *.h *.cc` \
	`find opencxx -name '*.cc' -or -name '*.h'` \
	Makefile.am configure.in COPYING \
	opencxx/parser/ptree-gen.in occ2.in occ2-test.in

distclean-local:
	rm -f opencxx/gc-related
	rm -f opencxx/parser/ptree-generated.h opencxx/parser/ptree-generated.cc
	rm -f occ2
	rm -rf occ2-test.dir

AUX_DIST = config.guess \
	config.sub \
	install-sh \
	ltmain.sh \
	missing \
	mkinstalldirs
#	ltconfig
EXTRA_DIST = \
	tools/addguards.in \
	doc/ref-appendix.html \
	doc/reference.pdf \
	opencxx.pc.in \
	opencxx.pc \
	bootstrap \
	FILES \
	TIPS

pkgdata_DATA = doc/ref-appendix.html \
	doc/reference.pdf

CLEANFILES = occ2-test occ2-test.passed

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in \
		       stamp-h.in
