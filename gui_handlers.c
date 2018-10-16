#include <gtk/gtk.h>

#include "image_modifiers.h"
#include "gui_handlers.h"

static void alert_message (GtkWidget *widget, gchar *message) {
	GtkWidget *parent_window = gtk_widget_get_toplevel (widget);
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(parent_window),
	                                            flags,
	                                            GTK_MESSAGE_ERROR,
	                                            GTK_BUTTONS_CLOSE,
	                                            message);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

static void on_open_image (GtkButton* button, gpointer *widgets) {
	// assign the images given 
	GtkWidget *image_orig = GTK_WIDGET (widgets[0]);
	GtkWidget *image_grey = GTK_WIDGET (widgets[1]);
	GtkWidget *image_bw = GTK_WIDGET (widgets[2]);
	
	// set up a dialog
	GtkWidget *toplevel = gtk_widget_get_toplevel (image_orig);
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open image",
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 "_Open", GTK_RESPONSE_ACCEPT,
	                                                 "_Cancel", GTK_RESPONSE_CANCEL,
	                                                 NULL);

	// set up a filter for only images
	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
	
	// If dialog ended successfuly, manage the image
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		// Get the filename
		gchar *filename = 
			gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		// original
		GdkPixbuf *pb_orig = gdk_pixbuf_new_from_file(filename, NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE (image_orig), pb_orig);

		// grayscale
		GdkPixbuf *pb_grey = gdk_pixbuf_copy(pb_orig);
		to_grayscale(pb_grey);
		gtk_image_set_from_pixbuf(GTK_IMAGE (image_grey), pb_grey);

		// black and white
		GdkPixbuf *pb_bw = gdk_pixbuf_copy(pb_orig);
		to_bw(pb_bw);
		gtk_image_set_from_pixbuf(GTK_IMAGE (image_bw), pb_bw);

	}
	gtk_widget_destroy (dialog);
}

void ocr_run(GtkButton* button, gpointer *data) {
	GtkImage *image = GTK_IMAGE(data);
	GdkPixbuf *pb = gtk_image_get_pixbuf(image);

	//in case of empty image
	if (pb == NULL) {
		alert_message(GTK_WIDGET(data), "You need to select an image first!");
		return;
	}

	ImageData *imageData = loadData(pb);




	// test printout
	int jump_w = imageData->w / 10;
	int jump_h = imageData->h / 10;
	int i, j;
	for (i = 0; i < imageData->h; i+=jump_h) {
		for (j = 0; j < imageData->w; j+=jump_w)
			g_print("%u\t", imageData->pixel[i][j]);
		g_print("\n");
	}

	freeData(imageData);
}

GtkWidget* create_window () {
	// ------------ Set up the UI ---------------
	// top level window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window), "demo");
	gtk_window_set_default_size(GTK_WINDOW (window), 600, 400);

	// buttons
	GtkWidget *open = gtk_button_new_with_label("Open image");
	GtkWidget *ocr = gtk_button_new_with_label("Let the magic happen!");
	GtkWidget *quit = gtk_button_new_with_label("Quit");
	
	// imgaes
	GtkWidget *image_orig = gtk_image_new();
	GtkWidget *image_grey = gtk_image_new();
	GtkWidget *image_bw = gtk_image_new();
	GtkWidget *image_edit = gtk_image_new();

	// wrappers
	GtkWidget *main_w = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *button_w = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

	// add images to main wrapper
	gtk_box_pack_start(GTK_BOX (main_w), image_orig, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (main_w), image_grey, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (main_w), image_bw, TRUE, TRUE, 0);

	// add buttons to button wrapper
	gtk_box_pack_start(GTK_BOX (button_w), open, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (button_w), ocr, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (button_w), quit, TRUE, TRUE, 0);

	// add button wrapper to main wrapper
	gtk_box_pack_start(GTK_BOX (main_w), button_w, TRUE, TRUE, 0);

	// add main wrapper to the window
	gtk_container_add(GTK_CONTAINER (window), main_w);

	// pointers to the widgets must be allocated on the heap
	gpointer *images = (gpointer *) malloc(3 * sizeof(gpointer));
	images[0] = (gpointer *)image_orig;
	images[1] = (gpointer *)image_grey;
	images[2] = (gpointer *)image_bw;

	// Open an image file 
	g_signal_connect(open, "clicked", G_CALLBACK (on_open_image), images);
	
	// Start the OCR process
	g_signal_connect (ocr, "clicked", G_CALLBACK (ocr_run), (gpointer *)image_bw);
	

	// Exit when the window is closed 
	g_signal_connect(quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	
	return window;
}
