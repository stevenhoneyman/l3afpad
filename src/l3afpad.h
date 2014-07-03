/*
 *  L3afpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _L3AFPAD_H
#define _L3AFPAD_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>

#ifndef ENABLE_EMACS
#define ENABLE_EMACS 0
#endif

#ifndef ENABLE_PRINT
#define ENABLE_PRINT 1
#endif

#ifndef SEARCH_HISTORY
#define SEARCH_HISTORY 0
#endif

#ifndef ENABLE_STATISTICS
#define ENABLE_STATISTICS 1
#endif

#ifndef ENABLE_XINPUT2
#define ENABLE_XINPUT2 1
#endif

#include "window.h"
#include "menu.h"
#include "callback.h"
#include "view.h"
#include "undo.h"
#include "font.h"
#include "linenum.h"
#include "indent.h"
#include "hlight.h"
#include "selector.h"
#include "file.h"
#include "encoding.h"
#include "search.h"
#include "dialog.h"
#include "dnd.h"
#include "utils.h"
#include "emacs.h"
#include "gtkprint.h"
#include <gdk/gdkkeysyms-compat.h>

typedef struct {
	FileInfo *fi;
	MainWin *mw;
} PublicData;

#ifndef _L3AFPAD_MAIN
extern PublicData *pub;
#endif

void save_config_file(void);

#endif /* _L3AFPAD_H */
