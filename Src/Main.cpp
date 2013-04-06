
#include "Headers.h"

// Handle to a program object
GLuint programObject;

GLuint posVBO;
GLuint posVAO;

GLuint idxVBO;


VertexGroup	  * pVG;
MaterialGroup * pMG;

///
// Initialize the shader and program object
//
void Init (ESContext * esContext) {

	// Display OpenGL version.
	const GLubyte * version = glGetString(GL_VERSION);
	printf("%s\n", version);

	char * vShaderStr = ReadFile("./Shaders/Hello.vert.glsl");
	char * fShaderStr = ReadFile("./Shaders/Hello.frag.glsl");

	// Create the program object.
	programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Read OBJ file.
	ReadWavefrontOBJ("./Data/Models/test.obj", pVG, pMG);

	// Create vertex buffer.
	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &idxVBO);	// Buffer for indices.

	// Load vertex position data into buffer.
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, pVG->nPositions * sizeof(Vector3), pVG->vPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load index data into buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMG->nIndices * sizeof(int), pMG->vIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Get position location.
	GLuint loc = glGetAttribLocation(programObject, "inPosition");

	// Create vertex array.
	glGenVertexArrays(1, &posVAO);
	glBindVertexArray(posVAO);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw (ESContext * esContext) {

	// Set the viewport
	glViewport (0, 0, esContext->width, esContext->height);

	// Clear the color buffer
	glClear (GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram (programObject);

	glBindVertexArray(posVAO);
	glDrawElements (GL_TRIANGLES, pMG->nIndices, GL_UNSIGNED_INT, 0);

	eglSwapBuffers (esContext->eglDisplay, esContext->eglSurface);
}


int main (int argc, char * argv[]) {

	ESContext esContext;

	esInitContext (&esContext);

	esCreateWindow (&esContext, "Hello Triangle", 300, 300, 800, 600, ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_ALPHA);

	Init ( &esContext );

	esDisplayFunc (&esContext, Draw);

	esMainLoop (&esContext);
}
