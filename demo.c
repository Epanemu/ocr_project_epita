#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui_handlers.h"
#include "image_data.h"

int main (int argc, char *argv[]) {

	GtkWidget *window;
	
	gtk_init(&argc, &argv);

	window = create_window();
	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
