#include <gtk/gtk.h>

//typedef struct MFButton;
typedef struct MFButton{
	GtkWidget *widget;
	GtkWidget *drawing;
	
	gboolean open;
	gboolean mine;
	gboolean flagged;
	gboolean hovered;
	gboolean pressed;
	
	int adjacent_mines;
	int adjacent_mines_remaining;
	
	struct MFButton *up;
	struct MFButton *down;
	struct MFButton *left;
	struct MFButton *right;
} MFButton;

MFButton* minefield_button_new();
