
#include "Headers.h"

// Handle to a program object.
GLuint programObject;

Model  * pMesh;
Camera * pCamera;

GLint loc;

float tx = 0.0f;
float ty = -1.0f;
float tz = -2.0f;

float t = 0.0f;

Texture * DiffuseTex;
Texture * NormalTex;

Vector4 GlobalAmbient(0.2f, 0.2f, 0.2f, 1.0f);

Vector4 LightAmbient(0.2f, 0.2f, 0.2f, 1.0f);
Vector4 LightDiffuse(0.6f, 0.6f, 0.6f, 1.0f);
Vector4 LightSpecular(1.4f, 0.8f, 0.6f, 1.0f);
Vector4 LightPosition(0.0f, 5.0f, -0.2f, 1.0f);

Vector4 MaterialAmbient(0.3f, 0.3f, 0.3f, 1.0f);
Vector4 MaterialDiffuse(1.0f, 0.8f, 1.0f, 1.0f);
Vector4 MaterialSpecular(0.8f, 0.8f, 0.8f, 1.0f);
float	MaterialShininess(2.0f);

bool keyPressed[6];

enum keyID_t {
	W, A, S, D, SPACE, C,
};

// Initialize the shader and program object.
void Init (ESContext * esContext) {

	// Specify buffer clear color.
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// Test texture.
	char * pixels;
	int width, height, ch;
	TextureOpts opts;

	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_color.tga", &width, &height, &ch);

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = 10;
	opts.compressed = false;

	DiffuseTex = new Texture("");
	DiffuseTex->generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_normal.tga", &width, &height, &ch);

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_BUMP;
	opts.mipmaps    = 10;
	opts.compressed = false;

	NormalTex = new Texture("");
	NormalTex->generate2D(pixels, width, height, opts);

}


void DisplayFunc (ESContext * esContext) {

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);

	// Bind a texture.
	glUseProgram(programObject);

	{
		loc = glGetUniformLocation(programObject, "uColorMap");
		if (loc != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, DiffuseTex->handle);
			glUniform1i(loc, 0);
		}

		loc = glGetUniformLocation(programObject, "uNormalMap");
		if (loc != -1) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, NormalTex->handle);
			glUniform1i(loc, 1);
		}

		loc = glGetUniformLocation(programObject, "uGlobalAmbient");
		if (loc != -1) {
			glUniform4fv(loc, 1, GlobalAmbient);
		}

		loc = glGetUniformLocation(programObject, "uLight.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightAmbient);
		}

		loc = glGetUniformLocation(programObject, "uLight.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightDiffuse);
		}

		loc = glGetUniformLocation(programObject, "uLight.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightSpecular);
		}

		loc = glGetUniformLocation(programObject, "uLight.Position");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightPosition);
		}

		loc = glGetUniformLocation(programObject, "uMaterial.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialDiffuse);
		}

		loc = glGetUniformLocation(programObject, "uMaterial.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialAmbient);
		}

		loc = glGetUniformLocation(programObject, "uMaterial.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialSpecular);
		}

		loc = glGetUniformLocation(programObject, "uMaterial.Shininess");
		if (loc != -1) {
			glUniform1f(loc, MaterialShininess);
		}
	}

	

	Matrix4 M = Matrix4::identity();
	M[3][0] = tx; M[3][1] = ty; M[3][2] = tz;

	// Set a model view projection matrix.
	pMesh->MV = M * pCamera->viewMatrix();
	pMesh->MVP = pMesh->MV * pCamera->projectionMatrix();

	// Draw.
	pMesh->draw(programObject);

	// Swap buffers.
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

	t += 0.1f;
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

	esContext->esInitDisplayMode(
		ES_RGB | 
		ES_ALPHA | 
		ES_DEPTH | 
		ES_MULTISAMPLE | 
		ES_SAMPLES_16
		);

	esContext->esInitWindowPosition(100, 100);
	esContext->esInitWindowSize(1024, 600);

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
