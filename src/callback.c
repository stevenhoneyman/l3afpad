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
#include <stdlib.h>
#include <string.h>

static void set_selection_bound(GtkTextBuffer *buffer, gint start, gint end)
{
	GtkTextIter start_iter, end_iter;

	gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, start);
	if (end < 0)
		gtk_text_buffer_get_end_iter(buffer, &end_iter);
	else
		gtk_text_buffer_get_iter_at_offset(buffer, &end_iter, end);
	gtk_text_buffer_place_cursor(buffer, &end_iter);
	gtk_text_buffer_move_mark_by_name(buffer, "selection_bound", &start_iter);
}

void on_file_new(void)
{
	gchar *comline;
	gchar *option;

	save_config_file();
	option = pub->fi->charset_flag ?
		g_strdup_printf("%s%s", " --codeset=", pub->fi->charset) : "";
	comline = g_strdup_printf("%s%s", PACKAGE, option);
	if (pub->fi->charset_flag)
		g_free(option);
	g_spawn_command_line_async(comline, NULL);
	g_free(comline);
}

void on_file_open(void)
#ifdef ENABLE_CSDI
{ // too slow...
	FileInfo *fi;
	gchar *comline;
	gchar *option = NULL;

	fi = get_fileinfo_from_selector(pub->fi, OPEN);
	if (fi) {
		save_config_file();
		option = g_strdup_printf("--codeset=%s ", fi->charset);
		comline = g_strdup_printf("%s %s%s", PACKAGE,
			fi->charset ? option : "",
			fi->filename);
		g_spawn_command_line_async(comline, NULL);
		g_free(option);
		g_free(comline);
		g_free(fi);
	}
}
#else
{
	FileInfo *fi;

	if (check_text_modification())
		return;
	fi = get_fileinfo_from_selector(pub->fi, OPEN);
	if (fi) {
		if (file_open_real(pub->mw->view, fi))
			g_free(fi);
		else {
			g_free(pub->fi);
			pub->fi = fi;
			undo_clear_all(pub->mw->buffer);
//			set_main_window_title();
			force_call_cb_modified_changed(pub->mw->view);
//			undo_init(sd->mainwin->textview, sd->mainwin->textbuffer, sd->mainwin->menubar);
		}
	}
}
#endif

gint on_file_save(void)
{
	if (pub->fi->filename == NULL)
		return on_file_save_as();
	if (check_file_writable(pub->fi->filename) == FALSE)
		return on_file_save_as();
	if (file_save_real(pub->mw->view, pub->fi))
		return -1;
//	set_main_window_title();
	force_call_cb_modified_changed(pub->mw->view);
//	undo_reset_step_modif();
	return 0;
}

gint on_file_save_as(void)
{
	FileInfo *fi;

	fi = get_fileinfo_from_selector(pub->fi, SAVE);
	if (fi == NULL)
		return -1;
	if (file_save_real(pub->mw->view, fi)) {
		g_free(fi);
		return -1;
	}
	g_free(pub->fi);
	pub->fi = fi;
	undo_clear_all(pub->mw->buffer);
//	set_main_window_title();
	force_call_cb_modified_changed(pub->mw->view);
//	undo_init(sd->mainwin->textview, sd->mainwin->textbuffer, sd->mainwin->menubar);
	return 0;
}

#if ENABLE_STATISTICS
void on_file_stats(void)
{
	gchar * stats = file_stats( pub->mw->view, pub->fi );

	GtkMessageDialog * msg = (GtkMessageDialog *)
		gtk_message_dialog_new_with_markup( NULL,
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			_("<b>Statistics</b>")
	);

	gtk_message_dialog_format_secondary_markup(
		msg,
		"<i>%s</i>",
		stats
	);

	gtk_window_set_title( GTK_WINDOW(msg),
		pub->fi->filename );
	gtk_window_set_transient_for(GTK_WINDOW(msg),
		GTK_WINDOW(pub->mw->window));
	gtk_dialog_run( (GtkDialog *) msg );
	gtk_widget_destroy( (GtkWidget *) msg );

	g_free( stats );
}
#endif

#if ENABLE_PRINT
void on_file_print_preview(void)
{
	create_gtkprint_preview_session(GTK_TEXT_VIEW(pub->mw->view),
		get_file_basename(pub->fi->filename, FALSE));
}

void on_file_print(void)
{
	create_gtkprint_session(GTK_TEXT_VIEW(pub->mw->view),
		get_file_basename(pub->fi->filename, FALSE));
}
#endif
void on_file_close(void)
{
	if (!check_text_modification()) {
		force_block_cb_modified_changed(pub->mw->view);
//		undo_block_signal(textbuffer);
		gtk_text_buffer_set_text(pub->mw->buffer, "", 0);
		gtk_text_buffer_set_modified(pub->mw->buffer, FALSE);
		if (pub->fi->filename)
			g_free(pub->fi->filename);
		pub->fi->filename = NULL;
		if (pub->fi->charset)
			g_free(pub->fi->charset);
		pub->fi->charset = NULL;
		pub->fi->charset_flag = FALSE;
		pub->fi->lineend = LF;
		undo_clear_all(pub->mw->buffer);
//		set_main_window_title();
		force_call_cb_modified_changed(pub->mw->view);
		force_unblock_cb_modified_changed(pub->mw->view);
//		undo_unblock_signal(textbuffer);
//		undo_init(sd->mainwin->textview, textbuffer, sd->mainwin->menubar);
	}
}

void on_file_quit(void)
{
	if (!check_text_modification()) {
		save_config_file();
		gtk_main_quit();
	}
}

void on_edit_undo(void)
{
	undo_undo(pub->mw->buffer);
}

void on_edit_redo(void)
{
	undo_redo(pub->mw->buffer);
}

void on_edit_cut(void)
{
	g_signal_emit_by_name(G_OBJECT(pub->mw->view), "cut-clipboard");
}

void on_edit_copy(void)
{
	g_signal_emit_by_name(G_OBJECT(pub->mw->view), "copy-clipboard");
}

void on_edit_paste(void)
{
	g_signal_emit_by_name(G_OBJECT(pub->mw->view), "paste-clipboard");
//	TODO: Use modify signal!!
/*	gtk_text_view_scroll_mark_onscreen(
		GTK_TEXT_VIEW(pub->mw->view),
		gtk_text_buffer_get_insert(pub->mw->buffer));
*/}

void on_edit_delete(void)
{
	gtk_text_buffer_delete_selection(pub->mw->buffer, TRUE, TRUE);
}

void on_edit_select_all(void)
{
	set_selection_bound(pub->mw->buffer, 0, -1);
//	g_signal_emit_by_name(G_OBJECT(pub->mw->view), "select-all");
}

static void activate_quick_find(void)
{
	static gboolean flag = FALSE;

	if (!flag) {
		gtk_widget_set_sensitive(
			gtk_ui_manager_get_widget(pub->mw->menubar, "/M/Search/FindNext"),
			TRUE);
		gtk_widget_set_sensitive(
			gtk_ui_manager_get_widget(pub->mw->menubar, "/M/Search/FindPrevious"),
			TRUE);
		flag = TRUE;
	}
}

void on_search_find(void)
{
	if (run_dialog_search(pub->mw->view, 0) == GTK_RESPONSE_OK)
		activate_quick_find();
}

void on_search_find_next(void)
{
	document_search_real(pub->mw->view, 1);
}

void on_search_find_previous(void)
{
	document_search_real(pub->mw->view, -1);
}

void on_search_replace(void)
{
	if (run_dialog_search(pub->mw->view, 1) == GTK_RESPONSE_OK)
		activate_quick_find();
}

void on_search_jump_to(void)
{
	run_dialog_jump_to(pub->mw->view);
}

void on_option_font(void)
{
	change_text_font_by_selector(pub->mw->view);
}

void on_option_word_wrap(void)
{
	gboolean state;

	state = gtk_toggle_action_get_active(
		GTK_TOGGLE_ACTION(gtk_ui_manager_get_action(pub->mw->menubar, "/M/Options/WordWrap")));
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pub->mw->view),
		state ? GTK_WRAP_WORD : GTK_WRAP_NONE);
}

void on_option_line_numbers(void)
{
	gboolean state;

	state = gtk_toggle_action_get_active(
		GTK_TOGGLE_ACTION(gtk_ui_manager_get_action(pub->mw->menubar, "/M/Options/LineNumbers")));
	show_line_numbers(pub->mw->view, state);
}

void on_option_always_on_top(void)
{
	static gboolean flag = FALSE;

	flag =! flag;
	gtk_window_set_keep_above(GTK_WINDOW(pub->mw->window), flag);
}

void on_option_auto_indent(void)
{
	gboolean state;

	state = gtk_toggle_action_get_active(
		GTK_TOGGLE_ACTION(gtk_ui_manager_get_action(pub->mw->menubar, "/M/Options/AutoIndent")));
	indent_set_state(state);
}

void on_help_about(void)
{
	const gchar *copyright = "Copyright \xc2\xa9 2004-2010 Tarot Osuji\nCopyright \xc2\xa9 2011 Wen-Yen Chuang\nCopyright \xc2\xa9 2011 Jack Gandy\nCopyright \xc2\xa9 2012 Yoo, Taik-Yon\nCopyright \xc2\xa9 2014 Steven Honeyman";
	const gchar *comments = _("GTK+ based simple text editor");
	const gchar *authors[] = {
		"Tarot Osuji <tarot@sdf.lonestar.org>",
		"Wen-Yen Chuang <caleb@calno.com>",
		"Yoo, Taik-Yon <jaagar@gmail.com>",
		"Steven Honeyman <stevenhoneyman@gmail.com>",
		NULL
	};
	const gchar *translator_credits = _("translator-credits");

	translator_credits = strcmp(translator_credits, "translator-credits")
		? translator_credits : NULL;

	const gchar *artists[] = {
		"Lapo Calamandrei <calamandrei@gmail.com>",
		"Jack Gandy <scionicspectre@gmail.com>",
		NULL
	};
	gtk_show_about_dialog(GTK_WINDOW(pub->mw->window),
		"program-name", _("Lɜafpad"),
		"version", PACKAGE_VERSION,
		"copyright", copyright,
		"comments", comments,
		"authors", authors,
		"artists", artists,
		"translator-credits", translator_credits,
		"logo-icon-name", PACKAGE,
		NULL);
}
