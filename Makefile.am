ACLOCAL_AMFLAGS = -Im4

SUBDIRS = meteo-vm2 src conf doc

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = meteo-vm2.pc

EXTRA_DIST = \
	     autogen.sh \
	     meteo-vm2.pc.in \
	     test/data/source-1.lua \
	     test/data/1.vm2

simcstations-update:
	git pull && \
	for f in bufr.lua default.lua; \
	    do curl http://meteozen.metarpa/simcstations/meteo-vm2/$$f > conf/$$f; \
	done

version-update:
	grep ^AC_INIT configure.ac && \
	echo 'nuova versione?' && \
	read versione && \
	vim configure.ac fedora/SPECS/meteo-vm2.spec && \
	echo 'procedo col commit (Ctrl-D per fermare)?' && \
	read procedo && \
	git commit -a -m v$${versione} && \
	git tag v$${versione}-1 && \
	git push && \
	git push --tags
