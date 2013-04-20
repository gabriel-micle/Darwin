
#include "Headers.h"

// Handle to a program object.
GLuint programObject;

Mesh   * pMesh;
Camera * pCamera;

float tx = 0.0f;
float ty = -1.0f;
float tz = -2.0f;


bool keyPressed[6];

enum keyID_t {
	W,
	A,
	S,
	D,
	SPACE,
	C,
};

// Initialize the shader and program object.
void Init (ESContext * esContext) {

	// Display OpenGL version.
	printf("%s\n", glGetString(GL_VERSION));

	// Specify buffer clear color.
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Set up camera and frustum.
	pCamera = new Camera();
	pCamera->setPerspective(60.0f, 1.0f * esContext->width / esContext->height, 0.1f, 1000.0f);


	// Read GLSL shader files.
	char * vShaderStr = ReadFile("Data/Shaders/Hello.vert.glsl");
	char * fShaderStr = ReadFile("Data/Shaders/Hello.frag.glsl");

	// Create the program object.
	programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Import mesh from Wavefront OBJ file.
	pMesh = Wavefront::ImportOBJ("Data/Models/Woman1.obj");

}


void DisplayFunc (ESContext * esContext) {

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Matrix4 M = Matrix4::identity();
	M[3][0] = tx; M[3][1] = ty; M[3][2] = tz;

	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);


	pMesh->MVP = M * pCamera->viewMatrix() * pCamera->projectionMatrix();
	pMesh->draw(programObject);

	esContext->esSwapBuffers();
}

void IdleFunc (ESContext * esContext, float t) {

	// Check key presses.
	if (keyPressed[W]) {
		pCamera->translateForward(0.05f);
	}
	if (keyPressed[S]) {
		pCamera->translateForward(-0.05f);
	}
	if (keyPressed[D]) {
		pCamera->translateRight(0.05f);
	}
	if (keyPressed[A]) {
		pCamera->translateRight(-0.05f);
	}
	if (keyPressed[SPACE]) {
		pCamera->translateUp(0.05f);
	}
	if (keyPressed[C]) {
		pCamera->translateUp(-0.05f);
	}
}

void KeyboardFunc (ESContext * esContext, unsigned char c, int x, int y) {

	switch (c) {
	case 'W': case 'w':
		keyPressed[W] = true;
		break;
	case 'S': case 's':
		keyPressed[S] = true;
		break;
	case 'D': case 'd':
		keyPressed[D] = true;
		break;
	case 'A': case 'a':
		keyPressed[A] = true;
		break;
	case ' ':
		keyPressed[SPACE] = true;
		break;
	case 'C': case 'c':
		keyPressed[C] = true;
		break;
	default:
		break;
	}

}

void KeyboardUpFunc (ESContext * esContext, unsigned char c, int x, int y) {
	
	switch (c) {
	case 'W': case 'w':
		keyPressed[W] = false;
		break;
	case 'S': case 's':
		keyPressed[S] = false;
		break;
	case 'D': case 'd':
		keyPressed[D] = false;
		break;
	case 'A': case 'a':
		keyPressed[A] = false;
		break;
	case ' ':
		keyPressed[SPACE] = false;
		break;
	case 'C': case 'c':
		keyPressed[C] = false;
		break;
	default:
		break;
	}
}

void MouseFunc (ESContext * esContext, int button, int state, int x, int y) {

}


GLint prevX = 0;
GLint prevY = 0;
void MotionFunc (ESContext * esContext, int x, int y) {

	float dX = (x - prevX) * M_PI / 180.0f;
	float dY = (y - prevY) * M_PI / 180.0f;

	pCamera->rotateUp(dY);
	pCamera->rotateRight(dX);

	prevX = x;
	prevY = y;
}

void PassiveMotionFunc (ESContext * esContext, int x, int y) {

	prevX = x;
	prevY = y;
}

int main (int argc, char * argv[]) {

	ESContext * esContext = new ESContext();

	esContext->esInitDisplayMode(ES_RGB | ES_ALPHA | ES_DEPTH | ES_MULTISAMPLE | ES_SAMPLES_16);
	esContext->esInitWindowPosition(300, 300);
	esContext->esInitWindowSize(800, 480);

	esContext->esCreateWindow("Darwin");

	Init(esContext);

	esContext->esDisplayFunc(DisplayFunc);
	esContext->esIdleFunc(IdleFunc);
	esContext->esKeyboardFunc(KeyboardFunc);
	esContext->esKeyboardUpFunc(KeyboardUpFunc);
	esContext->esMouseFunc(MouseFunc);
	esContext->esMotionFunc(MotionFunc);
	esContext->esPassiveMotionFunc(PassiveMotionFunc);

	esContext->esMainLoop();

	return 0;
}
