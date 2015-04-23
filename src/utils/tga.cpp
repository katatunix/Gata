/*-----------------------------------------------------------
This is a very simple TGA lib. It will only load and save 
uncompressed images in greyscale, RGB or RGBA mode.

If you want a more complete lib I suggest you take 
a look at Paul Groves' TGA loader. Paul's home page is at 

http://paulyg.virtualave.net


Just a little bit about the TGA file format.

Header - 12 fields


id						unsigned char
colour map type			unsigned char
image type				unsigned char

1	-	colour map image
2	-	RGB(A) uncompressed
3	-	greyscale uncompressed
9	-	greyscale RLE (compressed)
10	-	RGB(A) RLE (compressed)

colour map first entry	short int
colour map length		short int
map entry size			short int

horizontal origin		short int
vertical origin			short int
width					short int
height					short int
pixel depth				unsigned char

8	-	greyscale
24	-	RGB
32	-	RGBA

image descriptor		unsigned char

From all these fields, we only care about the image type, 
to check if the image is uncompressed and not color indexed, 
the width and height, and the pixel depth.

You may use this library for whatever you want. This library is 
provide as is, meaning that I won't take any responsability for
any damages that you may incur for its usage.

Antonio Ramires Fernandes ajbrf@yahoo.com
-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gata/utils/tga.h>
#include <gata/utils/MyUtils.h>

namespace gata {
	namespace utils {
//===============================================================================================================
// this variable is used for image series
static int savedImages = 0;

// load the image header fields. We only keep those that matter!
void tgaLoadHeader(FILE *file, tgaInfo *info) {

	unsigned char cGarbage;
	short int iGarbage;

	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);

	// type must be 2 or 3
	fread(&info->type, sizeof(unsigned char), 1, file);

	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);

	fread(&info->width, sizeof(short int), 1, file);
	fread(&info->height, sizeof(short int), 1, file);
	fread(&info->pixelDepth, sizeof(unsigned char), 1, file);

	fread(&cGarbage, sizeof(unsigned char), 1, file);
}

// loads the image pixels. You shouldn't call this function
// directly
void tgaLoadImageData(FILE *file, tgaInfo *info) {

	int mode,total,i;
	unsigned char aux;

	// mode equal the number of components for each pixel
	mode = info->pixelDepth / 8;
	// total is the number of bytes we'll have to read
	total = info->height * info->width * mode;

	fread(info->imageData,sizeof(unsigned char),total,file);

	// mode=3 or 4 implies that the image is RGB(A). However TGA
	// stores it as BGR(A) so we'll have to swap R and B.
	if (mode >= 3)
		for (i=0; i < total; i+= mode) {
			aux = info->imageData[i];
			info->imageData[i] = info->imageData[i+2];
			info->imageData[i+2] = aux;
		}
}	

// this is the function to call when we want to load
// an image
tgaInfo * tgaLoad(const char *filename) {

	FILE *file;
	tgaInfo *info;
	int mode,total;

	// allocate memory for the info struct and check!
	info = (tgaInfo *)malloc(sizeof(tgaInfo));
	
	if (info == NULL)
		return(NULL);
	info->imageData = 0;


	// open the file for reading (binary mode)
	int length;
	file = my_fopen(filename, &length);
	if (file == NULL) {
		info->status = TGA_ERROR_FILE_OPEN;
		return(info);
	}

	// load the header
	tgaLoadHeader(file,info);

	// check for errors when loading the header
	if (ferror(file)) {
		info->status = TGA_ERROR_READING_FILE;
		fclose(file);
		return(info);
	}

	// check if the image is color indexed
	if (info->type == 1) {
		info->status = TGA_ERROR_INDEXED_COLOR;
		fclose(file);
		return(info);
	}
	// check for other types (compressed images)
	if ((info->type != 2) && (info->type !=3)) {
		info->status = TGA_ERROR_COMPRESSED_FILE;
		fclose(file);
		return(info);
	}

	// mode equals the number of image components
	mode = info->pixelDepth / 8;
	// total is the number of bytes to read
	total = info->height * info->width * mode;
	// allocate memory for image pixels
	info->imageData = (unsigned char *)malloc(sizeof(unsigned char) * 
		total);

	// check to make sure we have the memory required
	if (info->imageData == NULL) {
		info->status = TGA_ERROR_MEMORY;
		fclose(file);
		return(info);
	}
	// finally load the image pixels
	tgaLoadImageData(file,info);

	// check for errors when reading the pixels
	if (ferror(file)) {
		info->status = TGA_ERROR_READING_FILE;
		fclose(file);
		return(info);
	}
	fclose(file);
	info->status = TGA_OK;
	return(info);
}		

// releases the memory used for the image
void tgaDestroy(tgaInfo *info) {

	if (info) {
		if (info->imageData)
			free(info->imageData);
		free(info);
	}
}
//===============================================================================================================
	}
}
