AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(LUA_CFLAGS)

.lua.luac:
	luac -o $@ $^

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

dist_source_DATA = default.lua bufr.lua

source_DATA = default.luac bufr.luac

tut_main_SOURCES = \
		   tut-main.cc \
		   conf-tut.cc

tut_main_LDADD = @top_builddir@/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS) $(LUA_LIBS)

if DBALLE

check_PROGRAMS = tut-main

TESTS = tut-main

TESTS_ENVIRONMENT = @srcdir@/run-test

endif

CLEANFILES = default.luac bufr.luac

EXTRA_DIST = run-test
