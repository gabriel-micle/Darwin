
#include "Model.h"


void Mesh::draw (GLuint programObject) {

	GLuint loc;

	// Create vertex data buffer if it was not created.
	if (!glIsBuffer(vertexVBO)) {

		glGenBuffers(1, &vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, pVG->nVertices * sizeof(Tuple<Vector3>), pVG->vVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Create index buffer of it was not created.
	if (!glIsBuffer(indexVBO)) {

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMG->nIndices * sizeof(int), pMG->vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Initialize vertex array.
	if (!glIsVertexArray(vertexVAO)) {

		glGenVertexArrays(1, &vertexVAO);
		glBindVertexArray(vertexVAO);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

		loc = glGetAttribLocation(programObject, "inPosition");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, x));
		}

		loc = glGetAttribLocation(programObject, "inTexCoord");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, y));
		}

		loc = glGetAttribLocation(programObject, "inNormal");
		if (loc != -1) {
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, z));
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

	loc = glGetAttribLocation(programObject, "inNormal");
	if (loc != -1) {
		glEnableVertexAttribArray(loc);
	}


	// Bind program.
	glUseProgram(programObject);


	// Set uniforms.
	loc = glGetUniformLocation(programObject, "ModelViewProjectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
	

	// Draw indexed.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glDrawElementsInstanced(GL_TRIANGLES, pMG->nIndices, GL_UNSIGNED_INT, 0, 1);


	// Disable vertex arrays.
	loc = glGetAttribLocation(programObject, "inPosition");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTexCoord");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inNormal");
	if (loc != -1) {
		glDisableVertexAttribArray(loc);
	}

	// Restore vertex array binding.
	glBindVertexArray(0);

	// Restore program binding.
	glUseProgram(0);
}
