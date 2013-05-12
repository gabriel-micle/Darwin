
#pragma once


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

	GLuint			m_vertexVBO;
	GLuint			m_vertexVAO;
	GLuint			m_indexVBO;

	void			ComputeTangentBitangent ();

public:

					Model ();			

	Matrix4			m_ModelView;
	Matrix4			m_ModelViewProjection;

	// Vertices.
	int				m_cVertices;
	int				m_nVertices;
	Vertex *		m_vVertices;

	void			AddVertex (Vertex & vert);

	// Indices.
	int				m_cIndices;
	int				m_nIndices;
	int *			m_vIndices;

	void			AddIndex (int & idx);


	void			Finalize ();

	void			Draw (GLuint programObject);

	
	static void		ComputeTangentBitangent (Vertex &, Vertex &, Vertex &);

};
