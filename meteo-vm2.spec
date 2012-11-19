Name:           meteo-vm2
Version:        0.1 
Release:        1%{?dist}
Summary:        C++ library for VM2 data 

License:        GPLv2+
URL:            http://www.arpa.emr.it/sim
Source:         %{name}-%{version}.tar.gz

BuildRequires:  libtool, pkgconfig, lua-devel >= 5.1
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
%{_libdir}/*.so.*
%{_sharedstatedir}/%{name}/source/default.lua*

%package devel
Summary:        C++ library for VM2 data - development files
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}

%description devel
VM2 decoding/encoding library - development files

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/meteo-vm2.pc

%package doc
Summary:        C++ library for VM2 data - documentation

%description doc
VM2 decoding/encoding library - documentation

%files doc
%doc %{_docdir}/%{name}/*
%defattr(-,root,root,-)

%changelog
* Tue Oct 30 2012 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.1-1%{dist}
- First version
