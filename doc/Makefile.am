if HAVE_DOXYGEN

DOXYFILES = \
	    meteo-vm2-fortranbinding.dox \
	    meteo-vm2-mainpage.dox \
	    meteo-vm2-vm2attributesfile.dox \
	    meteo-vm2-vm2valuefile.dox \
	    meteo-vm2-tools.dox

Doxygen.stamp: Doxygen $(DOXYFILES)
	$(DOXYGEN) Doxygen
	date -u > $@

all-local: Doxygen.stamp

install-data-local: install-doxygen-html

uninstall-local: uninstall-doxygen-html

install-doxygen-html:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	cp -r $(builddir)/doxygen/html $(DESTDIR)$(docdir)

uninstall-doxygen-html:
	$(RM) -r $(DESTDIR)$(docdir)/html

clean-local:
	$(RM) -r $(builddir)/doxygen

CLEANFILES = Doxygen.stamp

endif

examplesdir = $(docdir)/examples

dist_examples_DATA = \
		examples/generate-source.lua \
		examples/source.lua \
		examples/values.vm2 \
		examples/oracle-source.py \
		examples/parser.f90 \
		examples/source.f90 \
		examples/http-source.lua \
		examples/json-source-ws.py


EXTRA_DIST = \
	     Doxygen.in \
	     meteo-vm2-fortranbinding.dox \
	     meteo-vm2-mainpage.dox \
	     meteo-vm2-vm2attributesfile.dox \
	     meteo-vm2-vm2valuefile.dox \
	     meteo-vm2-tools.dox
