
#ifndef _VERTEX_GROUP_H_
#define _VERTEX_GROUP_H_


#include "..\Tuple.h"

// Container for vertex attributes (positions, texCoords and normals).
class VertexGroup {

public:

						VertexGroup ();

	int					cVertices;
	int					nVertices;
	Tuple<Vector3> *	vVertices;

	void				addVertex (Tuple<Vector3> &);
	void				finalize ();

};

#endif
