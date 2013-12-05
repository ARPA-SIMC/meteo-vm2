.lua.luac:
	luac -o $@ $^

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

dist_source_DATA = \
	      default.lua

source_DATA = \
	      default.luac

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

TESTS = test-source.sh
