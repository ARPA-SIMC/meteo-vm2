%global releaseno 1
# Note: define _srcarchivename in Travis build only.
%{!?srcarchivename: %global srcarchivename %{name}-%{version}-%{releaseno}}

Name:           meteo-vm2
Version:        2.0.31
Release:        %{releaseno}%{?dist}
Summary:        C++ library for VM2 data 

License:        GPLv2+
URL:            https://github.com/arpa-simc/%{name}
Source0:        https://github.com/arpa-simc/%{name}/archive/v%{version}-%{releaseno}.tar.gz#/%{srcarchivename}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  libtool
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(libdballe) >= 8.17
BuildRequires:  dballe
BuildRequires:  pkgconfig(lua) >= 5.1.1
BuildRequires:  help2man

%description
VM2 decoding/encoding library

%prep
%setup -q -n %{srcarchivename}

%build
sh autogen.sh
%configure --disable-static
make %{?_smp_mflags}

%check
make check || { find . -name "test-suite.log" -print0 | xargs -0 cat ; false; }

%install
rm -rf $RPM_BUILD_ROOT
%make_install

%files
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so.*

%package devel
Summary:        C++ library for VM2 data - development files
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}

%description devel
VM2 decoding/encoding library - development files

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*
%exclude %{_libdir}/lib%{name}.la
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/meteo-vm2.pc

%package doc
Summary:        C++ library for VM2 data - documentation

%description doc
VM2 decoding/encoding library - documentation

%files doc
%doc %{_docdir}/%{name}
%defattr(-,root,root,-)

%package utils
Summary:        C++ library for VM2 data - utilities
Requires:       %{name} = %{?epoch:%epoch:}%{version}-%{release}
Requires:       pkgconfig(libdballe) >= 8.17
Requires:       meteo-vm2-data-simc

%description utils
Collection of utilities for VM2 files

%files utils
%defattr(-,root,root,-)
%{_bindir}/meteo-vm2-to-bufr
%{_bindir}/bufr-to-meteo-vm2
%{_bindir}/meteo-vm2-update-source
%{_mandir}/man1/bufr-to-meteo-vm2.1.gz
%{_mandir}/man1/meteo-vm2-to-bufr.1.gz

%package data-simc
Summary:        C++ library for VM2 data - SIMC config files

%description data-simc
VM2 decoding/encoding library - SIMC config files

%files data-simc
%defattr(-,root,root,-)
%dir %{_sharedstatedir}/%{name}
%{_sharedstatedir}/%{name}/source/default.lua*
%{_sharedstatedir}/%{name}/source/bufr.lua*

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%changelog
* Fri Jan  9 2025 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.31-1
- Cambio coordinate stazione 53136

* Mon Dec 23 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.30-1
- Cambio nomi stazioni 53135,53136,53137

* Mon Nov 25 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.29-1
- Aggiunta stazione Riccardina 53253

* Tue Nov 12 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.28-1
- Nuove stazioni da 53249 a 53252

* Tue Jun 11 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.27-1
- Nuove stazioni 53247 e 53248

* Mon Jun 10 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.26-1
- Nuova stazione 53246

* Wed May 29 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.25-2
- Ripacchettizzato per problemi di tag

* Wed May 29 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.25-1
- Nuova stazione Monte Colombo

* Thu May  9 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.24-1
- Nuove stazioni Chiavica Rossa e Mezzani

* Tue May  7 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.23-1
- Nuova stazione Traversa Lentino

* Tue Apr  9 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.22-1
- Correzioni coordinate

* Thu Apr  4 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.21-1
- Correzione coordinate

* Tue Mar 26 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.20-1
- Correzione coordinate Verago (53067)

* Tue Mar 26 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.19-1
- Nuove stazioni Polesera Opera (53236) e Polesera Cassa (53237)
- Eliminazione stazione Ravone Torretta (14612)

* Mon Mar 18 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.18-1
- "Bagnatura fogliare cumulata su 15 minuti" (1776)

* Mon Mar 11 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.17-1
- Nuova variabile "Direzione del massimo valore di raffica lunga a 10 m dal
  suolo nell'ora" (1775)

* Tue Mar  5 2024 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.16-1
- Nuovi idrometri CAE

* Mon Nov 20 2023 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.15-1
- Nuove stazioni idrtl9

* Thu Nov  9 2023 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.14-1
- Nuove stazioni profe

* Thu Oct 12 2023 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.13-1
- Nuove stazioni idrost e idrtl9

* Thu Jun  1 2023 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.12-1
- Rinominata stazione 5896 da "Frassinoro" a "Frassinoro 0"
- Creata stazione 53161 "Frassinoro" (rete idrost)

* Mon May  8 2023 Emanuele Di Giacomo <edigiacomo@arpae.it> - 2.0.11-1
- Nuova stazione "S. Antonio Arpa 0" (53160)

* Fri Jan 27 2023 Marcello Nuccio <mnuccio@arpae.it> - 2.0.0-1
- Breaking change: meteo-vm2-to-bufr exit status=2 in caso di errori
- Breaking change: meteo-vm2-to-bufr mette flag A=0 quando A=2 perché manca valore2

* Mon Nov 15 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.13-1
- Sostituiti "San*" con "S." nei nomi delle stazioni (Valentina)

* Fri Oct  1 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.9-1
- Nuove variabili per San Pietro Capofiume

* Tue Aug 17 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.8-1
- Canonica Valle da simnpr a simnbo

* Tue Jul 20 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.7-1
- Correzione San Pancrazio

* Wed Jul 14 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.6-1
- Correzione livello variabile 1754

* Tue Jul  6 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.5-1
- Eliminate stazioni doppie: Bondanello e Rubiera
- Aggiornamento pindicator della variabile 1754
- Correzione nome stazioni 3008 e 4064

* Wed Jun 16 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.4-1
- Fix versione pacchetto

* Tue Jun 15 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.3-1
- Aggiornamento variabili

* Mon Jun 14 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.2-1
- Package meteo-vm2-data-simc

* Tue Jun  8 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.1-2
- Package meteo-vm2-data-simc

* Tue Jun  8 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.2.1-1
- Aggiornamento anagrafica

* Mon May 17 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.0-1
- Fixed compatibility with dballe 8 and 9

* Mon May 17 2021 Daniele Branchini <dbranchini@arpae.it> - 1.1.10-2
- Bogus release for dballe 9.0 update

* Thu May 13 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.10-1
- Aggiornamento anagrafica

* Wed May 12 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.9-1
- Aggiornamento anagrafica

* Thu Apr 22 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.8-1
- Variabile 1762 - Livello del mare media sugli ultimi 10 minuti

* Tue Apr 13 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.7-1
- Aggiornamento anagrafica

* Tue Apr 13 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.6-1
- Aggiornamento anagrafica

* Fri Apr 9 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.5-1
- Aggiornamento anagrafica

* Wed Mar 24 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.4-1
- Aggiornamento anagrafica

* Wed Mar 24 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.3-1
- Aggiornamento anagrafica

* Thu Mar 18 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.2-1
- Fix coordinate Monzugno

* Thu Mar 18 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.1.1-1
- Aggiornamenti vari da anagrafica meteozen

* Mon Jan 25 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.1.0-2
- Explicit requires libdballe >= 8.17

* Mon Jan 25 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.1.0-1
- Clamp e B33192=0 per valori fuori range (#241)

* Thu Jan  7 2021 Marcello Nuccio <mnuccio@arpae.it> - 1.0.31-1
- Nuove LOCALI e variabili

* Tue Nov 10 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.30-1
- Aggiornamento anagrafica stazioni

* Wed Sep 23 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.29-1
- Aggiornamento anagrafica stazioni

* Thu Sep 17 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.28-1
- Correzioni nome stazioni rmap

* Tue Sep 15 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.27-1
- Nuove stazioni FIDUVE

* Tue Sep 8 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.26-1
- Stazione Verghereto FC

* Tue Aug 11 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.25-1
- Fix changelog

* Tue Aug 11 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.24-1
- Idrometro S. Ilario d'Enza

* Mon Jun 29 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.23-1
- Stazioni Romagna Acque

* Tue Jun 16 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.22-1
- Stazioni Cantina Valtidone

* Mon Jun 15 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.21-1
- Nuove variabili vento a 5 m dal suolo

* Mon Jun 15 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.20-1
- Correzione variabili raffica

* Fri Jun 5 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.19-1
- Rimossa Stazione Casalecchio Canonica valle duplicata

* Thu Jun 4 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.18-1
- Stazione idrometro Casalecchio Canonica valle

* Mon May 25 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.17-1
- Stazioni FIDUVE parte I

* Fri May 22 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.16-1
- New station Cedogno IDRTL9

* Tue Apr 7 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.15-1
- New station Minerbio PROFE

* Tue Apr 7 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.14-1
- New station 14437

* Thu Apr 2 2020 Marcello Nuccio <mnuccio@arpae.it> - 1.0.13-1
- New stations fidusl (Slovenia)

* Fri Mar 27 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.12-1
- New variable 787

* Tue Mar 10 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.11-1
- New stations and variables

* Thu Feb 13 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.10-1
- New stations and variables

* Wed Jan 22 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.9-1
- Updated stations

* Wed Jan  8 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.8-1
- Fixed Parma S. Siro coordinates

* Tue Jan  7 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.7-1
- New stations (simnpr)
- Removed station (fiduli)
- New variables (snow)

* Tue Dec 10 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.6-1
- New stations (idrost)
- New stations (fiduli)

* Thu Nov 14 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.5-1
- New stations (fidutn)

* Mon Nov  4 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.4-1
- Renamed station 2304
- New station Riccione Urbana
- New variables

* Mon Jul  8 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.3-2
- Fixed files section

* Thu Jul  4 2019 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.3-1
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

* Thu Nov 22 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.67-1
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

* Thu Jun 14 2018 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.60-1
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

* Wed Oct 14 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.34-1
- Pollen unit

* Tue Oct 13 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.33-1
- Fixed error message
- Parser::regexp_str public again

* Mon Oct 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.32-1
- Fixed bug in gcc-4.8 regex parser

* Mon Oct 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.31-1
- Source updated

* Fri Oct 09 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.30-1
- gcc 4.8.3 support

* Thu Oct 01 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.29-1
- Removed wibble
- C++11

* Mon Aug 03 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.27-1
- Back to old reports

* Fri Jun 19 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.26-1
- dballe-7.1-4828

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25-3
- Fix default.lua, again

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25-2
- Fix default.lua

* Fri Jun 05 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.25
- default.lua and bufr.lua source files

* Thu Jun 04 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.24
- New stations and variables
- dballe 7.1-4758 support

* Tue May 26 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.23
- Disabled Python bindings

* Tue May 26 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.22
- dballe-7.1-4749 support

* Tue May 12 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.21
- Fixed l1, l2 and missing values as nil

* Mon May 11 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.20
- New stations

* Fri Apr 17 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.19
- New stations

* Thu Feb 19 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.18.1-1
- Error message

* Wed Feb 18 2015 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.18-1
- Fixed parser

* Tue Sep 16 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.17-1
- bufr-to-meteo-vm2: optional ident

* Tue Sep 16 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.16-1
- Updated stations
- Stations data for BUFR conversion

* Wed Sep 03 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.15-1
- New stations 1034, 1035, 1036

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.14-1
- Fixed stations

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.13-2
- Fixed lua dependency

* Wed Jun 25 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.13-1
- Updated stations

* Mon Jun 23 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.12-1
- Stations 4217,4222,4240,4247,4344 to idrmec-pub
- Station 13023 to rer

* Thu Jun 19 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.11-2
- meteo-vm2-to-bufr: fixed error message

* Thu Jun 19 2014 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.11-1
- Updated stations

* Wed Jan 15 2014 Daniele Branchini <dbranchini@carenza.metarpa> - 0.10-2
- Fixed error in pkg-config

* Wed Jan 15 2014 Daniele Branchini <dbranchini@arpae.it> - 0.10-1
- Support for Lua versioning

* Thu Dec 05 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.9-1
- Aliases in station attributes (lon, lat, rep)

* Wed Oct 02 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.8-1
- default.lua has station attributes with bcodes as keys

* Mon Sep 30 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.7-2
- Updated variables and stations

* Tue Aug 27 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.7-1
- Fixed p1 for daily airquality variables (p1=86400,p2=86400)

* Fri Aug 23 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.6-1
- Airquality stations and variables (experimental)

* Wed Jul 31 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.5-1
- Attribute tables updated

* Fri Jul 26 2013 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.4-1
- fixed bug in bufr-to-meteo-vm2
- messages with missing value are skipped in meteo-vm2-to-bufr

* Tue May 14 2013 Daniele Branchini <dbranchini@arpae.it> - 0.3-2
- fixed dep for libdballe6

* Fri May 10 2013 Daniele Branchini <dbranchini@arpae.it> - 0.3-1
- reflecting upstream changes

* Fri Nov 30 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-3
- %%post and %%postun directives

* Thu Nov 29 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-2
- Fortran bindings

* Tue Oct 30 2012 Emanuele Di Giacomo <edigiacomo@arpae.it> - 0.1-1
- First version
