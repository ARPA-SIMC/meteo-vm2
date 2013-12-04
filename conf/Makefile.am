.lua.luac:
	luac -o $@ $^

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

dist_source_DATA = \
	      default.lua

source_DATA = \
	      default.luac

CLEANFILES = \
	     default.luac

dist_check_SCRIPTS = test-source.sh

TESTS = test-source.sh
