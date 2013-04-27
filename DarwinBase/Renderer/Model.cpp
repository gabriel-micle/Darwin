
#include "Model.h"

Model::Model () {

	cVertices = INIT_ARRAY_SIZE;
	nVertices = 0;
	vVertices = NULL;

	cIndices = INIT_ARRAY_SIZE * 3;
	nIndices = 0;
	vIndices = NULL;
}

// Push back an index.
void Model::addIndex (int & idx) {

	if (nIndices == 0) {

		// If these are the first elements, initialize the arrays.
		vIndices = (int *) malloc(cIndices * sizeof(int));

	} else if (nIndices >= cIndices) {

		// If the capacity is exceeded, increase it.
		cIndices <<= 1;
		vIndices = (int *) realloc(vIndices, cIndices * sizeof(int));
	}

	vIndices[nIndices++] = idx;
}


// Push back a vertex.
void Model::addVertex (Vertex & vertex) {

	if (nVertices == 0) {

		// If this is the first element, initialize the array.
		vVertices = (Vertex *) malloc(cVertices * sizeof(Vertex));

	} else if (nVertices >= cVertices) {

		// If the capacity is exceeded, increase it.
		cVertices <<= 1;
		vVertices = (Vertex *) realloc(vVertices, cVertices * sizeof(Vertex));
	}

	vVertices[nVertices++] = vertex;
}


void Model::finalize () {

	if (!vIndices) {
		vIndices = (int *) realloc(vIndices, nIndices * sizeof(int));
	}

	if (vVertices) {
		vVertices = (Vertex *) realloc(vVertices, nVertices * sizeof(Vertex));
	}
}


void Model::computeTangentBitangent (Vertex & v0, Vertex & v1, Vertex & v2) {

	Vector3 deltaPos1 = v1.Position - v0.Position;
	Vector3 deltaPos2 = v2.Position - v0.Position;

	Vector2 deltaUV1  = v1.TexCoord - v0.TexCoord;
	Vector2 deltaUV2  = v2.TexCoord - v0.TexCoord;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	Vector3 tangent  = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	Vector3 binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

	if (r < 0) {
		tangent = -tangent;
	}

	// Update the attributes.
	v0.Tangent  = tangent;
	v0.Binormal = binormal;

	v1.Tangent  = tangent;
	v1.Binormal = binormal;

	v2.Tangent  = tangent;
	v2.Binormal = binormal;
}



void Model::draw (GLuint programObject) {

	GLuint loc;

	// Create vertex data buffer if it was not created.
	if (!glIsBuffer(vertexVBO)) {

		glGenBuffers(1, &vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(Vertex), vVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Create index buffer of it was not created.
	if (!glIsBuffer(indexVBO)) {

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(int), vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Initialize vertex array.
	if (!glIsVertexArray(vertexVAO)) {

		glGenVertexArrays(1, &vertexVAO);
		glBindVertexArray(vertexVAO);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

		loc = glGetAttribLocation(programObject, "inPosition");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Position));
		}

		loc = glGetAttribLocation(programObject, "inTexCoord");
		if (loc != -1) {
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoord));
		}

		loc = glGetAttribLocation(programObject, "inBinormal");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Binormal));
		}

		loc = glGetAttribLocation(programObject, "inTangent");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent));
		}


		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}


	// Enable vertex arrays.
	glBindVertexArray(vertexVAO);
	loc = glGetAttribLocation(programObject, "inPosition");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTexCoord");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inBinormal");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTangent");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}


	// Set uniforms (program must be bound before).
	loc = glGetUniformLocation(programObject, "ModelViewProjectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);

	loc = glGetUniformLocation(programObject, "ModelViewMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, MV);

	// Draw indexed.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glDrawElementsInstanced(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0, 1);


	// Disable vertex arrays.
	// Enable vertex arrays.
	glBindVertexArray(vertexVAO);
	loc = glGetAttribLocation(programObject, "inPosition");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTexCoord");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inBinormal");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTangent");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	// Restore vertex array binding.
	glBindVertexArray(0);

	// Restore program binding.
	glUseProgram(0);
}
