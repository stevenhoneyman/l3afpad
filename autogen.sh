#!/bin/sh
aclocal
autoheader
automake --gnu --add-missing --copy
autoconf
