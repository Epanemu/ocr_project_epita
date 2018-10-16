#include <gtk/gtk.h>

#include "image_modifiers.h"

#define BW_THRESHOLD 300

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

	// iterate over the image.
	for (i = 0; i < ht; i++) {
		// read pixels (each is bpp bytes long)
		for (j = 0; j < rowLen; j+=bpp) {
			// absolute values are required beacuse pixels can have negative value
			// which corresponds to the same, but positive
			if (abs(pixel[i*rowLen + j]) + abs(pixel[i*rowLen + j+1]) + abs(pixel[i*rowLen + j+2]) < BW_THRESHOLD) {
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
	double avg;

	// iterate over the image.
	for (i = 0; i < ht; i++) {
		// read pixels (each is bpp bytes long)
		for (j = 0; j < rowLen; j+=bpp) {
			// absolute values are required beacuse pixels can have negative value
			// which corresponds to the same, but positive
			avg = abs(pixel[i*rowLen + j]) + abs(pixel[i*rowLen + j+1]) + abs(pixel[i*rowLen + j+2]);
			avg /= 3;

			pixel[i*rowLen + j] = (int)avg;
			pixel[i*rowLen + j+1] = (int)avg;
			pixel[i*rowLen + j+2] = (int)avg; 
		}
	}
}