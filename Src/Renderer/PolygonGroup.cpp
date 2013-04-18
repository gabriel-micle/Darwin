
#include "Model.h"

// Create a new material group with default name.
PolygonGroup::PolygonGroup () {

	memcpy(materialName, "default", 7);

	cIndices = 3 * INIT_ARRAY_SIZE;
	nIndices = 0;

	vIndices = NULL;

}

PolygonGroup::PolygonGroup (const char * name) {

	memcpy(materialName, name, sizeof(name));

	cIndices = 3 * INIT_ARRAY_SIZE;
	nIndices = 0;

	vIndices = NULL;

}

void PolygonGroup::addIndex (int & idx) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices = (int *) realloc(vIndices, cIndices * sizeof(int));
	}

	vIndices[nIndices] = idx;

	nIndices++;
}


void PolygonGroup::finalize () {

	if (!vIndices) {
		vIndices = (int *) realloc(vIndices, nIndices * sizeof(int));
	}

}