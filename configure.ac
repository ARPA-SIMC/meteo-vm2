#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([meteo-vm2], [2.0.27], [edigiacomo@arpae.it])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_FC
AM_INIT_AUTOMAKE([foreign nostdinc subdir-objects])
LT_INIT

dnl Use c++11
AX_CXX_COMPILE_STDCXX_11(noext,optional)

PKGCONFIG_REQUIRES=""
AC_SUBST(PKGCONFIG_REQUIRES)

PKG_CHECK_EXISTS([lua5.2], [have_lua=yes], [have_lua=no])
if test x$have_lua = xyes
then
        PKG_CHECK_MODULES(LUA,lua5.2,,[have_lua=no])
        PKGCONFIG_REQUIRES="lua5.2"
else
	PKG_CHECK_EXISTS([lua5.1], [have_lua=yes], [have_lua=no])
	if test x$have_lua = xyes
	then
		PKG_CHECK_MODULES([LUA],[lua5.1])
        PKGCONFIG_REQUIRES="lua5.1 >= 5.1.1"
	else
		PKG_CHECK_MODULES([LUA],[lua])
        PKGCONFIG_REQUIRES="lua >= 5.1.1"
	fi
fi

AC_PATH_PROG([DOXYGEN], [doxygen])
if test x$DOXYGEN = x
then
	have_doxygen=no
else
	have_doxygen=yes
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test x$have_doxygen = xyes])

AC_PATH_PROG([CURL], [curl])
if test x$CURL = x
then
	have_curl=no
else
	have_curl=yes
fi
AM_CONDITIONAL([HAVE_CURL], [test x$have_curl = xyes])

AC_ARG_ENABLE([bufr],
	      [AS_HELP_STRING([--enable-bufr], [build BUFR support])],
	      [],
	      [enable_bufr=yes])
if test $enable_bufr = yes
then
	PKG_CHECK_EXISTS([libdballe >= 8.17], [have_dballe=yes], [have_dballe=no])
	if test $have_dballe = yes
	then
		AC_DEFINE([HAVE_DBALLE], 1, [dballe functions are available])
		PKG_CHECK_MODULES(DBALLE,[libdballe >= 8.17])
		PKG_CHECK_EXISTS([libdballe >= 9.0], AC_DEFINE([DBALLE_VERSION_GTE_9], 1, [dballe >= 9.0]))
	else
		AC_MSG_WARN([compiling without DB-All.e: BUFR support will be disabled])
	fi
else
	have_dballe=no
fi
AM_CONDITIONAL([DBALLE], [test x"$have_dballe" = x"yes"])

AC_CHECK_PROG([have_help2man], [help2man], [yes])
AM_CONDITIONAL([HELP2MAN], [test x"$have_help2man" = x"yes"])

AC_CONFIG_FILES([Makefile
		 meteo-vm2/Makefile
		 src/Makefile
		 meteo-vm2.pc
		 meteo-vm2f.pc
		 conf/Makefile
		 doc/Doxygen
		 doc/Makefile])
AC_OUTPUT
