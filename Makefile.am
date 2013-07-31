ACLOCAL_AMFLAGS = -Im4

SUBDIRS = wibble meteo-vm2 fortran src doc

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = meteo-vm2.pc

.lua.luac:
	luac -o $@ $^

bufr.lua: default.lua
	lua $(srcdir)/default2bufr.lua $< > $@

sourcedir = $(sharedstatedir)/$(PACKAGE)/source

source_DATA = \
	      default.lua \
	      default.luac \
	      bufr.lua \
	      bufr.luac

EXTRA_DIST = \
	     autogen.sh \
	     meteo-vm2.pc.in \
	     test/data/source-1.lua \
	     test/data/source-2.luac \
	     test/data/1.vm2 \
	     default.lua \
	     default2bufr.lua

CLEANFILES = \
	     default.luac \
	     bufr.lua \
	     bufr.luac
