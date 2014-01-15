#!/bin/sh
srcdir=$( ( cd $(dirname $0)/../test/data ; pwd ) )

export METEO_VM2_SOURCE=${srcdir}/source-1.lua
export METEO_VM2_INPUT_TEST=${srcdir}/1.vm2

exec "$@"
