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
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define STDIN_DELAY_MICROSECONDS 100000
#define GEDIT_STDIN_BUFSIZE 1024

// imported from gedit
#ifdef G_OS_UNIX
gchar *gedit_utils_get_stdin (void)
{
	GString * file_contents;
	gchar *tmp_buf = NULL;
	guint buffer_length;
//	GnomeVFSResult	res;
	fd_set rfds;
	struct timeval tv;

	FD_ZERO (&rfds);
	FD_SET (0, &rfds);

	// wait for 1/4 of a second
	tv.tv_sec = 0;
	tv.tv_usec = STDIN_DELAY_MICROSECONDS;

	if (select (1, &rfds, NULL, NULL, &tv) != 1)
		return NULL;

	tmp_buf = g_new0 (gchar, GEDIT_STDIN_BUFSIZE + 1);
	g_return_val_if_fail (tmp_buf != NULL, FALSE);

	file_contents = g_string_new (NULL);

	while (feof (stdin) == 0)
	{
		buffer_length = fread (tmp_buf, 1, GEDIT_STDIN_BUFSIZE, stdin);
		tmp_buf [buffer_length] = '\0';
		g_string_append (file_contents, tmp_buf);

		if (ferror (stdin) != 0)
		{
//			res = gnome_vfs_result_from_errno ();

			g_free (tmp_buf);
			g_string_free (file_contents, TRUE);
			return NULL;
		}
	}

	fclose (stdin);

	return g_string_free (file_contents, FALSE);
}
#endif

GtkWidget *create_button_with_stock_image(const gchar *text, const gchar *stock_id)
{
	GtkWidget *button;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *label;
	GtkWidget *align;

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);

	image = gtk_image_new_from_icon_name(stock_id, GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(text);
	gtk_box_pack_end(GTK_BOX(hbox), label, FALSE, FALSE, 0);

	align = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
	gtk_container_add(GTK_CONTAINER(align), hbox);

	button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(button), align);
	gtk_widget_show_all(button);

	return button;
}

#if SEARCH_HISTORY
void update_combo_data (GtkWidget *entry, GList **history)
{
	const gchar *text;
	GList *node;

	text = gtk_entry_get_text (GTK_ENTRY (entry));
	if (*text == '\0')
		return;
	for (node = *history; node != NULL; node = g_list_next (node))
	{
		if (g_str_equal ((gchar *)node->data, text))
		{
			g_free (node->data);
			*history = g_list_delete_link (*history, node);
			break;
		}
	}
	*history = g_list_prepend (*history, g_strdup (text));
}

GtkWidget *create_combo_with_history (GList **history)
{
	GtkWidget *combo;
	GList *node;

	combo = gtk_combo_box_text_new_with_entry ();
	//work around gtk silliness -
	//'appears-as-list' is a read-only style property instead of a widget property
	gtk_rc_parse_string (
		"style \"list-style-style\" { GtkComboBox::appears-as-list = 1 } "
		"widget \"*.list-style\" style \"list-style-style\"");
	gtk_widget_set_name (combo, "list-style");

	for (node = *history; node != NULL; node = g_list_next (node))
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), node->data);

	gtk_widget_show (combo);
	return combo;
}

#if 0 /* if we want to cleanup before exit ... */
void clear_combo_history (GList *history)
{
	GList *node;
	if (history != NULL)
	{
		for (node = history; node != NULL; node = g_list_next (node))
		{
			if (node->data != NULL)
				g_free (node->data);
		}
		g_list_free (history);
		history = NULL;
	}
}
#endif
#endif
