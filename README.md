# meteo-vm2

[![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=centos:7&label=centos7)](https://travis-ci.org/ARPA-SIMC/meteo-vm2)
[![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:29&label=fedora29)](https://travis-ci.org/ARPA-SIMC/meteo-vm2)
[![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:30&label=fedora30)](https://travis-ci.org/ARPA-SIMC/meteo-vm2)
[![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:rawhide&label=fedorarawhide)](https://travis-ci.org/ARPA-SIMC/meteo-vm2)

[![Build Status](https://copr.fedorainfracloud.org/coprs/simc/stable/package/meteo-vm2/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/simc/stable/package/meteo-vm2/)

## Introduction

Simple Python library for `VM2` data.

`VM2` is a simple format used at ARPA-SIMC to store observations data. It's a
`CSV` format with the following columns:

- Datetime in UTC (`YYYYmmddHHMM` or `YYYYmmddHHMMSS`)
- Station id
- Variable id
- Value 1 (double)
- Value 2 (double)
- Value 3 (string)
- Flags

Stations and variables metadata are stored in a JSON file (default path: `/var/lib/meteo-vm2/table.json`):

```
{
  "stations": {
    "12675": {
      "ident": null,
      "lon": 911081,
      "lat": 4609161,
      "rep": "fidupo",
      "B01019": "Cavargna",
      "B07030": "11000",
      "B07031": "11000"
    },
    "5591": {
      "ident": null,
      "lon": 1083529,
      "lat": 4400469,
      "rep": "idrost",
      "B01019": "Piastre",
      "B07030": "7410",
      "B07031": "7410"
    }
 },
  "variables": {
    "167": {
      "bcode": "B10004",
      "tr": 254,
      "p1": 0,
      "p2": 0,
      "lt1": 1,
      "lv1": null,
      "lt2": null,
      "lv2": null,
      "unit": "hPa"
    }
 }
}
```
