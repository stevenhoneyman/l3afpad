AC_INIT(L3afpad, 0.8.18.1.10, caleb@calno.com)
AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE([enable])
AC_SYS_LARGEFILE
AC_PROG_INTLTOOL(, no-xml)	# require >= 0.31 for autoreconf
AC_PROG_CC
if test "$GCC" = "yes"; then
	CFLAGS="$CFLAGS -Wall"
fi
AM_PROG_CC_C_O

PKG_CHECK_MODULES(GTK, gtk+-3.0)

AC_ARG_ENABLE(print,
	AC_HELP_STRING([--disable-print], [force to disable print feature]))
	if test "$enable_print" = "no"; then
		AC_DEFINE(ENABLE_PRINT, 0, [Define to 0 if you disable print.])
	fi

AC_ARG_ENABLE(statistics,
	AC_HELP_STRING([--disable-statistics], [force to disable statistics]))
	if test "$enable_statistics" = "no"; then
		AC_DEFINE(ENABLE_STATISTICS, 0, [Define to 0 if you disable statistics.])
	fi

AC_ARG_ENABLE(xinput2,
	AC_HELP_STRING([--disable-xinput2], [force to disable Xinput2]))
	if test "$enable_xinput2" = "no"; then
		AC_DEFINE(ENABLE_XINPUT2, 0, [Define to 0 if you disable Xinput2.])
	fi

AC_ARG_ENABLE(emacs,
	AC_HELP_STRING([--enable-emacs], [implement Emacs key theme (experimental)]))
	if test "$enable_emacs" = "yes"; then
		AC_DEFINE(ENABLE_EMACS, 1, [Define to 1 if you enable Emacs key theme.])
	fi

AC_ARG_ENABLE(gtkgrid,
	AC_HELP_STRING([--enable-gtkgrid], [Enable GtkGrid]))
	if test "$enable_gtkgrid" = "yes"; then
		AC_DEFINE(USE_GTK_GRID, 1, [Define to 1 if you enable GtkGrid.])
	fi

AC_ARG_ENABLE(search-history,
	AC_HELP_STRING([--enable-search-history], [force to enable search history]))
	if test "$enable_search_history" = "yes"; then
		AC_DEFINE(SEARCH_HISTORY, 1, [Define to 1 if you enable search history.])
	fi

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)

AC_OUTPUT([
	Makefile
	src/Makefile
	data/Makefile
	data/icons/16x16/Makefile
	data/icons/20x20/Makefile
	data/icons/22x22/Makefile
	data/icons/24x24/Makefile
	data/icons/32x32/Makefile
	data/icons/36x36/Makefile
	data/icons/40x40/Makefile
	data/icons/48x48/Makefile
	data/icons/64x64/Makefile
	data/icons/72x72/Makefile
	data/icons/96x96/Makefile
	data/icons/128x128/Makefile
	data/icons/192x192/Makefile
	data/icons/256x256/Makefile
	data/icons/Makefile
	po/Makefile.in
])
