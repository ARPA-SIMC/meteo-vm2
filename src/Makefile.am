if DBALLE

bin_PROGRAMS = \
	       meteo-vm2-to-bufr \
	       bufr-to-meteo-vm2

meteo_vm2_to_bufr_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(DBALLE_CFLAGS) -DMETEO_VM2_BUFR_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/bufr.luac"'

meteo_vm2_to_bufr_LDADD = $(top_builddir)/wibble/libwibble.la $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

meteo_vm2_to_bufr_SOURCES = meteo-vm2-to-bufr.cc

bufr_to_meteo_vm2_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(DBALLE_CFLAGS) -DMETEO_VM2_BUFR_SOURCE='"$(sharedstatedir)/$(PACKAGE)/source/bufr.luac"'

bufr_to_meteo_vm2_LDADD = $(top_builddir)/wibble/libwibble.la $(top_builddir)/meteo-vm2/libmeteo-vm2.la $(DBALLE_LIBS)

bufr_to_meteo_vm2_SOURCES = bufr-to-meteo-vm2.cc

endif
