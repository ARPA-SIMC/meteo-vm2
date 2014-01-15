pkgincludedir = $(includedir)/meteo-vm2

dist_pkginclude_HEADERS = \
			  source.h \
			  parser.h \
			  value.h

lib_LTLIBRARIES = libmeteo-vm2.la

libmeteo_vm2_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS) \
			   -DMETEO_VM2_DEFAULT_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/default.luac"'

libmeteo_vm2_la_LDFLAGS = -lpthread -version-info 0:0:0

libmeteo_vm2_la_LIBADD = $(LUA_LIBS)

libmeteo_vm2_la_SOURCES = \
			  source.cc \
			  parser.cc

check_PROGRAMS = meteo-vm2-test

meteo_vm2_test_SOURCES = \
			 tut-main.cc \
			 parser-tut.cc \
			 source-tut.cc

meteo_vm2_test_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS) \
			  -DSRCDIR='"$(abs_srcdir)"' \
			  -DTOP_SRCDIR='"$(abs_top_srcdir)"' \
			  -DBUILDDIR='"$(abs_builddir)"' \
			  -DTOP_BUILDDIR='"$(abs_top_builddir)"'

meteo_vm2_test_LDADD = $(builddir)/libmeteo-vm2.la $(LUA_LIBS)

TESTS = $(builddir)/meteo-vm2-test
