pkgincludedir = $(includedir)/meteo-vm2

dist_pkginclude_HEADERS = \
			  source.h \
			  parser.h \
			  value.h

lib_LTLIBRARIES = libmeteo-vm2.la

libmeteo_vm2_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS) \
			   -DMETEO_VM2_DEFAULT_SOURCE='"$(sharedstatedir)/$(PACKAGE)/default-source.luac"'

libmeteo_vm2_la_LDFLAGS = $(LUA_LIBS) -lpthread

libmeteo_vm2_la_LIBADD = $(top_builddir)/wibble/libwibble.la

libmeteo_vm2_la_SOURCES = \
			  source.cc \
			  parser.cc

check_PROGRAMS = meteo-vm2-test

meteo_vm2_test_SOURCES = \
			 tut-main.cc \
			 parser-tut.cc \
			 source-tut.cc

meteo_vm2_test_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS) \
			  -DSRCDIR='"$(CURDIR)/$(srcdir)"' \
			  -DTOP_SRCDIR='"$(CURDIR)/$(top_srcdir)"'

meteo_vm2_test_LDADD = $(builddir)/libmeteo-vm2.la

TESTS = $(builddir)/meteo-vm2-test
