
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = person2 VerboseClass2.exe
EXTRA_DIST = Makefile.local
person2_SOURCES = person2.cc
VerboseClass2_exe_SOURCES = VerboseClass2.mc


person2 : $(srcdir)/person2.cc VerboseClass2.exe
	./VerboseClass2.exe -- -o $@ $(srcdir)/person2.cc 

VerboseClass2.exe : $(srcdir)/VerboseClass2.mc
	$(OCC) $(srcdir)/VerboseClass2.mc -o VerboseClass2.exe

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/VerboseClass2.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/person2.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.lo *.ii *.so *.occ *.exe *.feedback *-init.cc a.out \
	      person2 
