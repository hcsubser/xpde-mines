#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gtk/gtk.h>
#include <glib.h>

#include "global.h"
#include "menubar.h"
#include "cairo_drawings.h"

static void on_window_close(GtkWidget *w, void *_data)
{
	gtk_main_quit();
}
void new_game_callback(GtkWidget *w, void *data)
{
	start_new_game(global_rows,global_columns,global_mines,&timer_thread);
}
void beginner_callback(GtkWidget *w, void *data)
{
	start_new_game(9,9,10,&timer_thread);
}
void intermediate_callback(GtkWidget *w, void *data)
{
	start_new_game(16,16,40,&timer_thread);
}
void expert_callback(GtkWidget *w, void *data)
{
	start_new_game(30,16,99,&timer_thread);
}
void ttt()
{
	global_scale=1;
	gtk_widget_queue_resize(main_grid);
	gtk_window_resize(GTK_WINDOW(window),10,10);
	gtk_widget_queue_draw(window);
	gtk_widget_queue_resize(main_grid);
	gtk_widget_queue_draw(main_grid);
	gtk_window_resize(GTK_WINDOW(window),10,10);
}
void s1_callback(GtkWidget *w, void *data)
{
	global_scale=1;
	//gtk_widget_queue_resize(main_grid);
	gtk_window_resize(GTK_WINDOW(window),10,10);
	gtk_widget_queue_draw(window);
}
void s15_callback(GtkWidget *w, void *data)
{
	global_scale=1.5;
	gtk_widget_queue_draw(window);
	gtk_window_resize(GTK_WINDOW(window),10,10);
}
void s2_callback(GtkWidget *w, void *data)
{
	global_scale=2;
	gtk_widget_queue_draw(window);
	gtk_window_resize(GTK_WINDOW(window),10,10);
}




void custom_ok_callback(GtkWidget *w, void *data)
{
	GtkGrid* grid = (GtkGrid*)data;
	int rows = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(grid,1,0))));
	int columns=atoi(gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(grid,1,1))));
	int mines = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(grid,1,2))));
	//some hard-coded limitations in original minesweeper
	if(rows<9) rows=9;
	if(columns<9) columns=9;
	if(mines<10) mines=10;
	if(rows>30) rows=30;
	if(columns>24) columns=24;
	if(mines>0.605*rows*columns) mines=(int)(0.605*rows*columns);
	
	gtk_widget_destroy(gtk_widget_get_parent((GtkWidget*)grid));
	start_new_game(rows,columns,mines,&timer_thread);
}

void custom_cancel_callback(GtkWidget *w, void *data)
{
	gtk_widget_destroy((GtkWidget*)data);
}

void custom_game_callback(GtkWidget *w, void *data) {
	GtkWidget* custom_game_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* grid = gtk_grid_new();
	GtkWidget* ok = gtk_button_new_with_label("OK");
	GtkWidget* cancel = gtk_button_new_with_label("Cancel");
	GtkWidget* rows = gtk_entry_new();
	GtkWidget* columns = gtk_entry_new();
	GtkWidget* mines = gtk_entry_new();
	
	gtk_window_set_modal(GTK_WINDOW(custom_game_window),TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(custom_game_window),GTK_WINDOW(window));
	gtk_container_add(GTK_CONTAINER(custom_game_window),grid);

	gtk_grid_attach (GTK_GRID(grid),gtk_label_new("Height:"),0,0,1,1);
	gtk_grid_attach (GTK_GRID(grid),gtk_label_new("Width:"),0,1,1,1);
	gtk_grid_attach (GTK_GRID(grid),gtk_label_new("Mines:"),0,2,1,1);
	gtk_grid_attach (GTK_GRID(grid),rows,1,0,1,1);
	gtk_grid_attach (GTK_GRID(grid),columns,1,1,1,1);
	gtk_grid_attach (GTK_GRID(grid),mines,1,2,1,1);
	gtk_grid_attach (GTK_GRID(grid),ok,2,0,1,1);
	gtk_grid_attach (GTK_GRID(grid),cancel,2,2,1,1);

	gtk_entry_set_input_purpose(GTK_ENTRY(rows),GTK_INPUT_PURPOSE_NUMBER);
	gtk_entry_set_input_purpose(GTK_ENTRY(columns),GTK_INPUT_PURPOSE_NUMBER);
	gtk_entry_set_input_purpose(GTK_ENTRY(mines),GTK_INPUT_PURPOSE_NUMBER);
	
	gtk_entry_set_text(GTK_ENTRY(rows),g_strdup_printf("%d",global_rows));
	gtk_entry_set_text(GTK_ENTRY(columns),g_strdup_printf("%d",global_columns));
	gtk_entry_set_text(GTK_ENTRY(mines),g_strdup_printf("%d",global_mines));

    g_signal_connect(ok, "clicked", G_CALLBACK(custom_ok_callback), grid);
    g_signal_connect(cancel, "clicked", G_CALLBACK(custom_cancel_callback), custom_game_window);
    gtk_widget_show_all(custom_game_window);
}

void populate_menubar(GtkMenuBar* menubar)
{
	GtkWidget* game = gtk_menu_item_new_with_label("Game");
	GtkWidget* help = gtk_menu_item_new_with_label("Help");
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), game); 
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
	
	// Game menu
	GtkWidget* game_menu = gtk_menu_new();
	GtkWidget* new_game = gtk_menu_item_new_with_label("New");
	GtkWidget* beginner = gtk_menu_item_new_with_label("Beginner");
	GtkWidget* intermediate = gtk_menu_item_new_with_label("Intermediate");
	GtkWidget* expert = gtk_menu_item_new_with_label("Expert");
	GtkWidget* custom = gtk_menu_item_new_with_label("Custom...");
	GtkWidget* s1 = gtk_menu_item_new_with_label("1x");
	GtkWidget* s15 = gtk_menu_item_new_with_label("1.5x");
	GtkWidget* s2 = gtk_menu_item_new_with_label("2x");
	GtkWidget* quit = gtk_menu_item_new_with_label("Quit");

	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), new_game);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), beginner);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), intermediate);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), expert);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), custom);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), s1);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), s15);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), s2);
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(game_menu), quit);
	
    g_signal_connect(new_game, "activate", G_CALLBACK(new_game_callback), NULL);
    g_signal_connect(beginner, "activate", G_CALLBACK(beginner_callback), NULL);
    g_signal_connect(intermediate, "activate", G_CALLBACK(intermediate_callback),NULL);
    g_signal_connect(expert, "activate", G_CALLBACK(expert_callback), NULL);
    g_signal_connect(custom, "activate", G_CALLBACK(custom_game_callback), NULL);
    g_signal_connect(s1, "activate", G_CALLBACK(s1_callback), NULL);
    g_signal_connect(s15, "activate", G_CALLBACK(s15_callback), NULL);
    g_signal_connect(s2, "activate", G_CALLBACK(s2_callback), NULL);
    g_signal_connect(quit, "activate", G_CALLBACK(on_window_close), NULL);

	//add submenu to game item	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(game), game_menu);
}
