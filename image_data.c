#include <gtk/gtk.h>

#include "image_data.h"

ImageData * loadData(GdkPixbuf *pb) {
	ImageData *imageData = (ImageData *) malloc(sizeof(ImageData));

	unsigned int h = gdk_pixbuf_get_height(pb);
	unsigned int w = gdk_pixbuf_get_width(pb);
	guchar *pixels = gdk_pixbuf_get_pixels(pb); 

	unsigned char bpp = 3;
	unsigned int i, j, row = w*bpp;

	imageData->pixel = (pixelData**) malloc(h * sizeof(pixelData*));
	for (i = 0; i < h; i++) {
		imageData->pixel[i] = (pixelData*) malloc(w * sizeof(pixelData));
		for (j = 0; j < w; j++) {
			(imageData->pixel)[i][j] = pixels[i*row + j*bpp];
		}
	}

	imageData->w = w;
	imageData->h = h;

	return imageData;
}

void freeData(ImageData *data) {
	unsigned int i;
	for (i = 0; i < data->h; i++) {
		free(data->pixel[i]);
	}
	free(data->pixel);
	free(data);
}
