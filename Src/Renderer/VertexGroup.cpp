
#include "Model.h"

// Create a new vertex group.
VertexGroup::VertexGroup () {

	cPositions = INIT_ARRAY_SIZE;
	cTexCoords = INIT_ARRAY_SIZE;
	cNormals   = INIT_ARRAY_SIZE;

	nPositions = 0;
	nTexCoords = 0;
	nNormals   = 0;

	vPositions = NULL;
	vTexCoords = NULL;
	vNormals   = NULL;

}

// Push back a position.
void VertexGroup::addPosition (float & x, float & y, float & z) {

	if (!vPositions) {

		// If this is the first element, initialize the array.
		vPositions = (Vector3 *) malloc(cPositions * sizeof(Vector3));

#ifdef _DEBUG
		printf("Init positions!\n");
#endif

	} else if (nPositions >= cPositions) {

		// If the capacity is exceeded, increase it.
		cPositions <<= 1;
		vPositions = (Vector3 *) realloc(vPositions, cPositions * sizeof(Vector3));

#ifdef _DEBUG
		printf("Realloc positions!\n");
#endif

	}

	vPositions[nPositions++] = Vector3(x, y, z);
}

// Push back a texture coordinate.
void VertexGroup::addTexCoord (float & u, float & v) {

	if (!vTexCoords) {

		// If this is the first element, initialize the array.
		vTexCoords = (Vector2 *) malloc(cTexCoords * sizeof(Vector2));

#ifdef _DEBUG
		printf("Init texCoords!\n");
#endif

	} else if (nTexCoords >= cTexCoords) {

		// If the capacity is exceeded, increase it.
		cTexCoords <<= 1;
		vTexCoords = (Vector2 *) realloc(vTexCoords, cTexCoords * sizeof(Vector2));

#ifdef _DEBUG
		printf("Realloc texCoords!\n");
#endif

	}

	vTexCoords[nTexCoords++] = Vector2(u, v);
}

// Push back a normal.
void VertexGroup::addNormal (float & x, float & y, float & z) {

	if (!vNormals) {

		// If this is the first element, initialize the array.
		vNormals = (Vector3 *) malloc(cNormals * sizeof(Vector3));

#ifdef _DEBUG
		printf("Init normals!\n");
#endif

	} else if (nNormals >= cNormals) {

		// If the capacity is exceeded, increase it.
		cNormals <<= 1;
		vNormals = (Vector3 *) realloc(vNormals, cNormals * sizeof(Vector3));

#ifdef _DEBUG
		printf("Realloc normals!\n");
#endif

	}

	vNormals[nNormals++] = Vector3(x, y, z);
}

// Optimizes the storage memory.
void VertexGroup::finalize () {

	if (vPositions) {
		vPositions = (Vector3 *) realloc(vPositions, nPositions * sizeof(Vector3));
	}
	if (vTexCoords) {
		vTexCoords = (Vector2 *) realloc(vTexCoords, nTexCoords * sizeof(Vector2));
	}
	if (vNormals) {
		vNormals = (Vector3 *) realloc(vNormals, nNormals * sizeof(Vector3));
	}

#ifdef _DEBUG
	printf("Finalize vertex group!\n");
#endif

}
