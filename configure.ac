# vim:set et ts=4:
#
# Linux Do - A new IDE for newbee 
#
# Copyright (c) 2010-2010 microcai  <microcai@fedoraproject.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# if not 1, append datestamp to the version number.
m4_define([package_name], [LinuxDo])

AC_INIT([package_name], [0.0.1], [ microcaicai@gmail.com ], [package_name])
AM_INIT_AUTOMAKE([1.10])
# AC_GNU_SOURCE

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_HEADER_STDC

AC_PREFIX_DEFAULT(/usr)

# define GETTEXT_* variables
GETTEXT_PACKAGE="$PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_SUBST(prefix)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Define to the read-only architecture-independent data directory.])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.16.1)

PKG_CHECK_MODULES(GTK2, [
    gtk+-2.0 >= 2.10
])

PKG_CHECK_MODULES(GTKSOURCEVIEW2, [
    gtksourceview-2.0
])

PKG_CHECK_MODULES(LIBVTE, [
    vte >= 0.22
])


PKG_CHECK_MODULES(ALL_NEEDED, [
	vte gtksourceview-2.0 gtk+-2.0 
])


AC_DEFINE(MICROCAI_WITHEMAIL,"microcai@fedoraproject.org",[author microcai and his email])

AC_SUBST(appicondir,["/usr/share/icons/hicolor/scalable/apps/"])
AC_DEFINE(APPICONDIR,["/usr/share/icons/hicolor/scalable/apps/"],[the dir hold icon file])
#AC_SUBST_FILE(iconsdir)


# OUTPUT files
AC_CONFIG_FILES(po/Makefile.in
src/Makefile
icons/Makefile
m4/Makefile
doc/Makefile
Makefile
)

AC_OUTPUT
