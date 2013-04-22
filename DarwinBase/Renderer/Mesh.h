
#ifndef _MESH_H_
#define _MESH_H_

class Mesh {

private:

	GLuint			vertexVBO;
	GLuint			vertexVAO;
	GLuint			indexVBO;

public:

	VertexGroup  *	pVG;
	PolygonGroup *	pMG;

	Matrix4			MVP;

	void			draw (GLuint programObject);

};

#endif
