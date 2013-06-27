
#include "Model.h"

Model::Model () :

	m_cVertices					(INIT_ARRAY_SIZE),
	m_nVertices					(0),
	m_vVertices					(NULL),

	m_cIndices					(INIT_ARRAY_SIZE * 3),
	m_nIndices					(0),
	m_vIndices					(NULL),

	m_ModelMatrix				(Matrix4::IDENTITY),
	m_ModelViewMatrix			(Matrix4::IDENTITY),
	m_ModelViewProjectionMatrix	(Matrix4::IDENTITY)

{}

Model::~Model () {

	delete m_vVertices;
	m_vVertices = NULL;

	delete m_vIndices;
	m_vIndices = NULL;
}

// Push back an index.
void Model::AddIndex (int & idx) {

	if (m_nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		m_vIndices = (int *) malloc(m_cIndices * sizeof(int));

	} else if (m_nIndices >= m_cIndices) {

		// If the capacity is exceeded, increase it.
		m_cIndices <<= 1;
		m_vIndices = (int *) realloc(m_vIndices, m_cIndices * sizeof(int));
	}

	m_vIndices[m_nIndices++] = idx;
}


// Push back a vertex.
void Model::AddVertex (Vertex & vertex) {

	if (m_nVertices == 0) {

		// If this is the first element, initialize the array.
		m_vVertices = (Vertex *) malloc(m_cVertices * sizeof(Vertex));

	} else if (m_nVertices >= m_cVertices) {

		// If the capacity is exceeded, increase it.
		m_cVertices <<= 1;
		m_vVertices = (Vertex *) realloc(m_vVertices, m_cVertices * sizeof(Vertex));
	}

	m_vVertices[m_nVertices++] = vertex;
}


void Model::Finalize () {

	if (m_vIndices) {
		m_vIndices = (int *) realloc(m_vIndices, m_nIndices * sizeof(int));
	}

	if (m_vVertices) {
		m_vVertices = (Vertex *) realloc(m_vVertices, m_nVertices * sizeof(Vertex));
	}
}


void Model::ComputeTangentBitangent (Vertex * v0, Vertex * v1, Vertex * v2) {

	Vector3 deltaPos1 = v1->Position - v0->Position;
	Vector3 deltaPos2 = v2->Position - v0->Position;

	Vector2 deltaUV1  = v1->TexCoord - v0->TexCoord;
	Vector2 deltaUV2  = v2->TexCoord - v0->TexCoord;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	Vector3 tangent  = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	Vector3 binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

	if (r < 0) {
		tangent = -tangent;
	}

	// Update the attributes.
	v0->Tangent  = tangent;
	v0->Binormal = binormal;

	v1->Tangent  = tangent;
	v1->Binormal = binormal;

	v2->Tangent  = tangent;
	v2->Binormal = binormal;
}



void Model::Draw (GLuint programObject) {

	// Create vertex data buffer if it was not created.
	if (!glIsBuffer(m_vertexVBO)) {

		glGenBuffers(1, &m_vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(Vertex), m_vVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Create index buffer of it was not created.
	if (!glIsBuffer(m_indexVBO)) {

		glGenBuffers(1, &m_indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(int), m_vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Initialize vertex array.
	if (!glIsVertexArray(m_vertexVAO)) {

		glGenVertexArrays(1, &m_vertexVAO);

		glBindVertexArray(m_vertexVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

		VertexArraysPointer(programObject, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	// Bind vertex array.
	glBindVertexArray(m_vertexVAO);

	// Enable arrays.
	EnableVertexArrays(programObject);

	// Enable element array buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
	// Draw indexed.
	glDrawElementsInstanced(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, 0, 1);
	// Disable element array buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Disable arrays.
	DisableVertexArrays(programObject);

	// Restore vertex array binding.
	glBindVertexArray(0);

}


void Model::EnableVertexArrays (GLuint programObject) {

	GLint loc;

	// Enable vertex arrays.
	loc = glGetAttribLocation(programObject, "in_Position");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_TexCoord");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_Binormal");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_Tangent");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}
}

void Model::DisableVertexArrays(GLuint programObject) {

	GLint loc;

	// Disable vertex arrays.
	loc = glGetAttribLocation(programObject, "in_Position");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_TexCoord");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_Binormal");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "in_Tangent");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}
}


void Model::VertexArraysPointer (GLuint programObject, int offset) {

	GLint loc;

	loc = glGetAttribLocation(programObject, "in_Position");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			reinterpret_cast<void *>(offsetof(Vertex, Position) + offset * sizeof(Vertex)));
	}

	loc = glGetAttribLocation(programObject, "in_TexCoord");
	if (loc != -1) {
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			reinterpret_cast<void *>(offsetof(Vertex, TexCoord) + offset * sizeof(Vertex)));
	}

	loc = glGetAttribLocation(programObject, "in_Binormal");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			reinterpret_cast<void *>(offsetof(Vertex, Binormal) + offset * sizeof(Vertex)));
	}

	loc = glGetAttribLocation(programObject, "in_Tangent");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			reinterpret_cast<void *>(offsetof(Vertex, Tangent) + offset * sizeof(Vertex)));
	}
}