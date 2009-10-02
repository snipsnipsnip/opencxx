
bin_SCRIPTS = libocc-config

#
# we need this to make sure, that libocc-config 
# in its build directory is executable 
# (so that tests can run it from build dir, not bindir)
# thus we introduce additional dependency on all
# whicih causes 'chmod' to be run
#

all-local: libocc-config
	chmod +x libocc-config

#
# Some installs do not set exec permissions for files that they
# do not recognize as executables, so we need to do it by hand, 
# when intalling
#
install-exec-local:
	-chmod a+x $(DESTDIR)$(bindir)/libocc-config
