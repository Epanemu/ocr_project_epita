#include <gtk/gtk.h>

#include "image_modifiers.h"

#define BW_THRESHOLD 100

guchar avg_color (guchar r, guchar g, guchar b) {
	r = abs(r);
	g = abs(g);
	b = abs(b);
	
	g_print("%d\n", (0.3*r + 0.59*g + 0.11*b));
	
	return (guchar)(0.3*r + 0.59*g + 0.11*b);
}

void to_bw (GdkPixbuf *pb) { 
	// image must be 24 bits per pixel
	if(gdk_pixbuf_get_bits_per_sample(pb)!=8)
		return; 

	// bytes per pixel 
	unsigned char bpp = 3;

	// get image atributes and array of pixels 
	int ht = gdk_pixbuf_get_height(pb);
	int wt = gdk_pixbuf_get_width(pb);
	guchar *pixel = gdk_pixbuf_get_pixels(pb);

	// length of a row in bytes
	int rowLen = wt*bpp;

	int i, j;
	guchar avg;

	// iterate over the image.
	for (i = 0; i < ht; i++) {
		// read pixels (each is bpp bytes long)
		for (j = 0; j < rowLen; j+=bpp) {
			// absolute values are required beacuse pixels can have negative value
			// which corresponds to the same, but positive
			avg = avg_color(pixel[i*rowLen + j], pixel[i*rowLen + j+1], pixel[i*rowLen + j+2]);
			
			if (avg < BW_THRESHOLD) {
				// black
				pixel[i*rowLen + j] = 0;
				pixel[i*rowLen + j+1] = 0;
				pixel[i*rowLen + j+2] = 0;
			} else {
				// white
				pixel[i*rowLen + j] = 255;
				pixel[i*rowLen + j+1] = 255;
				pixel[i*rowLen + j+2] = 255;
			}
		}
	}
}

void to_grayscale (GdkPixbuf *pb)
{ 
  // image must be 24 bits per pixel
	if(gdk_pixbuf_get_bits_per_sample(pb)!=8)
		return; 

	// bytes per pixel 
	unsigned char bpp = 3;

	// get image atributes and array of pixels 
	int ht = gdk_pixbuf_get_height(pb);   
	int wt = gdk_pixbuf_get_width(pb);
	guchar *pixel = gdk_pixbuf_get_pixels(pb);

	// length of a row in bytes
	int rowLen = wt*bpp;

	int i, j;
	guchar avg;

	// iterate over the image.
	for (i = 0; i < ht; i++) {
		// read pixels (each is bpp bytes long)
		for (j = 0; j < rowLen; j+=bpp) {
			// absolute values are required beacuse pixels can have negative value
			// which corresponds to the same, but positive
			avg = avg_color(pixel[i*rowLen + j], pixel[i*rowLen + j+1], pixel[i*rowLen + j+2]);

			pixel[i*rowLen + j] = avg;
			pixel[i*rowLen + j+1] = avg;
			pixel[i*rowLen + j+2] = avg; 
		}
	}
}
