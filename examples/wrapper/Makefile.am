
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = sync-test SyncClass.exe
EXTRA_DIST = Makefile.local
sync_test_SOURCES = sync-test.cc
SyncClass_exe_SOURCES = SyncClass.mc WrapperClass.cc WrapperClass.h

sync-test : $(srcdir)/sync-test.cc SyncClass.exe
	./SyncClass.exe -- -o $@ $(srcdir)/sync-test.cc

WrapperClass.o : $(srcdir)/WrapperClass.cc $(srcdir)/WrapperClass.h
	$(OCC) -c $(srcdir)/WrapperClass.cc -o $@

SyncClass.o: $(srcdir)/SyncClass.mc $(srcdir)/WrapperClass.h
	$(OCC) -c $(srcdir)/SyncClass.mc -o $@

SyncClass.exe : WrapperClass.o SyncClass.o
	$(CXX) WrapperClass.o SyncClass.o -locc -lltdl -o $@

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/WrapperClass.cc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/WrapperClass.h $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/SyncClass.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/sync-test.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.lo *.ii *.so *.occ *.exe *.feedback *-init.cc a.out \
	      sync-test
