#define _DEFAULT_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "counters.h"
#include "cairo_drawings.h"
#include "global.h"
#include "minefield.h"


double game_time;
//gboolean running;

void* timer()
{
	/*while(running==FALSE)
	{
		usleep(100000);
	}*/

	while(game_time<1000 && open_minefields_counter>0)
	{
		usleep(100000);
		//printf("\n%d",open_minefields_counter);fflush(stdout);
		game_time=game_time+0.1;
		drawing_top_queue_redraw();
	}
	if(global_won==TRUE)
		minefield_end_game_won();
	else
		minefield_end_game_lost();
	printf("\n%d",global_won);fflush(stdout);
	
	pthread_exit(NULL);
}

/*void start_timer()
{
	pthread_t timer_thread;
	if (pthread_create(&timer_thread, NULL, timer, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }
}*/

/*void game_set_running(gboolean b)
{
	running = b;
}*/

int get_current_time()
{
	return (int)game_time;
}

void get_current_time_digits(int *first, int *second, int *third)
{
	int gt = game_time;
	*third = gt%10;
	*second = gt/10%10;
	*first = gt/100%10;
}
void get_mine_counter_digits(int *first, int *second, int *third)
{
	*third = mine_counter%10;
	*second = mine_counter/10%10;
	*first = mine_counter/100%10;
}

void reset_timer()
{
	game_time=0;
}
