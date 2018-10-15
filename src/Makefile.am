DEVEL_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I$(srcdir) $(LUA_CFLAGS) \
				 -DMETEO_VM2_BUFR_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/bufr.luac"'

bin_PROGRAMS =

man_MANS =

CLEANFILES = 

if DBALLE

DEVEL_CPPFLAGS += $(DBALLE_CFLAGS)

bin_PROGRAMS += \
				meteo-vm2-to-bufr \
				bufr-to-meteo-vm2 \
				meteo-vm2-manual-validation

if HELP2MAN

man_MANS += \
			meteo-vm2-to-bufr.1 \
			bufr-to-meteo-vm2.1 \
			meteo-vm2-manual-validation.1

endif

CLEANFILES += \
			  meteo-vm2-to-bufr.1 \
			  bufr-to-meteo-vm2.1 \
			  meteo-vm2-manual-validation.1

%.1: %
	help2man -N $(builddir)/$< > $@

meteo_vm2_to_bufr_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

meteo_vm2_to_bufr_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

meteo_vm2_to_bufr_SOURCES = meteo-vm2-to-bufr.cc

bufr_to_meteo_vm2_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

bufr_to_meteo_vm2_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

bufr_to_meteo_vm2_SOURCES = bufr-to-meteo-vm2.cc

meteo_vm2_manual_validation_CPPFLAGS = $(DEVEL_CPPFLAGS) $(AM_CPPFLAGS)

meteo_vm2_manual_validation_LDADD = $(top_builddir)/meteo-vm2/libmeteo-vm2.la

meteo_vm2_manual_validation_SOURCES = meteo-vm2-manual-validation.cc

endif
