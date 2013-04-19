
#include "Model.h"

// Create a new vertex group.
VertexGroup::VertexGroup () {

	cVertices = INIT_ARRAY_SIZE;
	nVertices = 0;
	vVertices = NULL;
}

// Push back a position.
void VertexGroup::addVertex (Tuple<Vector3> & vertex) {

	if (!vVertices) {

		// If this is the first element, initialize the array.
		vVertices = (Tuple<Vector3> *) malloc(cVertices * sizeof(Tuple<Vector3>));

#ifdef _DEBUG
		printf("Init vertices!\n");
#endif

	} else if (nVertices >= cVertices) {

		// If the capacity is exceeded, increase it.
		cVertices <<= 1;
		vVertices = (Tuple<Vector3> *) realloc(vVertices, cVertices * sizeof(Tuple<Vector3>));

#ifdef _DEBUG
		printf("Realloc vertices!\n");
#endif

	}

	vVertices[nVertices++] = vertex;
}

// Optimizes the storage memory.
void VertexGroup::finalize () {

	if (vVertices) {
		vVertices = (Tuple<Vector3> *) realloc(vVertices, nVertices * sizeof(Tuple<Vector3>));
	}

#ifdef _DEBUG
	printf("Finalize vertex group!\n");
#endif

}
