#include <gtk/gtk.h>

int global_rows;
int global_columns;
int global_mines;
gboolean global_won;
double global_scale;
pthread_t timer_thread;

//this probably shouldnt be in global but oh well, too late now! 
void start_new_game(int rows, int columns,int mines, pthread_t* thread);
GtkWidget* window; //main game window
GtkWidget* main_grid;
