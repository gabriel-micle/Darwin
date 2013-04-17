
#ifndef _WAVEFRONT_H_
#define _WAVEFRONT_H_


#define MAX_BUFFER_SIZE	1024

#include <vector>
#include <map>

#include "..\Tuple.h"


Mesh * ReadWavefrontOBJ (const char * fileName) {

	FILE * pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		perror(fileName);
		return NULL;
	}

	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);

	std::vector<Vector3> vPositions;
	std::vector<Vector3> vTexCoords;
	std::vector<Vector3> vNormals;

	std::map<Tuple<int>, int> indexMap;
	int currentIdx = 0;


	Mesh * pMesh = new Mesh();
	pMesh->pVG = new VertexGroup();
	pMesh->pMG = new MaterialGroup();

	float x, y, z;
	int	p = 0;
	int t = 0;
	int n = 0;

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
		- s for smoothing groups
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
			vPositions.push_back(Vector3(x, y, z));

		} else if (strncmp (buffer, "vt", 2) == 0) {

			// Texture coordinates.
			sscanf(buffer + 3, "%f %f", &x, &y);
			vTexCoords.push_back(Vector3(x, y, 0.0f));

		} else if (strncmp (buffer, "vn", 2) == 0) {

			// Normals.
			sscanf(buffer + 3, "%f %f %f", &x, &y, &z);
			vNormals.push_back(Vector3(x, y, z));

		} else if (strncmp (buffer, "usemtl", 6) == 0) {
			//TODO

		} else if (strncmp (buffer, "f", 1) == 0) {


			// Indices.
			char * pch = buffer + 2;

			for (int i = 0 ; i < 3; i++) {

				char * end = strchr(pch, ' ');

				// Position index.
				sscanf(pch, "%d", &p);

				pch = (char *) memchr(pch + 1, '/', end - pch - 1);

				if (pch) {

					if (pch[1] != '/') {

						// Texture coordinate index.
						sscanf(pch + 1, "%d", &t);
					}

					pch = (char *) memchr(pch + 1, '/', end - pch - 1);

					if (pch) {

						// Normal index.
						sscanf(pch + 1, "%d", &n);
					}

				}

				auto found = indexMap.find(Tuple<int>(p, t, n));
				if (found == indexMap.end()) {

					pMesh->pMG->addIndex(currentIdx);
					indexMap[Tuple<int>(p, t, n)] = currentIdx++;

					Tuple<Vector3> ptn;

					if (p != 0) {
						ptn.x = vPositions[p - 1];
					}
					if (t != 0) {
						ptn.y = vTexCoords[t - 1];
					}
					if (n != 0) {
						ptn.z = vNormals[n - 1];
					}

					pMesh->pVG->addVertex(ptn);

				} else {
					pMesh->pMG->addIndex(found->second);
				}

				pch = end + 1;

				p = t = n = 0;
			}
			
		} // if.

		memset(buffer, 0, MAX_BUFFER_SIZE);

	} // while.

	fclose(pFile);

	std::vector<Vector3>().swap(vPositions);
	std::vector<Vector3>().swap(vTexCoords);
	std::vector<Vector3>().swap(vNormals);

	std::map<Tuple<int>, int>().swap(indexMap);

	pMesh->pVG->finalize();
	pMesh->pMG->finalize();
	

	return pMesh;
}

#endif
