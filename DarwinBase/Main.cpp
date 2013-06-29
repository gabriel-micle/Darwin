
#include "DarwinLib.h"


// Handle to a program object.
GLuint programObject1;
GLuint programObject2;

GLuint msaaFBO;
GLuint msaaRBO[3];
GLuint depthRBO;

GLuint lightUBO;

GLuint finalFBO;
GLuint finalTex[3];

GLenum status;

Model  * pModel1;
Model  * pModel2;
Model  * pQuad;
//Camera * pCamera;

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


enum keyID_t { 
	DW_KEY_NONE = -1, 
	DW_KEY_W, 
	DW_KEY_A, 
	DW_KEY_S, 
	DW_KEY_D, 
	DW_KEY_SP, 
	DW_KEY_C,
	DW_KEY_ARROW_UP,
	DW_KEY_ARROW_DOWN,
	DW_KEY_ARROW_LEFT,
	DW_KEY_ARROW_RIGHT,

	DW_NUM_KEYS,
};


bool keyPressed[DW_NUM_KEYS];



GLuint CreateLightParamsUniformBuffer (GLuint programObject) {

	GLuint blockIndex = glGetUniformBlockIndex(programObject, "LightParams");

	GLint blockSize;
	glGetActiveUniformBlockiv(programObject, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	GLubyte * blockBuffer = (GLubyte *) malloc(blockSize);
	memcpy(blockBuffer, LightAmbient, sizeof(Vector4));
	memcpy(blockBuffer + sizeof(Vector4), LightDiffuse,  sizeof(Vector4));
	memcpy(blockBuffer + sizeof(Vector4) * 2, LightSpecular, sizeof(Vector4));
	memcpy(blockBuffer + sizeof(Vector4) * 3, LightPosition, sizeof(Vector4));

	GLuint lightUBO;
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// glBufferSubData should be called if any parameters get modified!

	return lightUBO;
}



// Initialize the shader and program object.
void Init () {

	// Specify buffer clear color.
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);


	SceneManager::CreateInstance("Data/Scene/");
	SceneManager::GetInstance()->ImportScene("test_scene.xml");


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


	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_color.tga", &width, &height, &ch);

	opts.format     = (ch == 3) ? DW_RGB8 : DW_RGBA8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = 4;

	Tex1[0] = new Texture("Data/Textures/sarah_color.tga");
	Tex1[0]->Generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/sarah_normal.tga", &width, &height, &ch);

	opts.format     = (ch == 3) ? DW_RGB8 : DW_RGBA8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_NORMAL;
	opts.mipmaps    = 4;

	Tex1[1] = new Texture("Data/Textures/sarah_normal.tga");
	Tex1[1]->Generate2D(pixels, width, height, opts);



	// Diffuse texture.
	pixels = Truevision::ImportTGA("Data/Textures/betty_color32.tga", &width, &height, &ch);

	opts.format     = (ch == 3) ? DW_RGB8 : DW_RGBA8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_DIFFUSE;
	opts.mipmaps    = 4;

	Tex2[0] = new Texture("Data/Textures/betty_color.tga");
	Tex2[0]->Generate2D(pixels, width, height, opts);


	// Normal map.
	pixels = Truevision::ImportTGA("Data/Textures/betty_normal.tga", &width, &height, &ch);

	opts.format     = (ch == 3) ? DW_RGB8 : DW_RGBA8;
	opts.filter     = DW_TRILINEAR;
	opts.wrap       = DW_REPEAT;
	opts.usage      = DW_NORMAL;
	opts.mipmaps    = 4;

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
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, 
			4, 
			GL_RGBA16F, 
			ESDevice::GetInstance()->GetWindowWidth(), 
			ESDevice::GetInstance()->GetWindowHeight()
			);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, msaaRBO[i]);
	}

	glGenRenderbuffers(1, &depthRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorageMultisample(
		GL_RENDERBUFFER, 
		4, 
		GL_DEPTH_COMPONENT16, 
		ESDevice::GetInstance()->GetWindowWidth(), 
		ESDevice::GetInstance()->GetWindowHeight());
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ESDevice::GetInstance()->GetWindowWidth(), ESDevice::GetInstance()->GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
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


void DrawCombinedMeshes (Model * pM1, Model * pM2, GLuint programObject) {

	GLuint loc = -1;
	GLuint vertexVBO = -1;
	GLuint vertexVAO = -1;
	GLuint indexVBO = -1;

	int nVertices = pM1->m_nVertices + pM2->m_nVertices;
	Vertex * vVertices = (Vertex *) malloc(nVertices * sizeof(Vertex));
	memcpy(vVertices, pM1->m_vVertices, pM1->m_nVertices * sizeof(Vertex));
	memcpy(vVertices + pM1->m_nVertices, pM2->m_vVertices, pM2->m_nVertices * sizeof(Vertex));

	int nIndices = pM1->m_nIndices + pM2->m_nIndices;
	int * vIndices = (int *) malloc(nIndices * sizeof(int));
	memcpy(vIndices, pM1->m_vIndices, pM1->m_nIndices * sizeof(int));
	memcpy(vIndices + pM1->m_nIndices, pM2->m_vIndices, pM2->m_nIndices * sizeof(int));


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

	}

	// 1. Bind vertex array.
	glBindVertexArray(vertexVAO);

	// 2. Enable element array buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

	// 3. Enable arrays.
	Model::EnableVertexArrays(programObject);

	// 4. Bind vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);


	// !! Object 1 !!
	{
		const char * uniformNames [] = {
			"u_DiffuseMap",
			"u_NormalMap",
		};

		for (int i = 0; i < 2; i++) {
			loc = glGetUniformLocation(programObject, uniformNames[i]);
			if (loc != -1) {
				glUniform1i(loc, i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, Tex1[i]->m_hTexture);
				//glBindSampler(i, Tex1[i]->m_sampler);
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

		// Set matrix uniforms.
		loc = glGetUniformLocation(programObject, "u_ModelViewProjectionMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel1->m_ModelViewProjectionMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelViewMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel1->m_ModelViewMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel1->m_ModelMatrix);
		}

	}
	// Set vertex array pointer.
	Model::VertexArraysPointer(programObject, 0);
	// Draw indexed.
	glDrawElementsInstanced(GL_TRIANGLES, pM1->m_nIndices, GL_UNSIGNED_INT, 
		0, 1);



	// !! Object 2 !!
	{
		const char * uniformNames [] = {
			"u_DiffuseMap",
			"u_NormalMap",
		};

		for (int i = 0; i < 2; i++) {
			loc = glGetUniformLocation(programObject, uniformNames[i]);
			if (loc != -1) {
				glUniform1i(loc, i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, Tex2[i]->m_hTexture);
				//glBindSampler(i, Tex2[i]->m_sampler);
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

		// Set matrix uniforms.
		loc = glGetUniformLocation(programObject, "u_ModelViewProjectionMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel2->m_ModelViewProjectionMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelViewMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel2->m_ModelViewMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pModel2->m_ModelMatrix);
		}

	}
	// Set vertex array pointer.
	Model::VertexArraysPointer(programObject, pM1->m_nVertices);
	// Draw indexed.
	glDrawElementsInstanced(GL_TRIANGLES, pM2->m_nIndices, GL_UNSIGNED_INT, 
		reinterpret_cast<void *>(pM1->m_nIndices * sizeof(int)), 1);


	// 4. Reset vertex array buffer binding.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 3. Disable arrays.
	Model::DisableVertexArrays(programObject);

	// 2. Disable element array buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 1. Restore vertex array binding.
	glBindVertexArray(0);

}

void GeometryPass (GLuint programObject) {

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

	DrawCombinedMeshes(pModel1, pModel2, programObject);

	glDepthMask(GL_FALSE);

	// Copy data from the multisampled renderbuffers to textures.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);

	for (unsigned int i = 0; i < 3; i++) {

		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glDrawBuffers(1, &buffers[i]);
		glBlitFramebuffer(
			0, 0, 
			ESDevice::GetInstance()->GetWindowWidth(), 
			ESDevice::GetInstance()->GetWindowHeight(), 
			0, 0, 
			ESDevice::GetInstance()->GetWindowWidth(), 
			ESDevice::GetInstance()->GetWindowHeight(),
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
			glUniform3fv(loc, 1, SceneManager::GetInstance()->GetActiveCamera()->GetEyePosition());
		}


		if (!glIsBuffer(lightUBO)) {
			lightUBO = CreateLightParamsUniformBuffer(programObject);
		}

		glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4) * 3, sizeof(Vector4), LightPosition);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		GLuint blockIndex = glGetUniformBlockIndex(programObject, "LightParams");
		glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, lightUBO);

	}

	// Draw screenspace QUAD.
	pQuad->Draw(programObject);

}

void DrawFunc () {

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the viewport
	glViewport(0, 0, ESDevice::GetInstance()->GetWindowWidth(), ESDevice::GetInstance()->GetWindowHeight());

	//ForwardLighting (programObject0);
	GeometryPass(programObject1);
	LightingPass(programObject2);

	// Swap buffers.
	ESDevice::GetInstance()->SwapBuffers();
}

float yaw   = 0.0f;
float pitch = 0.0f;

void UpdateFunc (float t) {

	Camera * pCamera = SceneManager::GetInstance()->GetActiveCamera();

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

	if (keyPressed[DW_KEY_ARROW_UP]) {
		LightPosition.z -= 1.0f;
	}
	if (keyPressed[DW_KEY_ARROW_DOWN]) {
		LightPosition.z += 1.0f;
	}
	if (keyPressed[DW_KEY_ARROW_LEFT]) {
		LightPosition.x -= 1.0f;
	}
	if (keyPressed[DW_KEY_ARROW_RIGHT]) {
		LightPosition.x += 1.0f;
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


void KeyboardEventFunc(const KeyboardEvent & ev) {

	Camera * pCamera = SceneManager::GetInstance()->GetActiveCamera();

	Vector3 velocity = pCamera->GetCurrentVelocity();

	keyID_t key = DW_KEY_NONE;

	switch (ev.keyCode) {
	case KEY_ESCAPE:
		// TODO: Cleanup!
		exit(EXIT_SUCCESS);
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
	case KEY_UP:
		key = DW_KEY_ARROW_UP;
		break;
	case KEY_DOWN:
		key = DW_KEY_ARROW_DOWN;
		break;
	case KEY_LEFT:
		key = DW_KEY_ARROW_LEFT;
		break;
	case KEY_RIGHT:
		key = DW_KEY_ARROW_RIGHT;
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


void MouseEventFunc (const MouseEvent & ev) {

	static GLint prevX = 0;
	static GLint prevY = 0;

	if (ev.type == DW_MOUSE_MOVE && ev.left) {

		yaw   = radians(float(ev.x - prevX));
		pitch = radians(float(ev.y - prevY));
	}

	prevX = ev.x;
	prevY = ev.y;
}

int main (int argc, char * argv[]) {

	ESDevice::CreateInstance();

	ESContextParams escp;
	escp.esVersion       = 3;
	escp.windowWidth     = 1024;
	escp.windowHeight    = 600;
	escp.windowPositionX = 100;
	escp.windowPositionY = 100;
	escp.redSize         = 8;
	escp.greenSize       = 8;
	escp.blueSize        = 8;
	escp.alphaSize       = 0;
	escp.depthSize       = 24;
	escp.stencilSize     = 0;

	ESDevice::GetInstance()->CreateDisplay("Darwin", escp);

	Init();

	ESDevice::GetInstance()->SetDrawFunc(DrawFunc);
	ESDevice::GetInstance()->SetUpdateFunc(UpdateFunc);

	ESDevice::GetInstance()->SetMouseEventFunc(MouseEventFunc);
	ESDevice::GetInstance()->SetKeyboardEventFunc(KeyboardEventFunc);

	ESDevice::GetInstance()->Run();

	ESDevice::DestroyInstance();

	return EXIT_SUCCESS;
}
