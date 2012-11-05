if HAVE_DOXYGEN

Doxygen.stamp: Doxygen vm2.dox
	doxygen Doxygen
	date -u > $@

all-local: Doxygen.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)/html
	find doxygen/html -type f -exec $(INSTALL_DATA) '{}' $(DESTDIR)$(docdir)/html \;

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)/html

clean-local:
	rm -rf $(builddir)/doxygen

CLEANFILES = Doxygen.stamp

endif

examplesdir = $(docdir)/examples

examples_DATA = \
		examples/generate-source.lua \
		examples/source.lua \
		examples/values.vm2 \
		examples/oracle-source.py


EXTRA_DIST = \
	     Doxygen.in \
	     vm2.dox \
	     examples/generate-source.lua \
	     examples/source.lua \
	     examples/values.vm2 \
	     examples/oracle-source.py
