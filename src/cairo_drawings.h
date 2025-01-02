#include <gtk/gtk.h>
//#include <glib.h>
//#include <cairo_drawings.h>



GtkWidget *drawing_top_new();
GtkWidget *drawing_left_new();
GtkWidget *drawing_right_new();
GtkWidget *drawing_bottom_new();

void drawing_init_surfaces();
void drawing_top_queue_redraw();

cairo_surface_t* return_mines_number(int d);
cairo_surface_t* return_mine_drawing(int d);
