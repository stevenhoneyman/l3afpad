/*
 *  L3afpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *  Copyright (C)      2011 Wen-Yen Chuang <caleb AT calno DOT com>
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
#include <string.h>

#define DEFAULT_ITEM_NUM 2

static gint mode;
static gchar *other_codeset_title = NULL;
static gchar *lineend_str[] = {
	"LF",
	"CR+LF",
	"CR"
};

static void cb_select_lineend(GtkComboBox *option_menu, FileInfo *selected_fi)
{
	switch (gtk_combo_box_get_active(option_menu)) {
	case 1:
		selected_fi->lineend = CR+LF;
		break;
	case 2:
		selected_fi->lineend = CR;
		break;
	default:
		selected_fi->lineend = LF;
	}
}

static GtkWidget *create_lineend_menu(FileInfo *selected_fi)
{
	GtkListStore *store = gtk_list_store_new (1, G_TYPE_STRING);
	GtkTreeIter iter;
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkWidget *option_menu = gtk_combo_box_new_with_model (GTK_TREE_MODEL (store));
	gint i;

	for (i = 0; i <= 2; i++) {
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, lineend_str[i], -1);
	}
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (option_menu), renderer, FALSE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (option_menu), renderer, "text", 0, NULL);

	g_signal_connect(G_OBJECT(option_menu), "changed",
		G_CALLBACK(cb_select_lineend), selected_fi);

	i = 0;
	switch (selected_fi->lineend) {
	case CR+LF:
		i = 1;
		break;
	case CR:
		i = 2;
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(option_menu), i);

	return option_menu;
}

typedef struct {
	const gchar *charset[ENCODING_MAX_ITEM_NUM + DEFAULT_ITEM_NUM];
	const gchar *str[ENCODING_MAX_ITEM_NUM + DEFAULT_ITEM_NUM];
	guint num;
} CharsetTable;

static CharsetTable *get_charset_table(void)
{
	static CharsetTable *ctable = NULL;
	EncArray *encarray;
	gint i;

	if (!ctable) {
		ctable = g_malloc(sizeof(CharsetTable));
		ctable->num = 0;
		ctable->charset[ctable->num] = get_default_charset();
		ctable->str[ctable->num] = g_strdup_printf(_("Current Locale (%s)"), get_default_charset());
		ctable->num++;
		ctable->charset[ctable->num] = "UTF-8";
		ctable->str[ctable->num] = ctable->charset[ctable->num];
		ctable->num++;
		encarray = get_encoding_items(get_encoding_code());
		for (i = 0; i < ENCODING_MAX_ITEM_NUM; i++)
			if (encarray->item[i]) {
				ctable->charset[ctable->num] = encarray->item[i];
				ctable->str[ctable->num] = encarray->item[i];
				ctable->num++;
			}
	}

	return ctable;
}

static void toggle_sensitivity(GtkWidget *entry)
{
	gtk_dialog_set_response_sensitive(
		GTK_DIALOG(gtk_widget_get_toplevel(entry)), GTK_RESPONSE_OK,
		strlen(gtk_entry_get_text(GTK_ENTRY(entry))) ? TRUE : FALSE);
}

static GtkWidget *menu_item_manual_charset;
static GtkWidget *init_menu_item_manual_charset(gchar *manual_charset)
{
	static GtkLabel *label;
	gchar *str;

	if (other_codeset_title == NULL)
		other_codeset_title = _("Other Codeset");

	str = manual_charset
		? g_strdup_printf("%s (%s)", other_codeset_title, manual_charset)
		: g_strdup_printf("%s...", other_codeset_title);

	if (!menu_item_manual_charset) {
		menu_item_manual_charset = gtk_menu_item_new_with_label(str);
		label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(menu_item_manual_charset)));
	} else
//		gtk_label_set_text(GTK_LABEL(GTK_BIN(menu_item_manual_charset)->child), str);
		gtk_label_set_text(label, str);
	g_free(str);

	return menu_item_manual_charset;
}

static gboolean get_manual_charset(GtkComboBox *option_menu, FileInfo *selected_fi)
{
	GtkWidget *dialog;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry;
	GError *err = NULL;
	gchar *str;

	dialog = gtk_dialog_new_with_buttons(other_codeset_title,
			GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(option_menu))),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			_("_Cancel"), GTK_RESPONSE_CANCEL,
			_("_OK"), GTK_RESPONSE_OK,
			NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
	gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), vbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("Code_set:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

	entry = gtk_entry_new();
	gtk_widget_set_hexpand (entry, TRUE);
	gtk_widget_set_vexpand (entry, TRUE);
	gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
	gtk_label_set_mnemonic_widget(GTK_LABEL(label), entry);
	gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);

	gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK, FALSE);
	g_signal_connect_after(G_OBJECT(entry), "changed",
		G_CALLBACK(toggle_sensitivity), NULL);
	if (selected_fi->charset_flag)
		gtk_entry_set_text(GTK_ENTRY(entry), selected_fi->charset);

	gtk_widget_show_all(vbox);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		g_convert("TEST", -1, "UTF-8", gtk_entry_get_text(GTK_ENTRY(entry)), NULL, NULL, &err);
		if (err) {
			g_error_free(err);
			gtk_widget_hide(dialog);
			str = g_strdup_printf(_("'%s' is not supported"), gtk_entry_get_text(GTK_ENTRY(entry)));
			run_dialog_message(gtk_widget_get_toplevel(GTK_WIDGET(option_menu)),
				GTK_MESSAGE_ERROR, str);
			g_free(str);
		} else {
			g_free(selected_fi->charset);
			selected_fi->charset = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
			selected_fi->charset_flag = TRUE;
			gtk_widget_destroy(dialog);

			init_menu_item_manual_charset(selected_fi->charset_flag
				? selected_fi->charset : NULL);

			return TRUE;
		}
	}
	gtk_widget_destroy(dialog);

	return FALSE;
}

gboolean charset_menu_init_flag;

static void cb_select_charset(GtkComboBox *option_menu, FileInfo *selected_fi)
{
	CharsetTable *ctable;
	static guint index_history = 0, prev_history;

	prev_history = index_history;
	index_history = gtk_combo_box_get_active(option_menu);
	if (!charset_menu_init_flag) {
		ctable = get_charset_table();
		if (index_history < ctable->num + mode) {
			if (selected_fi->charset)
				g_free(selected_fi->charset);
			if (index_history == 0 && mode == OPEN)
				selected_fi->charset = NULL;
			else {
				selected_fi->charset =
					g_strdup(ctable->charset[index_history - mode]);
			}
		} else
			if (!get_manual_charset(option_menu, selected_fi)) {
				index_history = prev_history;
				gtk_combo_box_set_active(option_menu, index_history);
			}
	}
}

static GtkWidget *create_charset_menu(FileInfo *selected_fi)
{
	GtkListStore *store = gtk_list_store_new (1, G_TYPE_STRING);
	GtkTreeIter iter;
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkWidget *option_menu = gtk_combo_box_new_with_model (GTK_TREE_MODEL (store));
	CharsetTable *ctable;
	guint i;

	if (mode == OPEN) {
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, _("Auto-Detect"), -1);
	}
	ctable = get_charset_table();
	for (i = 0; i < ctable->num; i++) {
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, ctable->str[i], -1);
	}
	menu_item_manual_charset = NULL;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, gtk_menu_item_get_label(GTK_MENU_ITEM(init_menu_item_manual_charset(selected_fi->charset_flag
			? selected_fi->charset : NULL))), -1);

	charset_menu_init_flag = TRUE;
	g_signal_connect(G_OBJECT(option_menu), "changed",
		G_CALLBACK(cb_select_charset), selected_fi);
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (option_menu), renderer, FALSE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (option_menu), renderer, "text", 0, NULL);
	i = 0;
	if (selected_fi->charset) {
		do {
			if (g_ascii_strcasecmp(selected_fi->charset, ctable->charset[i]) == 0)
				break;
			i++;
		} while (i < ctable->num);
		if (mode == OPEN && selected_fi->charset_flag == FALSE) {
			g_free(selected_fi->charset);
			selected_fi->charset = NULL;
		} else if (i == ctable->num && selected_fi->charset_flag == FALSE) {
			init_menu_item_manual_charset(selected_fi->charset);
		}
		i += mode;
	}
	if (mode == SAVE || selected_fi->charset_flag)
		gtk_combo_box_set_active(GTK_COMBO_BOX(option_menu), i);
	charset_menu_init_flag = FALSE;

	return option_menu;
}

static GtkWidget *create_file_selector(FileInfo *selected_fi)
{
	GtkWidget *selector;
	GtkWidget *align;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *option_menu_charset;
	GtkWidget *option_menu_lineend;
	const gchar *title;

	title = mode ? _("Open") : _("Save As");

	selector = gtk_file_chooser_dialog_new(title, NULL,
		mode ? GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE,
		_("_Cancel"), GTK_RESPONSE_CANCEL,
		mode ? _("_Open") : _("_Save As"), GTK_RESPONSE_OK,
		NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(selector), GTK_RESPONSE_OK);

	align = gtk_alignment_new(1, 0, 0, 0);
	gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(selector), align);
	table = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(align), table);
	option_menu_charset = create_charset_menu(selected_fi);
	label = gtk_label_new_with_mnemonic(_("C_haracter Coding:"));
	gtk_label_set_mnemonic_widget(GTK_LABEL(label), option_menu_charset);
	gtk_grid_attach(GTK_GRID(table), label, 0, 0, 1, 1);
	gtk_grid_set_column_spacing(GTK_GRID(table), 8);
	gtk_grid_attach(GTK_GRID(table), option_menu_charset, 1, 0, 1, 1);
	if (mode == SAVE) {
		option_menu_lineend = create_lineend_menu(selected_fi);
		gtk_grid_attach(GTK_GRID(table), option_menu_lineend, 2, 0, 1, 1);
	}
	gtk_widget_show_all(align);

	if (selected_fi->filename)
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(selector), selected_fi->filename);

	return selector;
}

FileInfo *get_fileinfo_from_selector(FileInfo *fi, gint requested_mode)
{
	FileInfo *selected_fi;
	GtkWidget *selector;
	gchar *basename, *str;
	gint res, len;

	/* init values */
	mode = requested_mode;
	selected_fi = g_malloc(sizeof(FileInfo));
	selected_fi->filename =
		fi->filename ? g_strdup(fi->filename) : NULL;
	selected_fi->charset =
		fi->charset ? g_strdup(fi->charset) : NULL;
	selected_fi->charset_flag = fi->charset_flag;
	selected_fi->lineend = fi->lineend;

	selector = create_file_selector(selected_fi);
	gtk_window_set_transient_for(GTK_WINDOW(selector),
		GTK_WINDOW(pub->mw->window));

	do {
		res = gtk_dialog_run(GTK_DIALOG(selector));
		if (res == GTK_RESPONSE_OK) {
			if (selected_fi->filename)
				g_free(selected_fi->filename);
			selected_fi->filename =
				gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(selector));
			if (g_file_test(selected_fi->filename, G_FILE_TEST_IS_DIR)) {
				len = strlen(selected_fi->filename);
				if (len < 1 || selected_fi->filename[len - 1] != G_DIR_SEPARATOR)
					str = g_strconcat(selected_fi->filename, G_DIR_SEPARATOR_S, NULL);
				else
					str = g_strdup(selected_fi->filename);
				gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(selector), str);
				g_free(str);
				continue;
			}
			if ((mode == SAVE) && g_file_test(selected_fi->filename, G_FILE_TEST_EXISTS)) {
				basename = g_path_get_basename(selected_fi->filename);
				str = g_strdup_printf(_("'%s' already exists. Overwrite?"), basename);
				g_free(basename);
				res = run_dialog_message_question(selector, str);
				g_free(str);
				switch (res) {
				case GTK_RESPONSE_NO:
					continue;
				case GTK_RESPONSE_YES:
					res = GTK_RESPONSE_OK;
				}
			}
		}
		gtk_widget_hide(selector);
	} while (gtk_widget_get_visible(selector));

	if (res != GTK_RESPONSE_OK) {
		if (selected_fi->charset)
			g_free(selected_fi->charset);
		selected_fi = NULL;
		g_free(selected_fi);
	}

	gtk_widget_destroy(selector);

	return selected_fi;
}
