%global releaseno 0.1
# Note: define _srcarchivename in Travis build only.
%{!?srcarchivename: %global srcarchivename meteovm2-%{version}-%{releaseno}}
# Python 3 package names
%{?el7:%global python3_vers python36}
%{?fedora:%global python3_vers python3}


Name:           python-meteovm2
Version:        2.0.0
Release:        %{releaseno}
Summary:        Python library for VM2 data
License:        GPLv2+
URL:            https://github.com/arpa-simc/%{name}
Source0:        https://github.com/arpa-simc/%{name}/archive/v%{version}-%{release}.tar.gz#/%{srcarchivename}.tar.gz
BuildArch:      noarch


%description
Python library for VM2 data

%package -n %{python3_vers}-meteovm2
Summary:        %{summary}
BuildRequires:  %{python3_vers}-devel
BuildRequires:  %{python3_vers}-dballe >= 8
%{?python_provide:%python_provide %{python3_vers}-%{srcname}}

%description -n %{python3_vers}-meteovm2
Python3 library for VM2 data

%prep
%setup -q -n %{srcarchivename}


%build
%py3_build


%check
# %%py3_check

%install
%py3_install

%files -n %{python3_vers}-meteovm2
%defattr(-,root,root,-)
%{_bindir}/meteo-vm2-to-bufr
%{_bindir}/bufr-to-meteo-vm2
%{python3_sitelib}/meteovm2*


%changelog
* Mon Jul  8 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.3-2
- Fixed files section

* Tue Jul  4 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.3-1
- meteo-vm2-update-source

* Tue Jun 25 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.2-1
- Removed station Santa Maria Lavello
- Removed Ponte Orgia height
- Updated variabile 633
- New cro stations

* Tue Mar 19 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.1-1
- Updated fidupo network

* Wed Feb 27 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.0-1
- Support dballe8

* Mon Jan 21 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.71-1
- Updated stations with non-null ident

* Mon Jan 21 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.70-1
- Updated fiduli network

* Wed Jan 16 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.69-1
- Updated fidufv network

* Wed Dec  5 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.68-1
- Updated fiduto and fidufv networks

* Tue Nov 22 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.67-1
- Updated stations (fiduto)
- Removed variable 258

* Tue Nov 20 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.66-1
- Updated stations (fidubz)

* Wed Oct 31 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.65-1
- Updated stations (fiduma)

* Mon Oct 22 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.64-1
- Updated stations (fiduma)

* Mon Oct 22 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.63-1
- Updated stations (fiduma)

* Mon Oct 15 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.62-1
- Updated stations (fiduma, simnpr and claster networks)
- Added ParserException in public API
- Added wobble for testing

* Mon Sep  3 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.61-1
- Updated variables and stations

* Tue Jul 17 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.60-2
- Update dballe dependency

* Tue Jun 14 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.60-1
- Updated sea variables

* Tue Jun 12 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.59-1
- Updated stations

* Tue Jun  5 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.58-1
- Fixed pollen variables

* Thu May 31 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.57-1
- Updated variables (pollen)

* Thu May  3 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.56-1
- Updated stations (fidubz)

* Wed Mar 14 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.54-2
- Fixed variabile 351

* Tue Mar  6 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.53-2
- Updated stations

* Mon Mar  5 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.52-2
- Updated stations

* Wed Feb 28 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.51-1
- Updated stations

* Thu Feb  1 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.50-1
- Updated stations

* Tue Jan 16 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.49-1
- Update stations (network fiduum)

* Tue Dec 19 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.48-1
- Updated stations

* Fri Oct 13 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.47-1
- Updated stations (network marefe)

* Thu Sep 28 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.46-1
- Version bump

* Tue Jul 25 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.45-1
- Updated stations

* Fri Jun  9 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.44-1
- Updated variables and stations

* Thu Feb  2 2017 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.42-1
- Updated stations and variables

* Mon Dec 19 2016 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.40-1
- Updated stations

* Wed Nov 16 2016 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.39-1
- New stations and variables

* Tue Jul 19 2016 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.38-1
- New stations and variables
- Removed fortran bindings
- dballe7 support

* Tue Jan 26 2016 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.37-1
- New stations

* Mon Dec 28 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.36-1
- Stations from fidutn network 
- Updated stations and variables

* Wed Oct 14 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.35-1
- Fixed bufr-to-meteo-vm2 exit status

* Wed Oct 14 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.34-1%{dist}
- Pollen unit

* Tue Oct 13 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.33-1%{dist}
- Fixed error message
- Parser::regexp_str public again

* Mon Oct 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.32-1%{dist}
- Fixed bug in gcc-4.8 regex parser

* Mon Oct 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.31-1%{dist}
- Source updated

* Fri Oct 09 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.30-1%{dist}
- gcc 4.8.3 support

* Thu Oct 01 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.29-1%{dist}
- Removed wibble
- C++11

* Mon Aug 03 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.27-1%{dist}
- Back to old reports

* Fri Jun 19 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.26-1%{dist}
- dballe-7.1-4828

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25-3%{dist}
- Fix default.lua, again

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25-2%{dist}
- Fix default.lua

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25%{dist}
- default.lua and bufr.lua source files

* Thu Jun 04 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.24%{dist}
- New stations and variables
- dballe 7.1-4758 support

* Tue May 26 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.23%{dist}
- Disabled Python bindings

* Tue May 26 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.22%{dist}
- dballe-7.1-4749 support

* Tue May 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.21%{dist}
- Fixed l1, l2 and missing values as nil

* Mon May 11 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.20%{dist}
- New stations

* Fri Apr 17 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.19%{dist}
- New stations

* Thu Feb 19 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.18.1-1%{dist}
- Error message

* Wed Feb 18 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.18-1%{dist}
- Fixed parser

* Tue Sep 16 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.17-1%{dist}
- bufr-to-meteo-vm2: optional ident

* Tue Sep 16 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.16-1%{dist}
- Updated stations
- Stations data for BUFR conversion

* Wed Sep 03 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.15-1%{dist}
- New stations 1034, 1035, 1036

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.14-1%{dist}
- Fixed stations

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.13-2%{dist}
- Fixed lua dependency

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.13-1%{dist}
- Updated stations

* Mon Jun 23 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.12-1%{dist}
- Stations 4217,4222,4240,4247,4344 to idrmec-pub
- Station 13023 to rer

* Thu Jun 19 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.11-2%{dist}
- meteo-vm2-to-bufr: fixed error message

* Thu Jun 19 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.11-1%{dist}
- Updated stations

* Wed Jan 15 2014 Daniele Branchini <dbranchini@carenza.metarpa> - 0.10-2%{dist}
- Fixed error in pkg-config

* Wed Jan 15 2014 Daniele Branchini <dbranchini@arpae.it> - 0.10-1%{dist}
- Support for Lua versioning

* Thu Dec 05 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.9-1%{dist}
- Aliases in station attributes (lon, lat, rep)

* Wed Oct 02 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.8-1%{dist}
- default.lua has station attributes with bcodes as keys

* Mon Sep 30 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.7-2%{dist}
- Updated variables and stations

* Tue Aug 27 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.7-1%{dist}
- Fixed p1 for daily airquality variables (p1=86400,p2=86400)

* Fri Aug 23 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.6-1%{dist}
- Airquality stations and variables (experimental)

* Wed Jul 31 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.5-1%{dist}
- Attribute tables updated

* Fri Jul 26 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.4-1%{dist}
- fixed bug in bufr-to-meteo-vm2
- messages with missing value are skipped in meteo-vm2-to-bufr

* Tue May 14 2013 Daniele Branchini <dbranchini@arpae.it> - 0.3-2%{dist}
- fixed dep for libdballe6

* Fri May 10 2013 Daniele Branchini <dbranchini@arpae.it> - 0.3-1%{dist}
- reflecting upstream changes

* Fri Nov 30 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-3%{dist}
- %post and %postun directives

* Thu Nov 29 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-2%{dist}
- Fortran bindings

* Tue Oct 30 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-1%{dist}
- First version
