#include <gtk/gtk.h>

void init_minefield_for_gtk_grid(GtkGrid *grid, int rows, int columns, int mines);
void empty_grid();
void minefield_end_game_lost();
void minefield_end_game_won();
