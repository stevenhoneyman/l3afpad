AC_INIT(L3afpad, 0.8.18.1.1, caleb@calno.com)
AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)

AC_PROG_INTLTOOL(, no-xml)	# require >= 0.31 for autoreconf
AC_PROG_CC
if test "$GCC" = "yes"; then
	CFLAGS="$CFLAGS -Wall -Wextra"
fi
AM_PROG_CC_C_O

PKG_CHECK_MODULES(GTK, gtk+-3.0)

AC_ARG_ENABLE(print,
	AC_HELP_STRING([--disable-print], [force to disable print feature]))
if test "$enable_print" != "no"; then
	PKG_CHECK_EXISTS(gtk+-3.0 >= 3.0.10,
		AC_DEFINE(ENABLE_PRINT, 1, [Define if print feature is enabled.]))
fi

AC_ARG_ENABLE(emacs,
	AC_HELP_STRING([--enable-emacs], [implement Emacs key theme (experimental)]),
	AC_DEFINE(ENABLE_EMACS, 1, [Define if Emacs key theme is enabled.])
)

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)

AC_OUTPUT([
	Makefile
	src/Makefile
	data/Makefile
	po/Makefile.in
])
