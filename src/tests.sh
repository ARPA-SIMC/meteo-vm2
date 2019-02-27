#!/bin/bash
set -e

tmpdir=$(mktemp -d)
trap "rm -rf $tmpdir" EXIT
echo "199001010000,1,158,0,,," > $tmpdir/1.vm2
cat <<EOF > $tmpdir/bufr.lua
return {
    stations={
        [1]={ident=nil,lon=1207738,lat=4460016,rep='locali',B01019='Camse',B07030='-10',B07031='0',},
    },
    variables={
        [158]={bcode='B12101',tr=254,p1=0,p2=0,lt1=103,l1=2000,lt2=nil,l2=nil,unit='C',},
    },
}
EOF

$builddir/meteo-vm2-to-bufr $tmpdir/bufr.lua < $tmpdir/1.vm2 > $tmpdir/1.bufr
dbamsg dump $tmpdir/1.bufr | grep '^012101 TEMPERATURE/DRY-BULB TEMPERATURE(K): 273.15$'
$builddir/bufr-to-meteo-vm2 $tmpdir/bufr.lua < $tmpdir/1.bufr | awk -F, '{ printf("%d", $4) }' | grep '^0$'
