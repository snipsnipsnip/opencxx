
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = person VerboseClass.exe
EXTRA_DIST = Makefile.local
person_SOURCES = person.cc
VerboseClass_exe_SOURCES = VerboseClass.mc

person : $(srcdir)/person.cc VerboseClass.exe
	./VerboseClass.exe -- -o $@ $(srcdir)/person.cc 

VerboseClass.exe : $(srcdir)/VerboseClass.mc
	$(OCC) $(srcdir)/VerboseClass.mc -o $@

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/VerboseClass.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/person.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.lo *.ii *.so *.occ *.exe *.feedback *-init.cc a.out \
	      person 
