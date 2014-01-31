#!/bin/sh -x
set -e
# Generating the ./configure file
autoreconf --force --install  --verbose
./configure
