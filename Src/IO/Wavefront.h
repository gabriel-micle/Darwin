
#ifndef _WAVEFRONT_H_
#define _WAVEFRONT_H_


#define MAX_BUFFER_SIZE	1024


void ReadWavefrontOBJ (const char * fileName, VertexGroup *& pVG, MaterialGroup *& pMG) {

	FILE * pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		perror(fileName);
		return;
	}

	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);


	pVG = new VertexGroup();
	pMG = new MaterialGroup();

	float	x, y, z;
	int		p, t, n;

	while (fgets(buffer, MAX_BUFFER_SIZE, pFile)) {

		/* Possible keywords are:
		- # for comments
		- mtllib for material library filename
		- o for object name
		- g for vertex group
		- v for positions
		- vt for texture coordinates
		- vn for normals
		- usemtl for material name from material library
		- f for face indices
		*/

		if (strncmp(buffer, "#", 2) == 0) {
			// Discard comment.

		} else if (strncmp (buffer, "mtllib", 6) == 0) {
			// TODO

		} else if (strncmp (buffer, "o", 1) == 0) {
			// TODO

		} else if (strncmp (buffer, "g", 1) == 0) {
			// TODO

		} else if (strncmp (buffer, "v ", 2) == 0) {

			// Positions.
			sscanf(buffer + 2, "%f %f %f", &x, &y, &z);

			pVG->addPosition(x, y, z);

		} else if (strncmp (buffer, "vt", 2) == 0) {

			// Texture coordinates.
			sscanf(buffer + 3, "%f %f", &x, &y);
			pVG->addTexCoord(x, y);

		} else if (strncmp (buffer, "vn", 2) == 0) {

			// Normals.
			sscanf(buffer + 3, "%f %f %f", &x, &y, &z);
			pVG->addNormal(x, y, z);

		} else if (strncmp (buffer, "usemtl", 6) == 0) {
			//TODO

		} else if (strncmp (buffer, "f", 1) == 0) {


			// Indices.
			char * pch = buffer + 2;

			char indexType = 0;

			for (int i = 0 ; i < 3; i++) {

				char * end = strchr(pch, ' ');

				// Position index.
				sscanf(pch, "%d", &p);
				indexType |= 1;

				pch = (char *) memchr(pch + 1, '/', end - pch - 1);

				if (pch) {

					if (pch[1] != '/') {

						// Texture coordinate index.
						sscanf(pch + 1, "%d", &t);
						indexType |= 2;
					}

					pch = (char *) memchr(pch + 1, '/', end - pch - 1);

					if (pch) {

						// Normal index.
						sscanf(pch + 1, "%d", &n);
						indexType |= 4;
					}

				}

				switch (indexType) {
				case 1:
					pMG->addIndexP(p);
					break;
				case 3:
					pMG->addIndexPT(p, t);
					break;
				case 5:
					pMG->addIndexPN(p, n);
					break;
				case 7:
					pMG->addIndexPTN(p, t, n);
					break;
				}

				pch = end + 1;

			}
			
		} // if.

		memset(buffer, 0, MAX_BUFFER_SIZE);

	} // while.

	fclose(pFile);

	pVG->finalize();
	pMG->finalize();
	
}

#endif
