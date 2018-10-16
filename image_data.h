#pragma once

typedef unsigned char pixelData;

typedef struct {
	unsigned int w, h;
	pixelData **pixel;
} ImageData;

ImageData * loadData(GdkPixbuf *pb);

void freeData(ImageData *data);