
#ifndef _MESH_H_
#define _MESH_H_

class Mesh {

public:

	VertexGroup   *	pVG;
	MaterialGroup *	pMG;

	GLuint			vertexVBO;
	GLuint			vertexVAO;
	GLuint			indexVBO;

	Matrix4			MVP;

	void			draw (GLuint programObject);

};

#endif
