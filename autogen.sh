#!/bin/sh
aclocal
autoheader
automake --gnu --add-missing --copy
autoconf
[ -d autom4te.cache ] && rm -fr autom4te.cache
