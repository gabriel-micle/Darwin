
#include "Model.h"

// Create a new material group with default name.
MaterialGroup::MaterialGroup () {

	memcpy(materialName, "default", 7);

	cIndices = 3 * INIT_ARRAY_SIZE;
	nIndices = 0;

	vIndices[0] = NULL;
	vIndices[1] = NULL;
	vIndices[2] = NULL;

}

MaterialGroup::MaterialGroup (const char * name) {

	memcpy(materialName, name, sizeof(name));

	cIndices = 3 * INIT_ARRAY_SIZE;
	nIndices = 0;

	vIndices[0] = NULL;
	vIndices[1] = NULL;
	vIndices[2] = NULL;

}

void MaterialGroup::addIndexPTN (int & ip, int & it, int & in) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices[0] = (int *) malloc(cIndices * sizeof(int));
		vIndices[1] = (int *) malloc(cIndices * sizeof(int));
		vIndices[2] = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices[0] = (int *) realloc(vIndices[0], cIndices * sizeof(int));
		vIndices[1] = (int *) realloc(vIndices[1], cIndices * sizeof(int));
		vIndices[2] = (int *) realloc(vIndices[2], cIndices * sizeof(int));
	}

	vIndices[0][nIndices] = ip - 1;
	vIndices[1][nIndices] = it - 1;
	vIndices[2][nIndices] = in - 1;

	nIndices++;
}

void MaterialGroup::addIndexPT (int & ip, int & it) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices[0] = (int *) malloc(cIndices * sizeof(int));
		vIndices[1] = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices[0] = (int *) realloc(vIndices[0], cIndices * sizeof(int));
		vIndices[1] = (int *) realloc(vIndices[1], cIndices * sizeof(int));
	}

	vIndices[0][nIndices] = ip - 1;
	vIndices[1][nIndices] = it - 1;

	nIndices++;
}

void MaterialGroup::addIndexP (int & ip) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices[0] = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices[0] = (int *) realloc(vIndices[0], cIndices * sizeof(int));
	}

	vIndices[0][nIndices] = ip - 1;

	nIndices++;
}

void MaterialGroup::addIndexPN (int & ip, int & in) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices[0] = (int *) malloc(cIndices * sizeof(int));
		vIndices[2] = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices[0] = (int *) realloc(vIndices[0], cIndices * sizeof(int));
		vIndices[2] = (int *) realloc(vIndices[2], cIndices * sizeof(int));
	}

	vIndices[0][nIndices] = ip - 1;
	vIndices[2][nIndices] = in - 1;

	nIndices++;
}

void MaterialGroup::finalize () {

	for (int i = 0; i < 3; i++) {
		if (!vIndices[i]) {
			vIndices[i] = (int *) realloc(vIndices[i], nIndices * sizeof(int));
		}
	}

}