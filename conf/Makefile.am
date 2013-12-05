AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

.lua.luac:
	luac -o $@ $^

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

dist_source_DATA = \
	      default.lua

source_DATA = \
	      default.luac

check_PROGRAMS = test-variables

test_variables_SOURCES = test-variables.cc

test_variables_LDADD = @top_builddir@/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

check_SCRIPTS = test-source.sh

test-source.sh: test-source.sh.in
	sed \
		-e 's,[@]srcdir[@],$(srcdir),g' \
		-e 's,[@]top_srcdir[@],$(top_srcdir),g' \
		-e 's,[@]builddir[@],$(builddir),g' \
		-e 's,[@]top_builddir[@],$(top_builddir),g' \
		$< > $@
	chmod ugo+x $@

EXTRA_DIST = \
	     test-source.sh.in

CLEANFILES = \
	     default.luac \
	     test-source.sh

TESTS = test-variables test-source.sh

TESTS_ENVIRONMENT = @srcdir@/run-test
