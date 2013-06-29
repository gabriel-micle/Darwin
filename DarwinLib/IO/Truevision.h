
#pragma once


namespace Truevision {

#pragma pack (push, n)		// Push the current alignment.
#pragma pack (1)			// Set new alignment.
	struct TGAHeader {

		unsigned char	idSize, mapType, imageType;

		unsigned short	paletteStart, paletteSize;
		unsigned char	paletteEntryDepth;

		unsigned short	xOrigin, yOrigin, width, height;
		unsigned char	colorDepth, descriptor;

	};
#pragma pack (pop, n)	// Restore alignment.


	char * ImportTGA (const char * fileName, int * width, int * height, int * imageChannels) {

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

		for (int x0 = 0; x0 < (* height); x0++) {
			for (int y0 = 0; y0 < (* width); y0++) {

				int y1 = y0;
				int x1 = x0;

				if (imageHeader.descriptor & (1 << 4)) {
					y1 = (* width) - 1 - y0;
				}
				if (!(imageHeader.descriptor & (1 << 5))) {
					x1 = (* height) - 1 - x0;
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

}
