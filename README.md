# meteo-vm2

[![Build Status](https://simc.arpae.it/moncic-ci/meteo-vm2/centos7.png)](https://simc.arpae.it/moncic-ci/meteo-vm2/)
[![Build Status](https://simc.arpae.it/moncic-ci/meteo-vm2/centos8.png)](https://simc.arpae.it/moncic-ci/meteo-vm2/)
[![Build Status](https://simc.arpae.it/moncic-ci/meteo-vm2/fedora32.png)](https://simc.arpae.it/moncic-ci/meteo-vm2/)
[![Build Status](https://simc.arpae.it/moncic-ci/meteo-vm2/fedora34.png)](https://simc.arpae.it/moncic-ci/meteo-vm2/)
[![Build Status](https://copr.fedorainfracloud.org/coprs/simc/stable/package/meteo-vm2/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/simc/stable/package/meteo-vm2/)

## Introduction

Simple C++ library for `VM2` data.

`VM2` is a simple format used at ARPAE-SIMC to store observations data. It's a
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
