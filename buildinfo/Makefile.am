
#
# Generate symbolic name of current build
#

noinst_PROGRAMS = libgc-version

libgc_version_SOURCES	= libgc-version.cc
libgc_version_LDADD	= @libgc@
libgc_version_DEPENDENCIES = ../Makefile
EXTRA_DIST = compiler-name.sh

all-local: build-name.txt

build-name.txt: libgc-version ../Makefile
	echo @target@ > target.txt
	./libgc-version >libgc-version.txt
	$(SHELL) $(srcdir)/compiler-name.sh $(CXX) >compiler-name.txt
	echo `cat target.txt`/`cat libgc-version.txt`/`cat compiler-name.txt`\
		> build-name.txt

CLEANFILES = build-name.txt target.txt libgc-version.txt compiler-name.txt

