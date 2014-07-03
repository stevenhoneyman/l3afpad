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

/* GTK_MESSAGE_INFO, GTK_MESSAGE_WARNING, GTK_MESSAGE_ERROR */
void run_dialog_message(GtkWidget *window,
	GtkMessageType type,
	gchar *message, ...)
{
	va_list ap;
	GtkWidget *dialog;
	gchar *str;

	va_start(ap, message);
		str = g_strdup_vprintf(message, ap);
	va_end(ap);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		type,
		GTK_BUTTONS_NONE,
		"%s", str);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_dialog_add_buttons(GTK_DIALOG(dialog),
		_("_OK"), GTK_RESPONSE_CANCEL, NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);
	g_free(str);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

GtkWidget *create_dialog_message_question(GtkWidget *window, gchar *message, ...)
{
	va_list ap;
	GtkWidget *dialog;
	gchar *str;

	va_start(ap, message);
		str = g_strdup_vprintf(message, ap);
	va_end(ap);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_NONE,
		"%s", str);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_dialog_add_buttons(GTK_DIALOG(dialog),
		_("_No"), GTK_RESPONSE_NO,
		_("_Cancel"), GTK_RESPONSE_CANCEL,
		_("_Yes"), GTK_RESPONSE_YES,
		NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);
	g_free(str);

	return dialog;
}

gint run_dialog_message_question(GtkWidget *window, gchar *message, ...)
{
	va_list ap;
	GtkWidget *dialog;
	gchar *str;
	gint res;

	va_start(ap, message);
		str = g_strdup_vprintf(message, ap);
	va_end(ap);

	dialog = create_dialog_message_question(window, str);
	g_free(str);

	res = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	return res;
}
