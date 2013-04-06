
#ifndef _VERTEX_GROUP_H_
#define _VERTEX_GROUP_H_

// Container for vertex attributes (positions, texCoords and normals).
class VertexGroup {

public:

				VertexGroup ();

	int			cPositions;
	int			nPositions;
	Vector3 *	vPositions;

	int			cTexCoords;
	int			nTexCoords;
	Vector2 *	vTexCoords;

	int			cNormals;
	int			nNormals;
	Vector3 *	vNormals;

	void		addPosition (float & x, float & y, float & z);
	void		addTexCoord (float & u, float & v);
	void		addNormal	(float & x, float & y, float & z);
	void		finalize	();

};

#endif
