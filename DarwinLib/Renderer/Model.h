
#pragma once


#define INIT_ARRAY_SIZE	10000

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>

#include "Device/ESDevice.h"

#include "Math/Math.h"

#include "Vertex.h"

#include <vector>


class Model {

public:


	// Vertices.
	int					m_vertexOffset;
	int					m_vertexCount;

	int					m_indexOffset;
	int					m_indexCount;

						Model () {};
	virtual				~Model () {};

	static void			ComputeTangentBitangent (Vertex *, Vertex *, Vertex *);

	static void			EnableVertexArrays(GLuint programObject);
	static void			DisableVertexArrays(GLuint programObject);
	static void			VertexArraysPointer(GLuint programObject, GLint offset);

};
