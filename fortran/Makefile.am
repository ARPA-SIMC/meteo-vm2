dist_include_HEADERS = meteo-vm2-fortran.h

lib_LTLIBRARIES = libmeteo-vm2-fortran.la

libmeteo_vm2_fortran_la_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

libmeteo_vm2_fortran_la_LIBADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la -lcnf

libmeteo_vm2_fortran_la_SOURCES = meteo-vm2-fortran.cc

check_PROGRAMS = test

test_SOURCES = test.f90

test_FCFLAGS = -I$(top_srcdir) -I$(top_builddir)

test_LDADD = $(builddir)/libmeteo-vm2-fortran.la

TESTS = test

TESTS_ENVIRONMENT = $(srcdir)/run-test.sh

EXTRA_DIST = run-test.sh
