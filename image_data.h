#ifndef IMAGE_DATA_H123
#define IMAGE_DATA_H123

struct image_data {
	unsigned char* pixels;
	int w, h;
	int compPerPixel;
};

#endif