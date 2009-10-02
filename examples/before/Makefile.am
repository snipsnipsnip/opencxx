
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = before-test BeforeClass.exe
EXTRA_DIST = Makefile.local
before_test_SOURCES = before-test.cc
BeforeClass_exe_SOURCES = BeforeClass.mc

before-test : $(srcdir)/before-test.cc BeforeClass.exe
	./BeforeClass.exe -- -g -o $@ $(srcdir)/before-test.cc

BeforeClass.exe : $(srcdir)/BeforeClass.mc
	$(OCC) $(srcdir)/BeforeClass.mc -o $@

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/examples/before \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/BeforeClass.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/before-test.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.ii *.lo *.so *.occ *.exe *.feedback *-init.cc a.out \
		before-test
