
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
	float aspect = 1.0f * esContext->m_width / esContext->m_height;
	pCamera->SetPerspective(60.0f, aspect, 0.1f, 1000.0f);


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
	GLuint mipmaps;

	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_color.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2((float) max(width, height))) + 1;
	
	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	DiffuseTex = new Texture("");
	DiffuseTex->Generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_normal.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2((float) max(width, height))) + 1;

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_BUMP;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	NormalTex = new Texture("");
	NormalTex->Generate2D(pixels, width, height, opts);

}


void DisplayFunc (ESContext * esContext) {

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set the viewport
	glViewport(0, 0, esContext->m_width, esContext->m_height);

	// Bind a texture.
	glUseProgram(programObject);

	{
		loc = glGetUniformLocation(programObject, "uColorMap");
		if (loc != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, DiffuseTex->m_hTexture);
			glUniform1i(loc, 0);
		}

		loc = glGetUniformLocation(programObject, "uNormalMap");
		if (loc != -1) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, NormalTex->m_hTexture);
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



	Matrix4 M = Matrix4::Identity();
	M[3][0] = tx; M[3][1] = ty; M[3][2] = tz;

	// Set a model view projection matrix.
	pMesh->m_ModelView = M * pCamera->ViewMatrix();
	pMesh->m_ModelViewProjection = pMesh->m_ModelView * pCamera->ProjectionMatrix();

	// Draw.
	pMesh->Draw(programObject);

	// Swap buffers.
	esContext->SwapBuffers();
}

void IdleFunc (ESContext * esContext, float t) {

	// Check key presses.
	if (keyPressed[W]) {
		pCamera->TranslateForward(0.05f);
	}
	if (keyPressed[S]) {
		pCamera->TranslateForward(-0.05f);
	}
	if (keyPressed[D]) {
		pCamera->TranslateRight(0.05f);
	}
	if (keyPressed[A]) {
		pCamera->TranslateRight(-0.05f);
	}
	if (keyPressed[SPACE]) {
		pCamera->TranslateUp(0.05f);
	}
	if (keyPressed[C]) {
		pCamera->TranslateUp(-0.05f);
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

	float dX = (x - prevX) * float(M_PI) / 180.0f;
	float dY = (y - prevY) * float(M_PI) / 180.0f;

	pCamera->RotateUp(dY);
	pCamera->RotateRight(dX);

	prevX = x;
	prevY = y;
}

void PassiveMotionFunc (ESContext * esContext, int x, int y) {

	prevX = x;
	prevY = y;
}

int main (int argc, char * argv[]) {

	ESContext * esContext = new ESContext();

	esContext->InitDisplayMode(
		ES_RGB | 
		ES_ALPHA | 
		ES_DEPTH | 
		ES_MULTISAMPLE | 
		ES_SAMPLES_16
		);

	esContext->InitDisplayPosition(100, 100);
	esContext->InitDisplaySize(1024, 600);

	esContext->CreateDisplay("Darwin");

	Init(esContext);

	esContext->DisplayFunc(DisplayFunc);
	esContext->IdleFunc(IdleFunc);

	esContext->KeyboardFunc(KeyboardFunc);
	esContext->KeyboardUpFunc(KeyboardUpFunc);

	esContext->MouseFunc(MouseFunc);
	esContext->MotionFunc(MotionFunc);
	esContext->PassiveMotionFunc(PassiveMotionFunc);

	esContext->MainLoop();

	return 0;
}
