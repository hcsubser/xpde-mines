#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include <gtk/gtk.h>
#include <glib.h>

#include "cairo_drawings.h"
#include "minefield_button.h"
#include "minefield.h"
#include "counters.h"
#include "global.h"
#include "menubar.h"

GtkWidget* game_grid;
GtkWidget* main_grid;
//GtkWidget *h_box;

static void on_window_close(GtkWidget *w, void *_data)
{
	gtk_main_quit();
}
void start_new_game(int rows, int columns,int mines, pthread_t* thread)
{
	global_rows=rows;
	global_columns=columns;
	global_mines=mines;
	
	//game_set_running(FALSE);
	open_minefields_counter=0;
	
	pthread_join(timer_thread, NULL);
	
	mine_counter=mines;
	open_minefields_counter=rows*columns-mines;
	
	void empty_grid();
	gtk_widget_destroy(game_grid);
	game_grid= gtk_grid_new();
	init_minefield_for_gtk_grid(GTK_GRID(game_grid),rows,columns,mine_counter);
	gtk_widget_show_all(game_grid);
	gtk_grid_attach (GTK_GRID(main_grid),game_grid,1,2,1,1);
	gtk_widget_set_size_request(game_grid,100,100);
	//gtk_box_pack_start (GTK_BOX(h_box),game_grid,TRUE,TRUE,0);
	
	reset_timer();
	
	// start_timer();
	//pthread_t timer_thread;
	if (pthread_create(thread, NULL, timer, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }
}

static void on_activate (GtkApplication* app, void *_data)
{
	//main skeleton
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *menubar = gtk_menu_bar_new();
	main_grid = gtk_grid_new();
	game_grid = gtk_grid_new();

	global_scale=1;

	//borders
	drawing_init_surfaces();
	GtkWidget *drawing_top = drawing_top_new();
	GtkWidget *drawing_left = drawing_left_new();
	GtkWidget *drawing_right = drawing_right_new();
	GtkWidget *drawing_bottom = drawing_bottom_new();

	//styling for gtk_button (minefield buttons)
	GtkCssProvider *css = gtk_css_provider_new();
	GdkScreen *screen = gdk_display_get_default_screen(gdk_display_get_default());
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_data(css,
        "#special-btn {padding: 0px; border: 0px; border-width: 0px; margin: 0px; min-height: 1px; min-width: 1px;}", -1, NULL);
	
	//populating gtk_grid
	gtk_grid_attach (GTK_GRID(main_grid),menubar,0,0,3,1);
	gtk_grid_attach (GTK_GRID(main_grid),drawing_top,0,1,3,1);
	gtk_grid_attach (GTK_GRID(main_grid),drawing_left,0,2,1,1);
	gtk_grid_attach (GTK_GRID(main_grid),drawing_right,2,2,1,1);
	gtk_grid_attach (GTK_GRID(main_grid),drawing_bottom,0,3,3,1);
	
	populate_menubar(GTK_MENU_BAR(menubar));
	start_new_game(9,9,10,&timer_thread);

	//window settings
	gtk_container_add(GTK_CONTAINER(window),main_grid);
	gtk_window_set_title(GTK_WINDOW(window),"mines");
	gtk_widget_set_size_request(window,100,100);
	//gtk_widget_set_size_request(main_grid,100,100);
	//gtk_widget_queue_resize(main_grid);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    g_signal_connect (window, "destroy", G_CALLBACK (on_window_close), NULL);
		
	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_main();
}

int
main (int argc, char **argv)
{
    GtkApplication * app = gtk_application_new ("com.github.hcsubser.xpde-minesweeper", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
