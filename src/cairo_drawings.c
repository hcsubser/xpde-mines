#include <gtk/gtk.h>
#include <glib.h>

#include "cairo_drawings.h"
#include "counters.h"

GtkWidget *drawing_top;

cairo_surface_t *num0;
cairo_surface_t *num1;
cairo_surface_t *num2;
cairo_surface_t *num3;
cairo_surface_t *num4;
cairo_surface_t *num5;
cairo_surface_t *num6;
cairo_surface_t *num7;
cairo_surface_t *num8;
cairo_surface_t *num9;

cairo_surface_t *open1;
cairo_surface_t *open2;
cairo_surface_t *open3;
cairo_surface_t *open4;
cairo_surface_t *open5;
cairo_surface_t *open6;
cairo_surface_t *open7;
cairo_surface_t *open8;

cairo_surface_t *mine;
cairo_surface_t *mine_death;
cairo_surface_t *misflagged;
cairo_surface_t *flag;


cairo_surface_t* return_cairo_surface_digit(int d)
{
	switch (d) {
        case 1:
            return num1;
            break;
        case 2:
            return num2;
            break;
        case 3:
            return num3;
            break;
        case 4:
            return num4;
            break;
        case 5:
            return num5;
            break;
        case 6:
            return num6;
            break;
        case 7:
            return num7;
            break;
        case 8:
            return num8;
            break;
        case 9:
            return num9;
            break;
        case 0:
            return num0;
            break;
        default:
            return num0;
	}
}
cairo_surface_t* return_mines_number(int d)
{
	switch (d) {
        case 1:
            return open1;
            break;
        case 2:
            return open2;
            break;
        case 3:
            return open3;
            break;
        case 4:
            return open4;
            break;
        case 5:
            return open5;
            break;
        case 6:
            return open6;
            break;
        case 7:
            return open7;
            break;
        case 8:
            return open8;
            break;
        default:
            return open8;
	}
}

cairo_surface_t* return_mine_drawing(int d)
{
	switch (d) {
        case 0:
            return mine;
            break;
        case 1:
            return mine_death;
            break;
        case 2:
            return misflagged;
            break;
        case 3:
            return flag;
            break;
        default:
            return mine;
	}
}

//drawing top
void drawing_top_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    cairo_paint(cr);
    int width = gtk_widget_get_allocated_width(GTK_WIDGET(widget));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(widget));
    
    //white
	cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, width, 3); //gornja linija
    cairo_rectangle(cr, 0, 0, 3, height); //leva linija vertikalna
    cairo_rectangle(cr, 10, 44, width-20, 2); //donja horizontalna linija
    cairo_rectangle(cr, width-12, 11, 2, 35); //desna vertikalna linija
    cairo_fill(cr);
    
    //dark gray
	cairo_set_source_rgb(cr, 0.51, 0.51, 0.51);
    cairo_rectangle(cr, 8, 9, width-18, 2); //gornja horizontalna linija
    cairo_rectangle(cr, 8, 9, 2, 36); //leva vertikalna linja
    cairo_rectangle(cr, 8, 53, width-18, 4); //donja horizontalna linija
    cairo_fill(cr);

    int a,b,c;
	//mine counter
	if(mine_counter>0)
    	get_mine_counter_digits(&a,&b,&c);
    cairo_set_source_surface(cr, return_cairo_surface_digit(a), 14, 16);
    cairo_paint(cr); 
    cairo_set_source_surface(cr, return_cairo_surface_digit(b), 27, 16);
    cairo_paint(cr); 
    cairo_set_source_surface(cr, return_cairo_surface_digit(c), 40, 16);
    cairo_paint(cr);

	//yellow guy
    cairo_surface_t *image_surface = cairo_image_surface_create_from_png("assets/smile.png");
    cairo_set_source_surface(cr, image_surface, width/2-8.5, 16);
    cairo_paint(cr); 

	//timer
    get_current_time_digits(&a,&b,&c);
    cairo_set_source_surface(cr, return_cairo_surface_digit(a), width-54, 16);
    cairo_paint(cr); 
    cairo_set_source_surface(cr, return_cairo_surface_digit(b), width-41, 16);
    cairo_paint(cr); 
    cairo_set_source_surface(cr, return_cairo_surface_digit(c), width-28, 16);
    cairo_paint(cr); 
    
	//return FALSE;
}

GtkWidget *drawing_top_new()
{
	//drawing areas
	drawing_top = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_top, 104, 57);
	g_signal_connect (G_OBJECT (drawing_top), "draw", G_CALLBACK (drawing_top_callback), NULL);
	return drawing_top;
}
void drawing_top_queue_redraw()
{
	gtk_widget_queue_draw(drawing_top);
}

//drawing left
void drawing_left_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    cairo_paint(cr);
    //int width = gtk_widget_get_allocated_width(GTK_WIDGET(widget));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(widget));
    
    //white
	cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, 3, height);
    cairo_fill(cr);
    
    //dark gray
	cairo_set_source_rgb(cr, 0.51, 0.51, 0.51);
    cairo_rectangle(cr, 8, 0, 4, height);
    cairo_fill(cr);
    //return FALSE;
}

GtkWidget *drawing_left_new()
{
	//drawing areas
	GtkWidget *drawing_left = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_left, 13, 143);
	g_signal_connect (G_OBJECT (drawing_left), "draw", G_CALLBACK (drawing_left_callback), NULL);
	return drawing_left;
}


//drawing right
void drawing_right_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    cairo_paint(cr);
    //int width = gtk_widget_get_allocated_width(GTK_WIDGET(widget));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(widget));
    
    //white
	cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, 3, height);
    cairo_fill(cr);
}

GtkWidget *drawing_right_new()
{
	//drawing areas
	GtkWidget *drawing_right = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_right, 13, 143);
	g_signal_connect (G_OBJECT (drawing_right), "draw", G_CALLBACK (drawing_right_callback), NULL);
	return drawing_right;
}


//drawing bottom
void drawing_bottom_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    cairo_paint(cr);
    int width = gtk_widget_get_allocated_width(GTK_WIDGET(widget));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(widget));
    
    //white
	cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, 3, height);
    cairo_rectangle(cr, 8, 0, width-18, 3);
    cairo_fill(cr);
}

GtkWidget *drawing_bottom_new()
{
	//drawing areas
	GtkWidget *drawing_bottom = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_bottom, 104, 8);
	g_signal_connect (G_OBJECT (drawing_bottom), "draw", G_CALLBACK (drawing_bottom_callback), NULL);
	return drawing_bottom;
}

void drawing_init_surfaces()
{
    num0 = cairo_image_surface_create_from_png("assets/digit0.png");
	num1 = cairo_image_surface_create_from_png("assets/digit1.png");
	num2 = cairo_image_surface_create_from_png("assets/digit2.png");
	num3 = cairo_image_surface_create_from_png("assets/digit3.png");
	num4 = cairo_image_surface_create_from_png("assets/digit4.png");
	num5 = cairo_image_surface_create_from_png("assets/digit5.png");
	num6 = cairo_image_surface_create_from_png("assets/digit6.png");
	num7 = cairo_image_surface_create_from_png("assets/digit7.png");
	num8 = cairo_image_surface_create_from_png("assets/digit8.png");
	num9 = cairo_image_surface_create_from_png("assets/digit9.png");
	
	open1 = cairo_image_surface_create_from_png("assets/open1.png");
	open2 = cairo_image_surface_create_from_png("assets/open2.png");
	open3 = cairo_image_surface_create_from_png("assets/open3.png");
	open4 = cairo_image_surface_create_from_png("assets/open4.png");
	open5 = cairo_image_surface_create_from_png("assets/open5.png");
	open6 = cairo_image_surface_create_from_png("assets/open6.png");
	open7 = cairo_image_surface_create_from_png("assets/open7.png");
	open8 = cairo_image_surface_create_from_png("assets/open8.png");
	
	mine = cairo_image_surface_create_from_png("assets/mine-ceil.png");
	mine_death = cairo_image_surface_create_from_png("assets/mine-death.png");
	misflagged = cairo_image_surface_create_from_png("assets/misflagged.png");
	flag = cairo_image_surface_create_from_png("assets/flag.png");
}

