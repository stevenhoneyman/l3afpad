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
#include <gdk/gdkkeysyms.h>

#define accel_group gtk_ui_manager_get_accel_group(ifactory)

static GtkWidget *menu_item_save;
static GtkWidget *menu_item_cut;
static GtkWidget *menu_item_copy;
static GtkWidget *menu_item_paste;
static GtkWidget *menu_item_delete;

static GtkActionEntry menu_items[] =
{
	{ "File", NULL, N_("_File"), NULL, NULL, NULL },
	{ "Edit", NULL, N_("_Edit"), NULL, NULL, NULL },
	{ "Search", NULL, N_("_Search"), NULL, NULL, NULL },
	{ "Options", NULL, N_("_Options"), NULL, NULL, NULL },
	{ "Help", NULL, N_("_Help"), NULL, NULL, NULL },
	{ "New", GTK_STOCK_NEW, N_("_New"), "<control>N", NULL, G_CALLBACK(on_file_new) },
	{ "Open", GTK_STOCK_OPEN, N_("_Open..."), "<control>O", NULL, G_CALLBACK(on_file_open) },
	{ "Save", GTK_STOCK_SAVE, N_("_Save"), "<control>S", NULL, G_CALLBACK(on_file_save) },
	{ "SaveAs", GTK_STOCK_SAVE_AS, N_("Save _As..."), "<shift><control>S", NULL, G_CALLBACK(on_file_save_as) },
#if ENABLE_STATISTICS
	{ "Statistics", GTK_STOCK_PROPERTIES, N_("Sta_tistics..."), NULL, NULL, G_CALLBACK(on_file_stats) },
#endif
#if ENABLE_PRINT
	{ "PrintPreview", GTK_STOCK_PRINT_PREVIEW, N_("Print Pre_view"), "<shift><control>P", NULL, G_CALLBACK(on_file_print_preview) },
	{ "Print", GTK_STOCK_PRINT, N_("_Print..."), "<control>P", NULL, G_CALLBACK(on_file_print) },
#endif
	{ "Quit", GTK_STOCK_QUIT, N_("_Quit"), "<control>Q", NULL, G_CALLBACK(on_file_quit) },
	{ "Undo", GTK_STOCK_UNDO, N_("_Undo"), "<control>Z", NULL, G_CALLBACK(on_edit_undo) },
	{ "Redo", GTK_STOCK_REDO, N_("_Redo"), "<shift><control>Z", NULL, G_CALLBACK(on_edit_redo) },
	{ "Cut", GTK_STOCK_CUT, N_("Cu_t"), "<control>X", NULL, G_CALLBACK(on_edit_cut) },
	{ "Copy", GTK_STOCK_COPY, N_("_Copy"), "<control>C", NULL, G_CALLBACK(on_edit_copy) },
	{ "Paste", GTK_STOCK_PASTE, N_("_Paste"), "<control>V", NULL, G_CALLBACK(on_edit_paste) },
	{ "Delete", GTK_STOCK_DELETE, N_("_Delete"), NULL, NULL, G_CALLBACK(on_edit_delete) },
	{ "SelectAll", NULL, N_("Select _All"), "<control>A", NULL, G_CALLBACK(on_edit_select_all) },
	{ "Find", GTK_STOCK_FIND, N_("_Find..."), "<control>F", NULL, G_CALLBACK(on_search_find) },
	{ "FindNext", NULL, N_("Find _Next"), "<control>G", NULL, G_CALLBACK(on_search_find_next) },
	{ "FindPrevious", NULL, N_("Find _Previous"), "<shift><control>G", NULL, G_CALLBACK(on_search_find_previous) },
	{ "Replace", GTK_STOCK_FIND_AND_REPLACE, N_("_Replace..."), "<control>H", NULL, G_CALLBACK(on_search_replace) },
	{ "JumpTo", GTK_STOCK_JUMP_TO, N_("_Jump To..."), "<control>J", NULL, G_CALLBACK(on_search_jump_to) },
	{ "Font", GTK_STOCK_SELECT_FONT, N_("_Font..."), NULL, NULL, G_CALLBACK(on_option_font) },
	{ "About", GTK_STOCK_ABOUT, N_("_About"), NULL, NULL, G_CALLBACK(on_help_about) },
};

static guint nmenu_items = G_N_ELEMENTS (menu_items);

static GtkToggleActionEntry toggle_entries[] =
{
	{ "WordWrap", NULL, N_("_Word Wrap"), NULL, NULL, G_CALLBACK (on_option_word_wrap), FALSE },
	{ "LineNumbers", NULL, N_("_Line Numbers"), NULL, NULL, G_CALLBACK (on_option_line_numbers), FALSE },
	{ "AutoIndent", NULL, N_("_Auto Indent"), NULL, NULL, G_CALLBACK (on_option_auto_indent), FALSE },
};
static guint n_toggle_entries = G_N_ELEMENTS (toggle_entries);

static const gchar *ui_info =
"<ui>"
"  <menubar name='M'>"
"    <menu action='File'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
#if ENABLE_STATISTICS
"      <menuitem action='Statistics'/>"
#endif
#if ENABLE_PRINT
"      <menuitem action='PrintPreview'/>"
"      <menuitem action='Print'/>"
"      <separator/>"
#endif
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='Edit'>"
"      <menuitem action='Undo'/>"
"      <menuitem action='Redo'/>"
"      <separator/>"
"      <menuitem action='Cut'/>"
"      <menuitem action='Copy'/>"
"      <menuitem action='Paste'/>"
"      <menuitem action='Delete'/>"
"      <separator/>"
"      <menuitem action='SelectAll'/>"
"    </menu>"
"    <menu action='Search'>"
"      <menuitem action='Find'/>"
"      <menuitem action='FindNext'/>"
"      <menuitem action='FindPrevious'/>"
"      <menuitem action='Replace'/>"
"      <separator/>"
"      <menuitem action='JumpTo'/>"
"    </menu>"
"    <menu action='Options'>"
"      <menuitem action='Font'/>"
"      <menuitem action='WordWrap'/>"
"      <menuitem action='LineNumbers'/>"
"      <separator/>"
"      <menuitem action='AutoIndent'/>"
"    </menu>"
"    <menu action='Help'>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"</ui>";

static gchar *menu_translate(const gchar *path, gpointer data)
{
	return _(path);
}

void menu_sensitivity_from_modified_flag(gboolean is_text_modified)
{
	gtk_widget_set_sensitive(menu_item_save,   is_text_modified);
}

void menu_sensitivity_from_selection_bound(gboolean is_bound_exist)
{
	gtk_widget_set_sensitive(menu_item_cut,    is_bound_exist);
	gtk_widget_set_sensitive(menu_item_copy,   is_bound_exist);
	gtk_widget_set_sensitive(menu_item_delete, is_bound_exist);
}

//void menu_sensitivity_from_clipboard(gboolean is_clipboard_exist)
void menu_sensitivity_from_clipboard(void)
{
//g_print("clip board checked.\n");
	gtk_widget_set_sensitive(menu_item_paste,
		gtk_clipboard_wait_is_text_available(
			gtk_clipboard_get(GDK_SELECTION_CLIPBOARD)));
}

GtkUIManager *create_menu_bar(GtkWidget *window)
{
	GtkUIManager *ifactory;
#if 0
	gboolean flag_emacs = FALSE;

	gchar *key_theme = NULL;
	GtkSettings *settings = gtk_settings_get_default();
	if (settings) {
		g_object_get(settings, "gtk-key-theme-name", &key_theme, NULL);
		if (key_theme) {
			if (!g_ascii_strcasecmp(key_theme, "Emacs"))
				flag_emacs = TRUE;
			g_free(key_theme);
		}
	}
#endif

	ifactory = gtk_ui_manager_new();
	GtkActionGroup *actions = gtk_action_group_new("Actions");
	gtk_action_group_set_translate_func(actions, menu_translate, NULL, NULL);
	gtk_action_group_add_actions(actions, menu_items, nmenu_items, NULL);
	gtk_action_group_add_toggle_actions (actions, toggle_entries, n_toggle_entries, NULL);
	gtk_ui_manager_insert_action_group(ifactory, actions, 0);
	g_object_unref(actions);
	gtk_ui_manager_add_ui_from_string(ifactory, ui_info, -1, NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	/* hidden keybinds */
	gtk_accel_group_connect(
		accel_group, GDK_W, GDK_CONTROL_MASK, 0,
		g_cclosure_new_swap(G_CALLBACK(on_file_close), NULL, NULL));
	gtk_accel_group_connect(
		accel_group, GDK_T, GDK_CONTROL_MASK, 0,
		g_cclosure_new_swap(G_CALLBACK(on_option_always_on_top), NULL, NULL));
	gtk_widget_add_accelerator(
		gtk_ui_manager_get_widget(ifactory, "/M/Edit/Redo"),
		"activate", accel_group, GDK_Y, GDK_CONTROL_MASK, 0);
	gtk_widget_add_accelerator(
		gtk_ui_manager_get_widget(ifactory, "/M/Search/FindNext"),
		"activate", accel_group, GDK_F3, 0, 0);
	gtk_widget_add_accelerator(
		gtk_ui_manager_get_widget(ifactory, "/M/Search/FindPrevious"),
		"activate", accel_group, GDK_F3, GDK_SHIFT_MASK, 0);
	gtk_widget_add_accelerator(
		gtk_ui_manager_get_widget(ifactory, "/M/Search/Replace"),
		"activate", accel_group, GDK_R, GDK_CONTROL_MASK, 0);

	/* initialize sensitivities */
	gtk_widget_set_sensitive(
		gtk_ui_manager_get_widget(ifactory, "/M/Search/FindNext"),
		FALSE);
	gtk_widget_set_sensitive(
		gtk_ui_manager_get_widget(ifactory, "/M/Search/FindPrevious"),
		FALSE);

	menu_item_save   = gtk_ui_manager_get_widget(ifactory, "/M/File/Save");
	menu_item_cut    = gtk_ui_manager_get_widget(ifactory, "/M/Edit/Cut");
	menu_item_copy   = gtk_ui_manager_get_widget(ifactory, "/M/Edit/Copy");
	menu_item_paste  = gtk_ui_manager_get_widget(ifactory, "/M/Edit/Paste");
	menu_item_delete = gtk_ui_manager_get_widget(ifactory, "/M/Edit/Delete");
	menu_sensitivity_from_selection_bound(FALSE);

	return ifactory;
}
