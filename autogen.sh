#!/bin/sh -x
set -e
# Getting wibble
[ -d wibble ] || \
{
	rm -f wibble.tar.gz
	wget http://people.debian.org/~enrico/wibble/embeddable/wibble.tar.gz
	tar zxf wibble.tar.gz
	mv wibble/wibble/* wibble/
	rm -f wibble.tar.gz
}

# Generating the ./configure file
autoreconf --force --install  --verbose
