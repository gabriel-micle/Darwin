
#include "Model.h"


void Model::ComputeTangentBitangent (Vertex * v0, Vertex * v1, Vertex * v2) {

	Vector3 deltaPos1 = v1->Position - v0->Position;
	Vector3 deltaPos2 = v2->Position - v0->Position;

	Vector2 deltaUV1  = v1->TexCoord - v0->TexCoord;
	Vector2 deltaUV2  = v2->TexCoord - v0->TexCoord;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	Vector3 tangent  = ((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r).Normalize();
	Vector3 binormal = ((deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r).Normalize();

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



void Model::EnableVertexArrays(GLuint programObject) {

	GLint loc;

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



void Model::VertexArraysPointer(GLuint programObject, GLint offset) {

	GLint loc;

	GLint vertexSize   = sizeof(Vertex);
	GLint vertexOffset = offset * vertexSize;

	loc = glGetAttribLocation(programObject, "in_Position");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void *>( offsetof(Vertex, Position) + vertexOffset ));
	}

	loc = glGetAttribLocation(programObject, "in_TexCoord");
	if (loc != -1) {
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void *>( offsetof(Vertex, TexCoord) + vertexOffset ));
	}

	loc = glGetAttribLocation(programObject, "in_Binormal");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void *>( offsetof(Vertex, Binormal) + vertexOffset ));
	}

	loc = glGetAttribLocation(programObject, "in_Tangent");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void *>( offsetof(Vertex, Tangent) + vertexOffset ));
	}

}
