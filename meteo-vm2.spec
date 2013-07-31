Name:           meteo-vm2
Version:        0.5
Release:        1%{?dist}
Summary:        C++ library for VM2 data 

License:        GPLv2+
URL:            http://www.arpa.emr.it/sim
Source:         %{name}-%{version}.tar.gz

BuildRequires:  libtool, pkgconfig, lua-devel >= 5.1, libdballe-devel >= 5.19
Requires:       lua >= 5.1

%description
VM2 decoding/encoding library

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%check
make check

%install
rm -rf $RPM_BUILD_ROOT
%make_install

%files
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so.*
%dir %{_sharedstatedir}/%{name}
%{_sharedstatedir}/%{name}/source/default.lua*
%{_sharedstatedir}/%{name}/source/bufr.lua*

%package devel
Summary:        C++ library for VM2 data - development files
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}

%description devel
VM2 decoding/encoding library - development files

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*
%{_libdir}/lib%{name}.a
%{_libdir}/lib%{name}.la
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/meteo-vm2.pc

%package doc
Summary:        C++ library for VM2 data - documentation

%description doc
VM2 decoding/encoding library - documentation

%files doc
%doc %{_docdir}/%{name}
%defattr(-,root,root,-)

%package fortran
Summary:        meteo-vm2 Fortran library
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}, cnf-devel

%description fortran
VM2 decoding/encoding library - Fortran bindings

%files fortran
%defattr(-,root,root,-)
%{_libdir}/lib%{name}-fortran.so.*

%package fortran-devel
Summary:        meteo-vm2 Fortran development library
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}, %{name}-fortran = %{?epoch:%epoch:}%{version}-%{release}

%description fortran-devel
VM2 decoding/encoding library - Fortran development files

%files fortran-devel
%defattr(-,root,root,-)
%{_includedir}/meteo-vm2-fortran.h
%{_libdir}/lib%{name}-fortran.a
%{_libdir}/lib%{name}-fortran.la
%{_libdir}/lib%{name}-fortran.so

%package utils
Summary:        meteo-vm2 utilities
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}, libdballe6 >= 6.0

%description utils
Collection of utilities for VM2 files

%files utils
%defattr(-,root,root,-)
%{_bindir}/meteo-vm2-to-bufr
%{_bindir}/bufr-to-meteo-vm2

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%changelog
* Wed Jul 31 2013 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.5-1%{dist}
- Attribute tables updated

* Fri Jul 26 2013 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.4-1%{dist}
- fixed bug in bufr-to-meteo-vm2
- messages with missing value are skipped in meteo-vm2-to-bufr

* Tue May 14 2013 Daniele Branchini <dbranchini@arpa.emr.it> - 0.3-2%{dist}
- fixed dep for libdballe6

* Fri May 10 2013 Daniele Branchini <dbranchini@arpa.emr.it> - 0.3-1%{dist}
- reflecting upstream changes

* Fri Nov 30 2012 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.1-3%{dist}
- %post and %postun directives

* Thu Nov 29 2012 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.1-2%{dist}
- Fortran bindings

* Tue Oct 30 2012 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.1-1%{dist}
- First version
