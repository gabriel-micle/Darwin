
#ifndef _WAVEFRONT_H_
#define _WAVEFRONT_H_


#define MAX_BUFFER_SIZE	1024
#define INIT_ARRAY_SIZE	1500

#include "..\Renderer\Mesh.h"

#include <vector>

void ReadWavefrontOBJ (const char * fileName) {

	FILE * pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		perror(fileName);
		return;
	}

	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);


	VertexGroup * pVG = new VertexGroup();
	std::vector<PolygonGroup *> groups;

	int cp = INIT_ARRAY_SIZE;		// Position vector capacity.
	int ct = INIT_ARRAY_SIZE;		// TexCoord vector capacity.
	int cn = INIT_ARRAY_SIZE;		// Normals vector capacity.
	int ci = INIT_ARRAY_SIZE * 10;	// Indices vector capacity. 



	float x, y, z;
	PolygonGroup * pPG = NULL;

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

			// If a group was previously defined, save it.
			if (pPG != NULL) {

				pPG->optimizeSpace();
				groups.push_back(pPG);
				printf("Push back group %s!\n", pPG->groupName);
			}

			// Create a new polygon group.
			ci = INIT_ARRAY_SIZE * 3;
			pPG = new PolygonGroup();
			sscanf(buffer + 2, "%s", pPG->groupName);
			printf("Create new group %s!\n", pPG->groupName);

		} else if (strncmp (buffer, "v ", 2) == 0) {

			// Positions.
			sscanf(buffer + 2, "%f %f %f", &x, &y, &z);

			if (!pVG->Positions) {

				pVG->Positions = (Vector3 *) malloc(cp * sizeof(Vector3));
				printf("Init positions!\n");

			} else if (pVG->np >= cp) {

				cp <<= 1;
				pVG->Positions = (Vector3 *) realloc(pVG->Positions, cp * sizeof(Vector3));
				printf("Realloc positions!\n");
			}

			pVG->Positions[pVG->np++] = Vector3(x, y, z);

		} else if (strncmp (buffer, "vt", 2) == 0) {

			// Texture coordinates.
			sscanf(buffer + 3, "%f %f", &x, &y);

			if (!pVG->TexCoords) {

				pVG->TexCoords = (Vector2 *) malloc(ct * sizeof(Vector2));
				printf("Init texCoords!\n");

			} else if (pVG->nt >= ct) {

				ct <<= 1;
				pVG->TexCoords = (Vector2 *) realloc(pVG->TexCoords, ct * sizeof(Vector2));
				printf("Realloc texCoords!\n");
			}

			pVG->TexCoords[pVG->nt++] = Vector2(x, y);

		} else if (strncmp (buffer, "vn", 2) == 0) {

			// Normals.
			sscanf(buffer + 3, "%f %f %f", &x, &y, &z);

			if (!pVG->Normals) {

				pVG->Normals = (Vector3 *) malloc(cn * sizeof(Vector3));
				printf("Init normals!\n");

			} else if (pVG->nn >= cn) {

				cn <<= 1;
				pVG->Normals = (Vector3 *) realloc(pVG->Normals, cn * sizeof(Vector3));
				printf("Realloc normals!\n");
			}

			pVG->Normals[pVG->nn++] = Vector3(x, y, z);

		} else if (strncmp (buffer, "usemtl", 6) == 0) {

			//sscanf(buffer + 7, "%s", pPG->materialName);
			//printf("Set material %s!\n", pPG->materialName);

		} else if (strncmp (buffer, "f", 1) == 0) {

			// If a group was not previously defined, create one.
			if (!pPG) {

				pPG = new PolygonGroup();
				printf("Create implicit group\n");
			}

			// Indices.
			if (pPG->ni + 3 >= ci) {

				ci <<= 1;

				if (pPG->Indices[0]) {

					pPG->Indices[0] = (int *) realloc(pPG->Indices[0], ci * sizeof(int));
					printf("Realloc indices (position)!\n");
				}

				if (pPG->Indices[1]) {

					pPG->Indices[1] = (int *) realloc(pPG->Indices[1], ci * sizeof(int));
					printf("Realloc indices (texCoords)!\n");
				}

				if (pPG->Indices[2]) {

					pPG->Indices[2] = (int *) realloc(pPG->Indices[2], ci * sizeof(int));
					printf("Realloc indices (normals)!\n");
				}
			}

			char * pch = buffer + 2;

			for (int i = 0 ; i < 3; i++) {

				char * end = strchr(pch, ' ');

				// Position index.
				if (!pPG->Indices[0]) {
					pPG->Indices[0] = (int *) malloc(ci * sizeof(int));
					printf("Init indices (position)!\n");
				}

				sscanf(pch, "%d", &pPG->Indices[0][pPG->ni]);

				pch = (char *) memchr(pch + 1, '/', end - pch - 1);

				if (pch) {

					if (pch[1] != '/') {

						// Texture coordinate index.
						if (!pPG->Indices[1]) {
							pPG->Indices[1] = (int *) malloc(ci * sizeof(int));
							printf("Init indices (texCoord)!\n");
						}

						sscanf(pch + 1, "%d", &pPG->Indices[1][pPG->ni]);
					}

					pch = (char *) memchr(pch + 1, '/', end - pch - 1);

					if (pch) {

						// Normal index.
						if (!pPG->Indices[2]) {
							pPG->Indices[2]	= (int *) malloc(ci * sizeof(int));
							printf("Init indices (normal)!\n");
						}

						sscanf(pch + 1, "%d", &pPG->Indices[2][pPG->ni]);
					}
				}

				pch = end + 1;

				pPG->ni++;
			}

		} // if.

		memset(buffer, 0, MAX_BUFFER_SIZE);

	} // while.

	fclose(pFile);

	pPG->optimizeSpace();
	groups.push_back(pPG);
	printf("Push back group %s!\n", pPG->groupName);

	pVG->optimizeSpace();

	for (int i = 0; i < groups.size(); i++) {
		printf("%d indices!\n", groups.at(i)->ni);
	}
}

#endif
