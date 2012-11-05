#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([vm2], [0.1], [edigiacomo@arpa.emr.it])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])
AM_CONFIG_HEADER(wibble/config.h)



AC_PROG_CXX
AM_INIT_AUTOMAKE([foreign nostdinc subdir-objects])
LT_INIT

AM_CONDITIONAL([WIBBLE_STANDALONE], [false])

PKG_CHECK_EXISTS([lua5.1], [have_lua=yes], [have_lua=no])
if test x$have_lua = xyes
then
	PKG_CHECK_MODULES([LUA],[lua5.1])
else
	PKG_CHECK_MODULES([LUA],[lua])
fi

AC_PATH_PROG([DOXYGEN], [doxygen])
if test x$DOXYGEN = x
then
	have_doxygen=no
else
	have_doxygen=yes
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test x$have_doxygen = xyes])

AC_CONFIG_FILES([Makefile
		 wibble/Makefile
                 vm2/Makefile
		 vm2.pc
		 doc/Doxygen
		 doc/Makefile])
AC_OUTPUT
