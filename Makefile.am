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
	done && \
	echo "### AGGIORNAMENTI ###" && \
	git diff && \
	read -p "Creo un nuovo pacchetto? (Ctrl-D per fermare) " procedo && \
	echo "### VERSIONE ATTUALE ###" && \
	grep ^AC_INIT configure.ac && \
	read -p "Nuova versione? (Ctrl-D per fermare) " nuova_versione && \
	vim configure.ac fedora/SPECS/meteo-vm2.spec && \
	echo "### AGGIORNAMENTI ###" && \
	git diff && \
	read -p "Procedo col commit? (Ctrl-D per fermare) " procedo && \
	git commit -a -m "v$${nuova_versione}" && \
	git tag "v$${nuova_versione}-1" && \
	git push && \
	git push --tags
