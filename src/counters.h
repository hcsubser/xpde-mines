#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int mine_counter;
int open_minefields_counter;

//int get_current_time();
void get_current_time_digits(int *first, int *second, int *third);
void get_mine_counter_digits(int *first, int *second, int *third);

void* timer();
void reset_timer();
//void game_set_running(gboolean b);
