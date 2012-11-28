SUBDIRS = wibble meteo-vm2 fortran doc

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = meteo-vm2.pc

default.luac: default.lua
	luac -o $@ $^

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

source_DATA = default.lua default.luac

EXTRA_DIST = \
	     meteo-vm2.pc.in \
	     test/data/source-1.lua \
	     test/data/source-2.luac \
	     test/data/1.vm2 \
	     default.lua

CLEANFILES = default.luac
