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

#include "l3afpad.h"

void set_text_font_by_name(GtkWidget *widget, gchar *fontname)
{
	PangoFontDescription *font_desc;

	font_desc = pango_font_description_from_string(fontname);
	gtk_widget_override_font(widget, font_desc);
	pango_font_description_free(font_desc);
}

static gchar *get_font_name_by_selector(GtkWidget *window, gchar *current_fontname)
{
	GtkWidget *dialog;
	gchar *fontname;

	dialog = gtk_font_chooser_dialog_new(_("Font"),NULL);
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	gtk_font_chooser_set_font(GTK_FONT_CHOOSER(dialog), current_fontname);
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
		fontname = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog));
	else
		fontname = NULL;
	gtk_widget_destroy(dialog);

	return fontname;
}

void change_text_font_by_selector(GtkWidget *widget)
{
	gchar *current_fontname, *fontname;

	current_fontname = pango_font_description_to_string(gtk_style_context_get_font(gtk_widget_get_style_context(widget), 0));
	fontname = get_font_name_by_selector(
		gtk_widget_get_toplevel(widget), current_fontname);
	if (fontname) {
		set_text_font_by_name(widget, fontname);
		indent_refresh_tab_width(widget);
	}

	g_free(fontname);
	g_free(current_fontname);
}
