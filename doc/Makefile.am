if HAVE_DOXYGEN

Doxygen.stamp: Doxygen meteo-vm2.dox
	$(DOXYGEN) Doxygen
	date -u > $@

all-local: Doxygen.stamp

install-data-local: install-doxygen-html

uninstall-local: uninstall-doxygen-html

install-doxygen-html:
	test -z ${htmldir} || $(mkinstalldirs) ${htmldir}
	cp -r $(builddir)/doxygen/html ${docdir}

uninstall-doxygen-html:
	$(RM) -r ${docdir}/html

clean-local:
	$(RM) -r $(builddir)/doxygen

CLEANFILES = Doxygen.stamp

endif

examplesdir = $(docdir)/examples

examples_DATA = \
		examples/generate-source.lua \
		examples/source.lua \
		examples/values.vm2 \
		examples/oracle-source.py \
		examples/parser.f90 \
		examples/source.f90 \
		examples/bufr.lua


EXTRA_DIST = \
	     Doxygen.in \
	     meteo-vm2.dox \
	     examples/generate-source.lua \
	     examples/source.lua \
	     examples/values.vm2 \
	     examples/oracle-source.py \
	     examples/parser.f90 \
	     examples/source.f90 \
	     examples/bufr.lua
