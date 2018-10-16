/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>

void to_bw (GdkPixbuf *pb)
{ 
  int ht,wt;
  int i=0,j=0;
  int rowstride=0;  
  int bpp=0;
  double avg=0;
  gchar *pixel;


  if(gdk_pixbuf_get_bits_per_sample(pb)!=8)   //we handle only 24 bit images.
  	return;                               //look at 3 bytes per pixel.

  bpp=3;	         	  		  //getting attributes of height,
  ht=gdk_pixbuf_get_height(pb);   //width, and bpp.Also get pointer
  wt=gdk_pixbuf_get_width(pb);	  //to pixels.
  pixel=gdk_pixbuf_get_pixels(pb);
  rowstride=wt*bpp;

  for(i=0;i<ht;i++)		//iterate over the height of image.
    for(j=0;j<rowstride;j+=bpp)   //read every pixel in the row.skip
				//bpp bytes 
      {	
      
	//find avg of the pixel to grey it.

	/*avg+=pixel[i*rowstride + j+0]+pixel[i*rowstride + j+1]+pixel[i*rowstride + j+2];
	avg/=3.00;
	avg=((int)(avg))%256;

	pixel[i*rowstride + j+0]=(int)avg;
	pixel[i*rowstride + j+1]=(int)avg;
	pixel[i*rowstride + j+2]=(int)avg;*/
	    if (pixel[i*rowstride + j+0]+pixel[i*rowstride + j+1]+pixel[i*rowstride + j+2] > 0) {
			pixel[i*rowstride + j+0] = 0;
			pixel[i*rowstride + j+1] = 0;
			pixel[i*rowstride + j+2] = 0;
	    } else {
			pixel[i*rowstride + j+0] = 255;
			pixel[i*rowstride + j+1] = 255;
			pixel[i*rowstride + j+2] = 255;
	    }  
    }
  return;
}

void to_greyscale (GdkPixbuf *pb)
{ 
  int ht,wt;
  int i=0,j=0;
  int rowstride=0;  
  int bpp=0;
  double avg=0;
  gchar *pixel;


  if(gdk_pixbuf_get_bits_per_sample(pb)!=8)   //we handle only 24 bit images.
  	return;                               //look at 3 bytes per pixel.

  bpp=3;	         	  		  //getting attributes of height,
  ht=gdk_pixbuf_get_height(pb);   //width, and bpp.Also get pointer
  wt=gdk_pixbuf_get_width(pb);	  //to pixels.
  pixel=gdk_pixbuf_get_pixels(pb);
  rowstride=wt*bpp;

  for(i=0;i<ht;i++)		//iterate over the height of image.
    for(j=0;j<rowstride;j+=bpp)   //read every pixel in the row.skip
				//bpp bytes 
      {	
      
		//find avg of the pixel to grey it.

		avg+=pixel[i*rowstride + j+0]+pixel[i*rowstride + j+1]+pixel[i*rowstride + j+2];
		avg/=3.00;
		avg=((int)(avg))%256;

		pixel[i*rowstride + j+0]=(int)avg;
		pixel[i*rowstride + j+1]=(int)avg;
		pixel[i*rowstride + j+2]=(int)avg;  
    }
  return;
}

static void on_open_image (GtkButton* button, gpointer *user_data)
{
	GtkWidget *image = GTK_WIDGET (user_data[0]);
	GtkWidget *image_grey = GTK_WIDGET (user_data[1]);
	GtkWidget *image_bw = GTK_WIDGET (user_data[2]);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (_("Open image"),
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 NULL);

	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);
	
	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			gchar *filename = 
				gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

			GdkPixbuf *pb_orig, *pb_grey, *pb_bw;                         //pixbuf structure
			//GtkWidget *im;                                      //gtk widget

			pb_orig = gdk_pixbuf_new_from_file(filename, NULL);        //load the image to pixbuf

			gtk_image_set_from_pixbuf (GTK_IMAGE (image), pb_orig);

			pb_grey = gdk_pixbuf_new_from_file(filename, NULL);

			to_greyscale(pb_grey);

			gtk_image_set_from_pixbuf (GTK_IMAGE (image_grey), pb_grey);

			pb_bw = gdk_pixbuf_new_from_file(filename, NULL);

			to_bw(pb_bw);

			gtk_image_set_from_pixbuf (GTK_IMAGE (image_bw), pb_bw);

			//gtk_image_set_from_file (GTK_IMAGE (image), filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy (dialog);
}

static GtkWidget* create_window (void)
{
	GtkWidget *window;
	GtkWidget *open;
	GtkWidget *quit;
	GtkWidget *image;
	GtkWidget *image_grey;
	GtkWidget *image_bw;
	GtkWidget *wrapper;
	GtkWidget *button_box;

	/* Set up the UI */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "demo");
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	wrapper = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

	open = gtk_button_new_with_label (_("Open image"));
	quit = gtk_button_new_with_label (_("Quit"));

	image = gtk_image_new ();
	image_grey = gtk_image_new ();
	image_bw = gtk_image_new ();


	gtk_box_pack_start (GTK_BOX (wrapper), image, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (wrapper), image_grey, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (wrapper), image_bw, TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (button_box), open, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (button_box), quit, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (wrapper), button_box, TRUE, TRUE, 0);

	gtk_container_add (GTK_CONTAINER (window), wrapper);

	/* Connect signals */

	gpointer *images = (gpointer *) malloc(3 * sizeof(gpointer));
	images[0] = image;
	images[1] = image_grey;
	images[2] = image_bw;

	/* Show open dialog when opening a file */
	g_signal_connect (open, "clicked", G_CALLBACK (on_open_image), images);
	
	//g_signal_connect (greyscale, "clicked", G_CALLBACK (on_greyscaling), image);
	
	
	/* Exit when the window is closed */
	g_signal_connect (quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	
	return window;
}


int
main (int argc, char *argv[])
{
 	GtkWidget *window;
	
	gtk_init (&argc, &argv);

	window = create_window ();
	gtk_widget_show_all (window);

	gtk_main ();
	return 0;
}
