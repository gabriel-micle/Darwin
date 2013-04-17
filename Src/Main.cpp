
#include "Headers.h"

// Handle to a program object.
GLuint programObject;

GLuint pVBO;
GLuint tVBO;
GLuint nVBO;
GLuint iVBO;
GLuint VAO;



VertexGroup	  * pVG;
MaterialGroup * pMG;


// Initialize the shader and program object.
void Init (ESContext * esContext) {

	// Display OpenGL version.
	const GLubyte * version = glGetString(GL_VERSION);
	printf("%s\n", version);

	char * vShaderStr = ReadFile("./Shaders/Hello.vert.glsl");
	char * fShaderStr = ReadFile("./Shaders/Hello.frag.glsl");

	// Create the program object.
	programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Read OBJ file.
	ReadWavefrontOBJ("./Data/Models/Woman1.obj", pVG, pMG);


	// Load vertex position data into buffer.
	glGenBuffers(1, &pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, pVG->nPositions * sizeof(Vector3), pVG->vPositions, GL_STATIC_DRAW);

	// Load vertex texCoord data into buffer.
	glGenBuffers(1, &tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, pVG->nTexCoords * sizeof(Vector2), pVG->vTexCoords, GL_STATIC_DRAW);

	// Load vertex texCoord data into buffer.
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	glBufferData(GL_ARRAY_BUFFER, pVG->nNormals * sizeof(Vector3), pVG->vNormals, GL_STATIC_DRAW);

	// Reset binding.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load index data into buffer.
	glGenBuffers(1, &iVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMG->nIndices * sizeof(int), pMG->vIndices, GL_STATIC_DRAW);

	// Reset binding.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Get position location.
	GLint loc;

	// Create vertex array.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	loc = glGetAttribLocation(programObject, "inPosition");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);

	
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	loc = glGetAttribLocation(programObject, "inTexCoord");
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);
	

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	loc = glGetAttribLocation(programObject, "inNormal");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBO);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw (ESContext * esContext) {

	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram(programObject);

	glBindVertexArray(VAO);
	glDrawElementsInstanced (GL_TRIANGLES, pMG->nIndices, GL_UNSIGNED_INT, 0, 2);

	esContext->esSwapBuffers();
}


int main (int argc, char * argv[]) {

	ESContext * esContext = new ESContext();

	esContext->esInitDisplayMode(ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_ALPHA);
	esContext->esInitWindowPosition(300, 300);
	esContext->esInitWindowSize(800, 480);

	esContext->esCreateWindow("Darwin");

	Init(esContext);

	esContext->esDisplayFunc(Draw);

	esContext->esMainLoop();
}
