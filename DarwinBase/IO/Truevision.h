
#ifndef _TRUEVISION_H_
#define _TRUEVISION_H_


class Truevision {
public:
	static char *	ImportTGA (const char *, int *, int *, int *);
};


#pragma pack (push, n)	// Push the current alignment.
#pragma pack (1)			// Set new alignment.
struct TGAHeader {

	unsigned char	idSize;

	unsigned char	mapType;

	unsigned char	imageType;

	unsigned short	paletteStart;
	unsigned short	paletteSize;
	unsigned char	paletteEntryDepth;

	unsigned short	xOrigin;
	unsigned short	yOrigin;
	unsigned short	width;
	unsigned short	height;
	unsigned char	colorDepth;
	unsigned char	descriptor;

};
#pragma pack (pop, n)	// Restore alignment.


char * Truevision::ImportTGA (const char * fileName,
							  int * width, int * height,
							  int * imageChannels) {

	FILE * pFile = fopen(fileName, "rb");
	if (!pFile) {
		perror(fileName);
		return NULL;
	}

	// Read header.
	TGAHeader imageHeader;
	fread(&imageHeader, sizeof(TGAHeader), 1, pFile);

	* width  = imageHeader.width;
	* height = imageHeader.height;

	if (imageHeader.colorDepth == 24) {
		* imageChannels = 3;
	} else if (imageHeader.colorDepth == 32) {
		* imageChannels = 4;
	} else {
		printf("Not supported: %d bpp!", imageHeader.colorDepth);
		fclose(pFile);
		return NULL;
	}

	// Allocate a buffer for reading.
	int bufferSize = (* imageChannels) * (* width) * (* height);
	char * buffer = (char *) malloc(bufferSize);
	fread(buffer, 1, bufferSize, pFile);

	// Reorganize image based on pixel position on screen.
	char * imageData = (char *) malloc(bufferSize);
	int idx = 0;

	for (int x = 0; x < (* height); x++) {
		for (int y = 0; y < (* width); y++) {

			int y1 = y;
			int x1 = x;

			if (imageHeader.descriptor & (1 << 4)) {
				y1 = (* width) - 1 - y;
			}
			if (!(imageHeader.descriptor & (1 << 5))) {
				x1 = (* height) - 1 - x;
			}

			int k = (x1 * (* width) + y1) * (* imageChannels);
	
			// Image is stored in BGRA format, we need RGBA.
			imageData[idx++] = buffer[k + 2];
			imageData[idx++] = buffer[k + 1];
			imageData[idx++] = buffer[k];
			if (* imageChannels == 4) {
				imageData[idx++] = buffer[k + 3];
			}
		}
	}

	fclose(pFile);
	free(buffer);

	return imageData;
}


#endif
