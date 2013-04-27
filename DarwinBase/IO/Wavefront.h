
#ifndef _WAVEFRONT_H_
#define _WAVEFRONT_H_


#define WAVEFRONT_BUFSIZE	1024

#include <vector>
#include <map>

#include "..\Tuple.h"

class Wavefront {
public:
	static Model *	ImportOBJ (const char * fileName);
};


Model * Wavefront::ImportOBJ (const char * fileName) {

	FILE * pFile = fopen(fileName, "rb");
	if (pFile == NULL) {
		perror(fileName);
		return NULL;
	}

	char buffer[WAVEFRONT_BUFSIZE];

	std::vector<Vector3> vPositions;
	std::vector<Vector2> vTexCoords;

	std::vector<std::pair<int, int> > vIndices;



	Model * pModel = new Model();

	float x, y, z;
	int	p = 0;
	int t = 0;
	int n = 0;

	while (fgets(buffer, WAVEFRONT_BUFSIZE, pFile)) {

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
			// Ignore normals, as they will be generated.
			// sscanf(buffer + 2, "%f %f %f", &x, &y, &z);
			// vNormals.push_back(Vector3(x, y, z));

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

				// Save position and texCoord indices. Discard normal index.
				vIndices.push_back(std::make_pair(p, t));

				pch = end + 1;

				p = t = n = 0;
			}

		} // if.

	} // while.

	fclose(pFile);


	// Unindex positions and texture coordinates.
	Vertex * vVertices = (Vertex *) malloc(vIndices.size() * sizeof(Vertex));

	for (int i = 0; i < vIndices.size(); i += 3) {

		for (int k = i; k < i + 3; k++) {
			vVertices[k].Position = vPositions[vIndices[k].first  - 1];
			vVertices[k].TexCoord = vTexCoords[vIndices[k].second - 1];
		}

		Model::computeTangentBitangent(vVertices[i], vVertices[i + 1], vVertices[i + 2]);
	}

	
	// Create unified index for both position and texcoord.
	int currentIdx = 0;
	std::map<std::pair<int, int>, int> indexMap;

	for (int i = 0; i < vIndices.size(); i++) {

		// Search for vp/vt index pair.
		auto found = indexMap.find(vIndices[i]);

		// Extract vertex data for the index tuple.
		Vertex v = vVertices[i];

		if (found == indexMap.end()) {

			// If index tuple was NOT encountered before, create a new unified index in the index buffer.
			pModel->addIndex(currentIdx);

			// Map tuple to the new index.
			indexMap[vIndices[i]] = currentIdx++;

			// Add the vertex as a new mesh vertex.
			pModel->addVertex(v);

		} else {
			
			// If index tuple was encountered before, add its unified mapping to the index buffer.
			pModel->addIndex(found->second);

			// Also interpolate the tangent and bitangent.
			pModel->vVertices[found->second].Binormal += v.Binormal;
			pModel->vVertices[found->second].Tangent  += v.Tangent;
		}

	}


	// Cleanup.
	std::vector<Vector3>().swap(vPositions);
	std::vector<Vector2>().swap(vTexCoords);
	std::vector<std::pair<int, int> >().swap(vIndices);
	std::map<std::pair<int, int>, int>().swap(indexMap);
	free(vVertices);

	// Optimize storage.
	pModel->finalize();

	return pModel;
}

#endif
