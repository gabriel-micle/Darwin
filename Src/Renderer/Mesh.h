
#ifndef _MESH_H_
#define _MESH_H_

#include "esUtil.h"
#include "..\Math\Math.h"

#include <vector>

class VertexGroup {

public:

				VertexGroup ();

	int			np;
	Vector3	*	Positions;
	
	int			nt;
	Vector2 *	TexCoords;

	int			nn;
	Vector3 *	Normals;

	void		optimizeSpace ();

	void		print ();

};


class PolygonGroup {

public:
	
				PolygonGroup ();

	char		groupName[255];
	char		materialName[255];

	int			ni;
	int *		Indices[3];

	void		optimizeSpace ();

	void		print();

};


#endif
