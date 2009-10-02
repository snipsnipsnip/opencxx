
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = metaobj-test RtmopClass.exe
EXTRA_DIST = Makefile.local
metaobj_test_SOURCES = metaobj-test.cc metaobj.h metaobj.cc
RtmopClass_exe_SOURCES = RtmopClass.mc RtmopClass.h

metaobj-test : RtmopClass.exe metaobj.o
	./RtmopClass.exe -- -g -o $@ metaobj.o $(srcdir)/metaobj-test.cc

metaobj.o : $(srcdir)/metaobj.cc $(srcdir)/metaobj.h
	$(CXX) -c $(srcdir)/metaobj.cc

RtmopClass.exe : $(srcdir)/RtmopClass.mc $(srcdir)/RtmopClass.h
	$(OCC) $< -o $@

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/RtmopClass.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/RtmopClass.h $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/metaobj.cc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/metaobj.h $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/metaobj-test.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.lo *.ii *.so *.occ *.exe *.feedback *-init.cc a.out \
	      metaobj-test
