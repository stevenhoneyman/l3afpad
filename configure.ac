AC_INIT(L3afpad, 0.8.18.1.4, caleb@calno.com)
AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
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

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)

AC_OUTPUT([
	Makefile
	src/Makefile
	data/Makefile
	po/Makefile.in
])
