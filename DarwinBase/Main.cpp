
#include "DarwinLib.h"


// Handle to a program object.
GLuint programObject1;
GLuint programObject2;

GLuint msaaFBO;
GLuint msaaRBO[3];
GLuint depthRBO;

GLuint finalFBO;
GLuint finalTex[3];

GLenum status;

Model  * pModel1;
Model  * pModel2;
Model  * pQuad;
Camera * pCamera;

GLint loc;

float tx = 0.0f;
float ty = -1.0f;
float tz = -2.0f;


Texture * Tex1[2];
Texture * Tex2[2];

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
	DW_KEY_NONE = -1, 
	DW_KEY_W, 
	DW_KEY_A, 
	DW_KEY_S, 
	DW_KEY_D, 
	DW_KEY_SP, 
	DW_KEY_C, 
};

// Initialize the shader and program object.
void Init (ESContext * esContext) {

	// Specify buffer clear color.
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);


	// Set up camera and frustum.
	// --------------------------
	float aspect = 1.0f * esContext->m_width / esContext->m_height;
	pCamera = new Camera();
	pCamera->SetPerspective(60.0f, aspect, 0.1f, 1000.0f);
	pCamera->SetPosition(Vector3(0.0f));
	pCamera->SetAcceleration(Vector3(8.0f));
	pCamera->SetVelocity(Vector3(2.0f));



	// Init models.
	// ------------

	// Import mesh from Wavefront OBJ file.
	pModel1 = Wavefront::ImportOBJ("Data/Models/Woman1.obj");
	pModel2 = Wavefront::ImportOBJ("Data/Models/Woman2.obj");

	pQuad = Wavefront::ImportOBJ("Data/Models/Quad.obj");



	// Init textures.
	// --------------

	// Test texture.
	char * pixels;
	int width, height, ch;
	TextureOpts opts;
	GLuint mipmaps;

	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_color.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2(static_cast<float>(max(width, height)))) + 1;

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	Tex1[0] = new Texture("Data/Textures/sarah_color.tga");
	Tex1[0]->Generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_normal.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2(static_cast<float>(max(width, height)))) + 1;

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_BUMP;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	Tex1[1] = new Texture("Data/Textures/sarah_normal.tga");
	Tex1[1]->Generate2D(pixels, width, height, opts);



	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/betty_color.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2(static_cast<float>(max(width, height)))) + 1;

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	Tex2[0] = new Texture("Data/Textures/betty_color.tga");
	Tex2[0]->Generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/betty_normal.tga", &width, &height, &ch);
	mipmaps = (GLuint) ceil(log2(static_cast<float>(max(width, height)))) + 1;

	opts.format     = DW_RGB8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_BUMP;
	opts.mipmaps    = mipmaps;
	opts.compressed = false;

	Tex2[1] = new Texture("Data/Textures/betty_normal.tga");
	Tex2[1]->Generate2D(pixels, width, height, opts);




	char * vShaderStr;
	char * fShaderStr;



	// Parse, compile and link shader files for DEFERRED rendering.
	// ------------------------------------------------------------

	// Read GLSL shader file.
	vShaderStr = ReadFile("Data/Shaders/DeferredGeometryPass.vert.glsl");
	fShaderStr = ReadFile("Data/Shaders/DeferredGeometryPass.frag.glsl");

	// Create the program object.
	programObject1 = esLoadProgram(vShaderStr, fShaderStr);

	delete vShaderStr; delete fShaderStr;

	// Read GLSL shader file.
	vShaderStr = ReadFile("Data/Shaders/DeferredLightingPass.vert.glsl");
	fShaderStr = ReadFile("Data/Shaders/DeferredLightingPass.frag.glsl");

	// Create the program object.
	programObject2 = esLoadProgram(vShaderStr, fShaderStr);

	delete vShaderStr; delete fShaderStr;



	// Init MSAA'd framebuffer for geometry pass.
	// ------------------------------------------

	glGenFramebuffers(1, &msaaFBO);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFBO);

	glGenRenderbuffers(3, msaaRBO);


	for (unsigned int i = 0; i < 3; i++) {
		glBindRenderbuffer(GL_RENDERBUFFER, msaaRBO[i]);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA16F, esContext->m_width, esContext->m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, msaaRBO[i]);
	}

	glGenRenderbuffers(1, &depthRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, esContext->m_width, esContext->m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("Framebuffer undefined!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("Framebuffer incomplete attachment!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("Framebuffer incomplete/missing attachemtn!\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("Framebuffer unsupported!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf("Framebuffer incomplete multisample!\n");
		break;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);




	// Init final framebuffer from which we sample the screen space textures.
	// ----------------------------------------------------------------------

	glGenFramebuffers(1, &finalFBO);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);

	glGenTextures(3, finalTex);

	for (unsigned int i = 0; i < 3; i++) {
		glBindTexture(GL_TEXTURE_2D, finalTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, esContext->m_width, esContext->m_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, finalTex[i], 0);
	}

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("Framebuffer undefined!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("Framebuffer incomplete attachment!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("Framebuffer incomplete/missing attachemtn!\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("Framebuffer unsupported!\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf("Framebuffer incomplete multisample!\n");
		break;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void GeometryPass (ESContext * esContext, GLuint programObject) {

	// Enable.
	glUseProgram(programObject);

	// Bind.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFBO);

	GLenum buffers [] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
	};

	glDrawBuffers(3, buffers);

	// Only the geometry pass updates the depth buffer.
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	{
		const char * uniformNames [] = {
			"u_DiffuseMap",
			"u_NormalMap",
		};

		for (int i = 0; i < 2; i++) {
			loc = glGetUniformLocation(programObject, uniformNames[i]);
			if (loc != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, Tex1[i]->m_hTexture);
				glUniform1i(loc, i);
			}
		}


		loc = glGetUniformLocation(programObject, "u_Material.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialDiffuse);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialAmbient);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialSpecular);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Shininess");
		if (loc != -1) {
			glUniform1f(loc, MaterialShininess);
		}

		pModel1->Draw(programObject);
	}

	{
		const char * uniformNames [] = {
			"u_DiffuseMap",
			"u_NormalMap",
		};

		for (int i = 0; i < 2; i++) {
			loc = glGetUniformLocation(programObject, uniformNames[i]);
			if (loc != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, Tex2[i]->m_hTexture);
				glUniform1i(loc, i);
			}
		}


		loc = glGetUniformLocation(programObject, "u_Material.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialDiffuse);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialAmbient);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, MaterialSpecular);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Shininess");
		if (loc != -1) {
			glUniform1f(loc, MaterialShininess);
		}

		// Draw scene.
		pModel2->Draw(programObject);

	}





	glDepthMask(GL_FALSE);

	// Copy data from the multisampled renderbuffers to textures.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);

	for (unsigned int i = 0; i < 3; i++) {

		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glDrawBuffers(1, &buffers[i]);
		glBlitFramebuffer(
			0, 0, esContext->m_width, esContext->m_height, 
			0, 0, esContext->m_width, esContext->m_height,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
			);
	}

}

void LightingPass (GLuint programObject) {

	// Enable.
	glUseProgram(programObject);

	// Bind.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Disable depth test.
	glDisable(GL_DEPTH_TEST);

	{
		const char * uniformNames [] = {
			"u_DiffuseAndAlpha",
			"u_NormalAndSpecular",
			"u_PositionAndMaterialID",
		};

		for (int i = 0; i < 3; i++) {
			loc = glGetUniformLocation(programObject, uniformNames[i]);
			if (loc != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, finalTex[i]);
				glUniform1i(loc, i);
			}
		}

		loc = glGetUniformLocation(programObject, "u_GlobalAmbient");
		if (loc != -1) {
			glUniform4fv(loc, 1, GlobalAmbient);
		}

		loc = glGetUniformLocation(programObject, "u_EyePosition");
		if (loc != -1) {
			glUniform3fv(loc, 1, pCamera->GetEyePosition());
		}

		loc = glGetUniformLocation(programObject, "u_Light.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightAmbient);
		}

		loc = glGetUniformLocation(programObject, "u_Light.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightDiffuse);
		}

		loc = glGetUniformLocation(programObject, "u_Light.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightSpecular);
		}

		loc = glGetUniformLocation(programObject, "u_Light.Position");
		if (loc != -1) {
			glUniform4fv(loc, 1, LightPosition);
		}


	}

	// Draw screenspace QUAD.
	pQuad->Draw(programObject);

}

void DisplayFunc (ESContext * esContext) {

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set the viewport
	glViewport(0, 0, esContext->m_width, esContext->m_height);

	//ForwardLighting (programObject0);
	GeometryPass(esContext, programObject1);
	LightingPass(programObject2);

	// Swap buffers.
	esContext->SwapBuffers();
}

float yaw   = 0.0f;
float pitch = 0.0f;

void IdleFunc (ESContext * esContext, float t) {

	Vector3 velocity = pCamera->GetCurrentVelocity();
	Vector3 direction = Vector3(0.0f);

	if (keyPressed[DW_KEY_W]) {
		direction.z += 1.0f;
	}
	if (keyPressed[DW_KEY_S]) {
		direction.z -= 1.0f;
	}
	if (keyPressed[DW_KEY_D]) {
		direction.x += 1.0f;
	}
	if (keyPressed[DW_KEY_A]) {
		direction.x -= 1.0f;
	}
	if (keyPressed[DW_KEY_SP]) {
		direction.y += 1.0f;
	}
	if (keyPressed[DW_KEY_C]) {
		direction.y -= 1.0f;
	}

	
	pCamera->Rotate(yaw, pitch);
	float lambda = 100.0f;
	yaw   *= exp(-lambda * t);
	pitch *= exp(-lambda * t);
	pCamera->UpdatePosition(direction, t);
	

	Matrix4 M = Matrix4::IDENTITY;

	M[3][0] = 1; M[3][1] = ty; M[3][2] = tz;

	pModel1->m_ModelMatrix               = M;
	pModel1->m_ModelViewMatrix           = pModel1->m_ModelMatrix * pCamera->GetViewMatrix();
	pModel1->m_ModelViewProjectionMatrix = pModel1->m_ModelViewMatrix * pCamera->GetProjectionMatrix();

	M[3][0] = -1; M[3][1] = ty; M[3][2] = tz;

	pModel2->m_ModelMatrix               = M;
	pModel2->m_ModelViewMatrix           = pModel2->m_ModelMatrix * pCamera->GetViewMatrix();
	pModel2->m_ModelViewProjectionMatrix = pModel2->m_ModelViewMatrix * pCamera->GetProjectionMatrix();
}


void KeyboardEventFunc(ESContext * esContext, const KeyboardEvent & ev) {

	Vector3 velocity = pCamera->GetCurrentVelocity();

	keyID_t key = DW_KEY_NONE;

	switch (ev.keyCode) {
	case 'W': case 'w':
		key = DW_KEY_W;
		velocity.z = 0.0f;
		break;
	case 'S': case 's':
		key = DW_KEY_S;
		velocity.z = 0.0f;
		break;
	case 'D': case 'd':
		key = DW_KEY_D;
		velocity.x = 0.0f;
		break;
	case 'A': case 'a':
		key = DW_KEY_A;
		velocity.x = 0.0f;
		break;
	case ' ':
		key = DW_KEY_SP;
		velocity.y = 0.0f;
		break;
	case 'C': case 'c':
		key = DW_KEY_C;
		velocity.y = 0.0f;
		break;
	}



	if (ev.pressed) {
		if (!keyPressed[key]) {
			keyPressed[key] = true;
			pCamera->SetCurrentVelocity(velocity);
		}
	} else {
		keyPressed[key] = false;
	}

}


void MouseEventFunc (ESContext * esContext, const MouseEvent & ev) {

	static GLint prevX = 0;
	static GLint prevY = 0;

	if (ev.type == DW_MOUSE_MOVE && ev.left) {

		yaw   = (ev.x - prevX) * static_cast<float>(M_PI) / 180.0f;
		pitch = (ev.y - prevY) * static_cast<float>(M_PI) / 180.0f;
	}

	prevX = ev.x;
	prevY = ev.y;
}

int main (int argc, char * argv[]) {

	ESContext * esContext = new ESContext();

	esContext->InitDisplayMode(ES_RGB | ES_ALPHA | ES_DEPTH);

	esContext->InitDisplayPosition(100, 100);
	esContext->InitDisplaySize(1024, 600);

	esContext->CreateDisplay("Darwin");

	Init(esContext);

	esContext->DisplayFunc(DisplayFunc);
	esContext->IdleFunc(IdleFunc);

	esContext->MouseEventFunc(MouseEventFunc);
	esContext->KeyboardEventFunc(KeyboardEventFunc);

	esContext->MainLoop();

	return 0;
}
