#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gtk/gtk.h>
#include <glib.h>

#include "minefield_button.h"

MFButton *head;
	
unsigned int xorshift32(unsigned int *state) {
    *state ^= *state << 13;
    *state ^= *state >> 17;
    *state ^= *state << 5;
    return *state;
}

void empty_grid()
{
	MFButton* pom_h;
	MFButton* pom_v;
	MFButton* pom;
	pom_v=head;
	//pom_h=head->down;
	pom=pom_v;
	while(pom_v!=NULL)
	{
		while(pom!=NULL)
		{
			pom_h=pom;
			pom=pom->down;
			//pom_h=pom_h->right;
			free(pom_h->widget);
			free(pom_h);
		}
		pom=pom_v;
		pom_v=pom_v->right;
		free(pom->widget);
		free(pom);
	}
}

void minefield_end_game_lost()
{
	MFButton* pom_v;
	MFButton* pom;
	pom_v=head;
	pom=pom_v;
	while(pom_v!=NULL)
	{
		while(pom!=NULL)
		{
			pom->open=TRUE;
			pom=pom->down;
		}
		pom_v=pom_v->right;
		pom=pom_v;
	}
	gtk_widget_queue_draw(gtk_widget_get_parent(head->widget));
}

void minefield_end_game_won()
{
	MFButton* pom_v;
	MFButton* pom;
	pom_v=head;
	pom=pom_v;
	while(pom_v!=NULL)
	{
		while(pom!=NULL)
		{
			if(pom->mine==TRUE)
				pom->flagged=TRUE;
			else
				pom->open=TRUE;
			pom=pom->down;
		}
		pom_v=pom_v->right;
		pom=pom_v;
	}
	gtk_widget_queue_draw(gtk_widget_get_parent(head->widget));
}

void init_minefield_for_gtk_grid(GtkGrid *grid, int rows, int columns, int mines)
{
	MFButton* minefield_array[rows][columns];
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++){
			minefield_array[i][j] = minefield_button_new();
			gtk_grid_attach(grid,minefield_array[i][j]->widget,i,j,1,1);
		}
	}
	head = minefield_array[0][0];
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++){
			if(j+1<columns)
				minefield_array[i][j]->down = minefield_array[i][j+1];
			if(j!=0)
				minefield_array[i][j]->up = minefield_array[i][j-1];
			if(i+1<rows)
				minefield_array[i][j]->right  = minefield_array[i+1][j];
			if(i!=0)
				minefield_array[i][j]->left  = minefield_array[i-1][j];
		}
	}
	//mines
	unsigned int state = time(NULL);
	
	while(mines > 0) {
    	int x = xorshift32(&state) % rows;
    	int y = xorshift32(&state) % columns;
    
    	if (minefield_array[x][y]->mine == FALSE) {
    	    minefield_array[x][y]->mine = TRUE;
    	    // Adjust the bounds for the neighboring cells
    	    int x_start = (x > 0) ? x - 1 : x;  // Ensure x doesn't go below 0
    	    int y_start = (y > 0) ? y - 1 : y;  // Ensure y doesn't go below 0
    	    int x_end = (x < rows - 1) ? x + 1 : x;
    	    int y_end = (y < columns - 1) ? y + 1 : y;
    	    // Loop through the surrounding cells
    	    for (int i = x_start; i <= x_end; i++) {
    	        for (int j = y_start; j <= y_end; j++) {
    	            // Only increment if we are not accessing the current mine cell
    	            if (minefield_array[i][j]->mine == FALSE) {
    	                minefield_array[i][j]->adjacent_mines++;
    	                minefield_array[i][j]->adjacent_mines_remaining++;
    	            }
    	        }
    	    }
    	    mines--;  // Decrease the mine count
   		}
    }
}
