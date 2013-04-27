
#ifndef _MODEL_H_
#define _MODEL_H_


#define INIT_ARRAY_SIZE	10000

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>

#include "ESUtil.h"

#include "Math/Math.h"

#include "Vertex.h"


class Model {

private:

	GLuint			vertexVBO;
	GLuint			vertexVAO;
	GLuint			indexVBO;

	void			computeTangentBitangent ();

public:

					Model ();			

	Matrix4			MV;
	Matrix4			MVP;

	// Vertices.
	int				cVertices;
	int				nVertices;
	Vertex *		vVertices;

	void			addVertex (Vertex & vert);

	// Indices.
	int				cIndices;
	int				nIndices;
	int *			vIndices;

	void			addIndex (int & idx);


	void			finalize ();

	void			draw (GLuint programObject);

	
	static void		computeTangentBitangent (Vertex &, Vertex &, Vertex &);

};


#endif
