
CXX = @CXX@
OCC = occ2

EXTRA_PROGRAMS = matrix-test MatrixClass.exe
EXTRA_DIST = Makefile.local
matrix_test_SOURCES = matrix-test.cc matrix.cc matrix.h
MatrixClass_exe_SOURCES = MatrixClass.mc

matrix-test : $(srcdir)/matrix-test.cc MatrixClass.exe matrix.o
	./MatrixClass.exe -- matrix.o -g -o $@ $(srcdir)/matrix-test.cc 

matrix.o : $(srcdir)/matrix.cc $(srcdir)/matrix.h
	$(CXX) -c $<

MatrixClass.exe : $(srcdir)/MatrixClass.mc
	$(OCC) $(srcdir)/MatrixClass.mc -o MatrixClass.exe

install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/Makefile.local $(DESTDIR)$(pkgdatadir)/$(subdir)/Makefile \
	&& cp $(srcdir)/MatrixClass.mc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/matrix.h $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/matrix.cc $(DESTDIR)$(pkgdatadir)/$(subdir) \
	&& cp $(srcdir)/matrix-test.cc $(DESTDIR)$(pkgdatadir)/$(subdir)

clean :
	rm -f *~ *.o *.lo *.ii *.so *.occ *.exe *.feedback *-init.cc a.out \
	      matrix-test
