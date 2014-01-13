ACLOCAL_AMFLAGS = -Im4

SUBDIRS = wibble meteo-vm2 fortran src conf doc

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = meteo-vm2.pc

EXTRA_DIST = \
	     autogen.sh \
	     meteo-vm2.pc.in \
	     test/data/source-1.lua \
	     test/data/1.vm2

test/data/source-2.lua: test/data/source-1.lua
	luac -o $@ $<

CLEANFILES = test/data/source-2.lua
