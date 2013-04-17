
#ifndef _WAVEFRONT_H_
#define _WAVEFRONT_H_


#define MAX_BUFFER_SIZE	1024

#include <vector>
#include <map>

class tuple {
public:
	int x;
	int y;
	int z;

	tuple(int _x, int  _y, int _z) : x(_x), y(_y), z(_z) {}
	bool operator < (const tuple & t) const {
		if (x == t.x) {
			if (y == t.y) {
				return z < t.z;
			} else {
				return y < t.y;
			}
		} else {
			return x < t.x;
		}
	}
	bool operator == (const tuple & t) const {
		return x == t.x && y == t.y && z == t.z;
	}
};


void ReadWavefrontOBJ (const char * fileName, VertexGroup *& pVG, MaterialGroup *& pMG) {

	FILE * pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		perror(fileName);
		return;
	}

	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);

	std::vector<Vector3> vPositions;
	std::vector<Vector2> vTexCoords;
	std::vector<Vector3> vNormals;

	std::map<tuple, int> indexMap;
	int currentIdx = 0;

	pVG	= new VertexGroup();
	pMG = new MaterialGroup();

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
			vTexCoords.push_back(Vector2(x, y));

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

				std::map<tuple, int>::iterator found;
				found = indexMap.find(tuple(p, t, n));
				if (found == indexMap.end()) {
					pMG->addIndex(currentIdx);
					indexMap[tuple(p, t, n)] = currentIdx++;
					if (p != 0) {
						pVG->addPosition(
							vPositions[p - 1].x, 
							vPositions[p - 1].y, 
							vPositions[p - 1].z);
					}
					if (t != 0) {
						pVG->addTexCoord(
							vTexCoords[t - 1].x,
							vTexCoords[t - 1].y);
					}
					if (n != 0) {
						pVG->addNormal(
							vNormals[n - 1].x,
							vNormals[n - 1].y,
							vNormals[n - 1].z);
					}
				} else {
					pMG->addIndex(found->second);
				}

				pch = end + 1;

				p = t = n = 0;
			}
			
		} // if.

		memset(buffer, 0, MAX_BUFFER_SIZE);

	} // while.

	fclose(pFile);

	std::vector<Vector3>().swap(vPositions);
	std::vector<Vector2>().swap(vTexCoords);
	std::vector<Vector3>().swap(vNormals);

	std::map<tuple, int>().swap(indexMap);

	pVG->finalize();
	pMG->finalize();
	
}

#endif
