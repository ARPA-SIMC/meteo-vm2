DEVEL_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I$(srcdir) $(LUA_CFLAGS) \
				 -DMETEO_VM2_BUFR_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/bufr.luac"'

bin_PROGRAMS =
bin_SCRIPTS =

man_MANS =

check_SCRIPTS = tests.sh

CLEANFILES =

EXTRA_DIST = meteo-vm2-update-source

if DBALLE

DEVEL_CPPFLAGS += $(DBALLE_CFLAGS)

bin_PROGRAMS += \
				meteo-vm2-to-bufr \
				bufr-to-meteo-vm2

TESTS = tests.sh

TESTS_ENVIRONMENT = builddir=$(builddir)

if HELP2MAN

man_MANS += \
			meteo-vm2-to-bufr.1 \
			bufr-to-meteo-vm2.1

endif

CLEANFILES += \
			  meteo-vm2-to-bufr.1 \
			  bufr-to-meteo-vm2.1

%.1: %
	help2man -N $(builddir)/$< > $@

meteo_vm2_to_bufr_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

meteo_vm2_to_bufr_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

meteo_vm2_to_bufr_SOURCES = meteo-vm2-to-bufr.cc

bufr_to_meteo_vm2_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

bufr_to_meteo_vm2_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

bufr_to_meteo_vm2_SOURCES = bufr-to-meteo-vm2.cc

endif

if HAVE_CURL

bin_SCRIPTS += meteo-vm2-update-source

endif
