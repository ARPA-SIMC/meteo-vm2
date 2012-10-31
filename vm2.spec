Name:           vm2
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
%doc
%{_libdir}/*
%{_includedir}/*
%{_libdir}/pkgconfig/vm2.pc



%changelog
* Tue Oct 30 2012 Emanuele Di Giacomo <edigiacomo@arpa.emr.it> - 0.1-1%{dist}
- First version
