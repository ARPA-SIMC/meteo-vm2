pkgincludedir = $(includedir)/vm2

dist_pkginclude_HEADERS = \
			  source.h \
			  parser.h \
			  value.h

lib_LTLIBRARIES = libvm2.la

libvm2_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS)

libvm2_la_LDFLAGS = $(LUA_LIBS) -lpthread

libvm2_la_LIBADD = $(top_builddir)/wibble/libwibble.la

libvm2_la_SOURCES = \
		    source.cc \
		    parser.cc

check_PROGRAMS = vm2-test

vm2_test_SOURCES = \
		   tut-main.cc \
		   parser-tut.cc \
		   source-tut.cc

vm2_test_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(LUA_CFLAGS) \
		    -DSRCDIR='"$(CURDIR)/$(srcdir)"' \
		    -DTOP_SRCDIR='"$(CURDIR)/$(top_srcdir)"'

vm2_test_LDADD = $(builddir)/libvm2.la

TESTS = $(builddir)/vm2-test
