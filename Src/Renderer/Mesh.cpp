
#include "Mesh.h"

VertexGroup::VertexGroup () {

	np = nt = nn = 0;
	Positions = NULL;
	TexCoords = NULL;
	Normals   = NULL;
}

void VertexGroup::optimizeSpace () {

	if (Positions != NULL) {
		Positions = (Vector3 *) realloc(Positions, np * sizeof(Vector3));
	}
	if (TexCoords != NULL) {
		TexCoords = (Vector2 *) realloc(TexCoords, nt * sizeof(Vector2));
	}
	if (Normals != NULL) {
		Normals = (Vector3 *) realloc(Normals, nn * sizeof(Vector3));
	}
}

void VertexGroup::print () {

	if (Positions != NULL) {
		printf("%d Positions:\n", nt);
		for (int i = 0; i < np; i++) {
			printf("%f %f %f\n", Positions[i].x, Positions[i].y, Positions[i].z);
		}
	}
	if (TexCoords != NULL) {
		printf("%d TexCoords:\n", nt);
		for (int i = 0; i < nt; i++) {
			printf("%f %f\n", TexCoords[i].x, TexCoords[i].y);
		}
	}
	if (Normals != NULL) {
		printf("%d Normals:\n", nn);
		for (int i = 0; i < nn; i++) {
			printf("%f %f %f\n", Normals[i].x, Normals[i].y, Normals[i].z);
		}
	}
}

PolygonGroup::PolygonGroup () {

	memset(groupName,    0, sizeof(groupName));
	memset(materialName, 0, sizeof(materialName));

	ni = 0;
	Indices[0] = NULL;
	Indices[1] = NULL;
	Indices[2] = NULL;
}

void PolygonGroup::optimizeSpace () {

	for (int i = 0; i < 3; i++) {
		if (Indices[i] != NULL) {
			Indices[i] = (int *) realloc(Indices[i], ni * sizeof(int));
		}
	}
}
