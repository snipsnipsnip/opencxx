
EXTRA_DIST = occ2-test.in tester2.in
check: blackboxtests unittests

blackboxtests:
	./tester2

unittests: occ2-test.passed
	./occ2-test
	echo PASS > $@

occ2-test.passed : occ2-test	# occ2

occ2-test : $(srcdir)/occ2-test.in
	sed 's:\(@\)abs_top_builddir\(@\):@abs_top_builddir@:g' <$< >$@.tmp
	chmod +x $@.tmp
	mv $@.tmp $@

distclean-local:
	rm -f tester2 occ2-test
