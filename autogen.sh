#!/bin/sh -x
set -e
# Getting wibble
rm -rf wibble wibble.tar.gz
wget http://people.debian.org/~enrico/wibble/embeddable/wibble.tar.gz
tar zxf wibble.tar.gz
mv wibble/wibble/* wibble/

# Generating the ./configure file
autoreconf --force --install  --verbose
