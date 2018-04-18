# meteo-vm2

## Build status

| Environment | Status |
| ----------- | ------ |
| CentOS 7    | [![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=centos:7&label=centos7)](https://travis-ci.org/ARPA-SIMC/meteo-vm2) |
| Fedora 26   | [![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:26&label=fedora26)](https://travis-ci.org/ARPA-SIMC/meteo-vm2) |
| Fedora 27   | [![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:27&label=fedora27)](https://travis-ci.org/ARPA-SIMC/meteo-vm2) |
| Fedora 28   | [![Build Status](https://badges.herokuapp.com/travis/ARPA-SIMC/meteo-vm2?branch=master&env=DOCKER_IMAGE=fedora:28&label=fedora28)](https://travis-ci.org/ARPA-SIMC/meteo-vm2) |


## Introduction

Simple C++ library for `VM2` data.

`VM2` is a simple format used at ARPA-SIMC to store observations data. It's a
`CSV` format with the following columns:

- Datetime in UTC (`YYYYmmddHHMM` or `YYYYmmddHHMMSS`)
- Station id
- Variable id
- Value 1 (double)
- Value 2 (double)
- Value 3 (string)
- Flags

Stations and variables metadata are stored in a file as a Lua table:

```
return {
    stations={
        [1]={name="Camse"},
        ...
    },
    variables={
        ...
    },
}
```
