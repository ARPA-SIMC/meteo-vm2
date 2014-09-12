AM_CPPFLAGS = -DMETEO_VM2_BUFR_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/bufr.luac"'

DEVEL_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I$(srcdir) $(LUA_CFLAGS)

if DBALLE

DEVEL_CPPFLAGS += $(DBALLE_CFLAGS)

bin_PROGRAMS = \
	       meteo-vm2-to-bufr \
	       bufr-to-meteo-vm2

noinst_LTLIBRARIES = libutils.la

libutils_la_SOURCES = utils.cc

noinst_HEADERS = utils.h

meteo_vm2_to_bufr_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

meteo_vm2_to_bufr_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS) $(builddir)/libutils.la

meteo_vm2_to_bufr_SOURCES = meteo-vm2-to-bufr.cc

bufr_to_meteo_vm2_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

bufr_to_meteo_vm2_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS) $(builddir)/libutils.la

bufr_to_meteo_vm2_SOURCES = bufr-to-meteo-vm2.cc

endif
