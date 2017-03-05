bin_PROGRAMS = l3afpad

AM_CPPFLAGS = -DICONDIR=\"$(datadir)/pixmaps\" \
	-Icharset-detector/include

l3afpad_SOURCES = \
	l3afpad.h main.c \
	window.h window.c \
	menu.h menu.c \
	callback.h callback.c \
	view.h view.c \
	undo.h undo.c \
	font.h font.c \
	linenum.h linenum.c \
	indent.h indent.c \
	hlight.h hlight.c \
	selector.h selector.c \
	file.h file.c \
	encoding.h encoding.c \
	search.h search.c \
	dialog.h dialog.c \
	gtkprint.h gtkprint.c \
	dnd.h dnd.c \
	utils.h utils.c \
	emacs.h emacs.c \
	gtksourceiter.h gtksourceiter.c

l3afpad_CFLAGS  = $(GTK_CFLAGS)
l3afpad_LDADD   = $(GTK_LIBS) $(INTLLIBS) \
	charset-detector/src/libchardetect.a -lstdc++

SUBDIRS = charset-detector
