#include <gtk/gtk.h>
#include <glib.h>

#include "minefield_button.h"
#include "counters.h"
#include "cairo_drawings.h"
#include "global.h"
#include "minefield.h"

/*cairo_surface_t get_minefield_png(int n)
{
	cairo_surface_t *open1 = cairo_image_surface_create_from_png("assets/open1.png");
}*/


//minefield_button
void button_not_opened(cairo_t *cr, int width,int height)
{
	//dark gray background
	cairo_set_source_rgb(cr, 0.51, 0.51, 0.51);
	cairo_paint(cr);
	//white triangle
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, width, 0);
	cairo_line_to(cr, 0, height);
	cairo_close_path(cr);
    cairo_fill(cr);
	//gray rectangle
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
	cairo_rectangle(cr, 2*global_scale, 2*global_scale, width-4*global_scale, height-4*global_scale);
    cairo_fill(cr);
}
void button_opened(cairo_t *cr, int width,int height)
{
	cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    cairo_paint(cr);
    //dark gray
	cairo_set_source_rgb(cr, 0.51, 0.51, 0.51);
    cairo_rectangle(cr, width-1*global_scale, 0, 1*global_scale, height);
    cairo_rectangle(cr, 0, height-1*global_scale, width, 1*global_scale);
    cairo_fill(cr);
}

void button_has_mine(cairo_t *cr, gboolean pressed)
{
    cairo_scale (cr,global_scale,global_scale);
    if(pressed==FALSE)
		cairo_set_source_surface(cr, return_mine_drawing(0), 0, 0);
	else
		cairo_set_source_surface(cr, return_mine_drawing(1), 0, 0);
	cairo_paint(cr); 
}
void button_has_flag(cairo_t *cr)
{
    cairo_scale (cr,global_scale,global_scale);
	cairo_set_source_surface(cr, return_mine_drawing(3), 0, 0);
	cairo_paint(cr); 
}

void button_has_adjacent_mines(cairo_t *cr, int adjacent_mines)
{
    cairo_scale (cr,global_scale,global_scale);
    cairo_set_source_surface(cr, return_mines_number(adjacent_mines), 0, 0);
    cairo_paint(cr); 
}


void button_design (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    int width = 16*global_scale;//(gtk_widget_get_allocated_width(GTK_WIDGET(widget)))*global_scale;
    int height = 16*global_scale;//(gtk_widget_get_allocated_height(GTK_WIDGET(widget)))*global_scale;


    gtk_widget_set_size_request (widget, 16*global_scale, 16*global_scale);

    MFButton *btn = (MFButton *)data;
    
    //when the button is pressed but not yet released
    if(btn->pressed==TRUE && btn->flagged==FALSE && btn->open==FALSE) 
    {
    	button_opened(cr,width,height);
    	return;
    }
    //set flag if flagged
    if( (btn->open) ==FALSE)
	{
    	button_not_opened(cr,width,height);
    	if(btn->flagged==TRUE)
    		button_has_flag(cr);
    }
    else
    {//if button has been opened
    	button_opened(cr,width,height);
    	if(btn->mine == TRUE)
    		button_has_mine(cr, btn->pressed);
    	else if(btn->adjacent_mines != 0)
    		button_has_adjacent_mines(cr,btn->adjacent_mines);
    }

}

void adjust_adjacent_mines_remaining(MFButton *btn,int a)
{
	if(btn->down!=NULL)	
		btn->down->adjacent_mines_remaining += a;
	if(btn->up!=NULL)	
		btn->up->adjacent_mines_remaining += a;
	if(btn->left!=NULL)
	{	
		btn->left->adjacent_mines_remaining += a;
		if(btn->left->down!=NULL)	
			btn->left->down->adjacent_mines_remaining += a;
		if(btn->left->up!=NULL)	
			btn->left->up->adjacent_mines_remaining += a;
	}
	if(btn->right!=NULL)
	{	
		btn->right->adjacent_mines_remaining += a;
		if(btn->right->down!=NULL)	
			btn->right->down->adjacent_mines_remaining += a;
		if(btn->right->up!=NULL)	
			btn->right->up->adjacent_mines_remaining += a;
	}

}


void chain_open_open_one_mine(MFButton *btn)
{
	if(btn->open==TRUE)
		return;
	if(btn->mine!=TRUE || btn->flagged!=TRUE)
	{
		/*if(btn->mine==TRUE)
		{
			btn->open=TRUE;
			open_minefields_counter=0;
			//global_running=FALSE;
			btn->pressed=TRUE;
			global_won=FALSE;
			//minefield_end_game_lost();
			return;
		}*/
		
		btn->open=TRUE;
		if(btn->mine==FALSE)
		{
			open_minefields_counter--;
			if(open_minefields_counter==0)
				global_won=TRUE;
		}
		else
		{
			open_minefields_counter=0;
			global_won=FALSE;
		}
		if(btn->flagged==TRUE)
		{
			btn->flagged=FALSE;
			mine_counter++;
			adjust_adjacent_mines_remaining(btn,+1);
		}

	}
}
//Chain opening when an empty mine is pressed.
//This is probably the worst code I've ever written! I am almost proud of it.
void chain_open(MFButton *btn)
{
	if(btn==NULL)
		return;
	if(btn->open!=TRUE || btn->adjacent_mines<=0 || btn->adjacent_mines_remaining!=0)
	{	
		if(btn->open==TRUE)
			return;
		if(btn->adjacent_mines!=0)
			return;
	}
	/*if(btn->mine==TRUE && btn->flagged==FALSE)
	{
		btn->open=TRUE;
		return;
	}
	if(btn->mine==TRUE && btn->flagged==TRUE)
	{
		return;
	}*/
	chain_open_open_one_mine(btn);
	//is there a nicer way to do all this?
	//There's gotta be, I should never ever be allowed near
	// a keyboard again because of this abomination.		
	if(btn->left!=NULL)	
	{
		if(btn->left->adjacent_mines==0 && btn->left->mine==FALSE)
			chain_open(btn->left);
		else
			{
				chain_open_open_one_mine(btn->left);
				if(btn->down!=NULL)	
				{
					if(btn->left->down->adjacent_mines==0 && btn->left->down->mine==FALSE)
						chain_open(btn->left->down);
					else
						chain_open_open_one_mine(btn->left->down);
				}
				if(btn->up!=NULL)
				{
					if(btn->left->up->adjacent_mines==0 && btn->left->up->mine==FALSE)
						chain_open(btn->left->up);
					else
						chain_open_open_one_mine(btn->left->up);
				}
			}
	}
	
	if(btn->right!=NULL)
	{
		if(btn->right->adjacent_mines==0 && btn->right->mine==FALSE)
			chain_open(btn->right);
		else
			{
				chain_open_open_one_mine(btn->right);
				if(btn->down!=NULL)	
				{
					if(btn->right->down->adjacent_mines==0 && btn->right->down->mine==FALSE)
						chain_open(btn->right->down);
					else
						chain_open_open_one_mine(btn->right->down);
				}
				if(btn->up!=NULL)
				{
					if(btn->right->up->adjacent_mines==0 && btn->right->up->mine==FALSE)
						chain_open(btn->right->up);
					else
						chain_open_open_one_mine(btn->right->up);
				}	
			}
	}
	if(btn->down!=NULL)	
	{
		if(btn->down->adjacent_mines==0 && btn->down->mine==FALSE)
			chain_open(btn->down);
		else
			chain_open_open_one_mine(btn->down);
	}
	if(btn->up!=NULL)
	{
		if(btn->up->adjacent_mines==0 && btn->up->mine==FALSE)
			chain_open(btn->up);
		else
			chain_open_open_one_mine(btn->up);
	}
}

//this function is pure chaos but it is readable for now...
void button_released(GtkWidget *button, GdkEventButton *event, gpointer data)
{
	MFButton *btn = ((MFButton *)data);
	btn->pressed=FALSE;
	if(btn->hovered==FALSE)
		return;
	
	if(event->button == 1)//left click
	{
		if(btn->flagged==TRUE)
			return;
		if(btn->mine==TRUE)
			{
				btn->open=TRUE;
				open_minefields_counter=0;
				//global_running=FALSE;
				btn->pressed=TRUE;
				global_won=FALSE;
				//minefield_end_game_lost();
				return;
			}
		if(btn->adjacent_mines==0 || (btn->open==TRUE && btn->adjacent_mines_remaining==0))
			chain_open(btn);
		else
		{
			if(btn->open)
				return;
			btn->open=TRUE;
			if(btn->mine==FALSE)
			{
				open_minefields_counter--;
				if(open_minefields_counter==0)
					global_won=TRUE;

			}
		}
	}
	else if(event->button == 3)//right click
	{
		if(btn->open)
			return;
		if(btn->flagged==FALSE)
		{
			mine_counter--;
			btn->flagged=TRUE;
			adjust_adjacent_mines_remaining(btn,-1);
		}
		else
		{
			mine_counter++;
			btn->flagged=FALSE;
			adjust_adjacent_mines_remaining(btn,+1);
		}
	}
	gtk_widget_queue_draw(gtk_widget_get_parent(btn->widget));
}
void button_pressed(GtkWidget *button, GdkEventButton *event, gpointer data)
{
	MFButton *btn = ((MFButton *)data);
	if(btn->open==TRUE)
		return;
	if(event->button == 1)
		btn->pressed=TRUE;
	gtk_widget_queue_draw(button);
}
gboolean pointer_enters (GtkWidget* self, GdkEventCrossing *event, gpointer data)
{
	MFButton *btn = ((MFButton *)data);
	btn->hovered=TRUE;
	return FALSE;
}
gboolean pointer_leaves (GtkWidget* self, GdkEventCrossing *event, gpointer data)
{
	MFButton *btn = ((MFButton *)data);
	btn->hovered=FALSE;
	return FALSE;
}

MFButton* minefield_button_new()
{
	GtkWidget *button = gtk_button_new();
	gtk_widget_set_name(button,"special-btn");
	//gtk_widget_set_size_request (button, 16, 16);
	
	GtkWidget *drawing = gtk_drawing_area_new();
	//gtk_widget_set_size_request (drawing, 16, 16);
	gtk_container_add(GTK_CONTAINER(button),drawing);
	
	MFButton* mfb = malloc(sizeof(MFButton));
	mfb->widget= malloc(sizeof(GtkWidget));
	mfb->widget=button;
	mfb->drawing=drawing;
	
	mfb->open=FALSE;
	mfb->mine=FALSE;
	mfb->flagged=FALSE;
	mfb->hovered=FALSE;
	mfb->pressed=FALSE;
	
	mfb->right=NULL;
	mfb->down=NULL;
	mfb->left=NULL;
	mfb->up=NULL;
	
	mfb->adjacent_mines=0;
	mfb->adjacent_mines_remaining=0;


	g_signal_connect (G_OBJECT (drawing), "draw", G_CALLBACK (button_design), mfb);
	g_signal_connect (button, "button-press-event", G_CALLBACK (button_pressed), mfb);
	g_signal_connect (button,"button-release-event",G_CALLBACK (button_released),mfb);
	g_signal_connect (button, "enter-notify-event", G_CALLBACK (pointer_enters), mfb);
	g_signal_connect (button, "leave-notify-event", G_CALLBACK (pointer_leaves), mfb);

	return mfb;

}
