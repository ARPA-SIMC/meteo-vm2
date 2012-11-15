SUBDIRS = wibble meteo-vm2 doc

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = meteo-vm2.pc

default-source.luac: default-source.lua
	luac -o $@ $^

sourcedir = $(localstatedir)/lib/$(PACKAGE)

source_DATA = default-source.lua default-source.luac

EXTRA_DIST = \
	     meteo-vm2.pc.in \
	     test/data/source-1.lua \
	     test/data/source-2.luac \
	     default-source.lua
