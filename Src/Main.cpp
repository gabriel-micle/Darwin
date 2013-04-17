
#include "Headers.h"

// Handle to a program object.
GLuint programObject;

GLuint VBO;
GLuint iVBO;
GLuint VAO;

Mesh   * pMesh;
Camera * pCamera;

GLint loc;

float tx = 0.0f;
float ty = -1.0f;
float tz = -2.0f;

// Initialize the shader and program object.
void Init (ESContext * esContext) {

	// Display OpenGL version.
	const GLubyte * version = glGetString(GL_VERSION);
	printf("%s\n", version);


	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);


	// Set up camera.
	pCamera = new Camera();
	pCamera->setFrustum(-0.1f, 0.1f, -0.057735f, 0.057735f, 0.1f, 1000.0f);


	char * vShaderStr = ReadFile("./Shaders/Hello.vert.glsl");
	char * fShaderStr = ReadFile("./Shaders/Hello.frag.glsl");

	// Create the program object.
	programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Read OBJ file.
	pMesh = ReadWavefrontOBJ("./Data/Models/Woman1.obj");



	// Load vertex data into buffer.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pMesh->pVG->nVertices * sizeof(Tuple<Vector3>), pMesh->pVG->vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load index data into buffer.
	glGenBuffers(1, &iVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->pMG->nIndices * sizeof(int), pMesh->pMG->vIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Create vertex array.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	loc = glGetAttribLocation(programObject, "inPosition");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, x));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inTexCoord");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, y));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(programObject, "inNormal");
	if (loc != -1) {
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Tuple<Vector3>), (void *) offsetof(Tuple<Vector3>, z));
		glEnableVertexAttribArray(loc);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBO);

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

	Matrix4 M = Matrix4::identity();
	M[3][0] = tx; M[3][1] = ty; M[3][2] = tz;

	M = M * pCamera->viewMatrix();
	M = M * pCamera->projectionMatrix();

	loc = glGetUniformLocation(programObject, "ModelViewProjectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, M);


	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, pMesh->pMG->nIndices, GL_UNSIGNED_INT, 0, 2);

	esContext->esSwapBuffers();
}

void KeyboardFunc (ESContext * esContext, unsigned char c, int x, int y) {

	switch (c) {
	case 'W': case 'w':
		pCamera->translateForward(0.1f);
		//tz += 0.1f;
		break;
	case 'S': case 's':
		pCamera->translateForward(-0.1f);
		//tz -= 0.1f;
		break;
	case 'D': case 'd':
		pCamera->translateRight(0.1f);
		//tx += 0.1f;
		break;
	case 'A': case 'a':
		pCamera->translateRight(-0.1f);
		//tx -= 0.1f;
		break;
	case ' ':
		pCamera->translateUp(0.1f);
		//ty += 0.1f;
		break;
	case 'C': case 'c':
		pCamera->translateUp(-0.1f);
		//ty -= 0.1f;
		break;
	default:
		break;
	}

}

void KeyboardUpFunc (ESContext * esContext, unsigned char c, int x, int y) {

}

int main (int argc, char * argv[]) {

	ESContext * esContext = new ESContext();

	esContext->esInitDisplayMode(ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_ALPHA);
	esContext->esInitWindowPosition(300, 300);
	esContext->esInitWindowSize(800, 480);

	esContext->esCreateWindow("Darwin");

	Init(esContext);

	esContext->esDisplayFunc(Draw);
	esContext->esKeyboardFunc(KeyboardFunc);
	esContext->esKeyboardUpFunc(KeyboardUpFunc);

	esContext->esMainLoop();
}
