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
/*
static void cb_scroll_event(GtkAdjustment *adj, GtkWidget *view)
{
	gtk_text_view_place_cursor_onscreen(GTK_TEXT_VIEW(view));
}
*/
MainWin *create_main_window(void)
{
	GtkWidget *vbox;
	GtkWidget *sw;

	MainWin *mw = g_malloc(sizeof(MainWin));

	mw->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_name(mw->window, PACKAGE_NAME);

	gtk_window_set_icon_from_file(GTK_WINDOW(mw->window), ICONDIR"/l3afpad.png", NULL);
	gtk_window_set_default_icon_name(PACKAGE);

	g_signal_connect(G_OBJECT(mw->window), "delete-event",
		G_CALLBACK(on_file_quit), NULL);
	g_signal_connect_after(G_OBJECT(mw->window), "delete-event",
		G_CALLBACK(gtk_widget_hide_on_delete), NULL);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(mw->window), vbox);

	mw->menubar = create_menu_bar(mw->window);
	gtk_box_pack_start(GTK_BOX(vbox), gtk_ui_manager_get_widget(mw->menubar, "/M"), FALSE, FALSE, 0);

	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_hexpand (sw, TRUE);
	gtk_widget_set_vexpand (sw, TRUE);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
		GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);

	mw->view = create_text_view();
	gtk_container_add(GTK_CONTAINER(sw), mw->view);
	mw->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mw->view));

	return mw;
}

void set_main_window_title(void)
{
	gchar *title;

	title = get_file_basename(pub->fi->filename, TRUE);
	gtk_window_set_title(GTK_WINDOW(pub->mw->window), title);
	g_free(title);
}
