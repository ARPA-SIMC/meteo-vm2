AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(LUA_CFLAGS)

.lua.luac:
	luac -o $@ $^

default.lua: meteo.json airquality.json
	@PYTHON@ json2lua.py $^ > $@

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

dist_source_DATA = \
	      default.lua

source_DATA = \
	      default.luac

check_PROGRAMS = tut-main

tut_main_SOURCES = \
		   tut-main.cc \
		   conf-tut.cc

tut_main_LDADD = @top_builddir@/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS) $(LUA_LIBS)

TESTS = tut-main

TESTS_ENVIRONMENT = @srcdir@/run-test

CLEANFILES = \
	     default.luac

EXTRA_DIST = run-test meteo.json airquality.json json2lua.py
