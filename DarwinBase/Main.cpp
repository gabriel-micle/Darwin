
#include "DarwinLib.h"

#include "IO/IO.h"


// Handle to a program object.
GLuint programObject1;
GLuint programObject2;

GLuint msaaFBO;
GLuint msaaRBO[4];
GLuint depthRBO;

GLuint lightUBO;

GLuint finalFBO;
GLuint finalTex[4];

GLenum status;


const int samples = 4;


Vector4 GlobalAmbient(0.02f, 0.01f, 0.02f, 1.0f);

Vector4 LightPosition(0.0f, 1.5f, 0.0f, 1.0f);


enum keyID_t { 
	DW_KEY_NOT_ASSIGNED = -1, 
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
	DW_PLUS,
	DW_MINUS,

	DW_NUM_KEYS,
};

bool keyPressed[DW_NUM_KEYS];



GLuint CreateLightParamsUniformBuffer (GLuint programObject) {

	GLuint lightUBO = -1;

	GLuint index = glGetUniformBlockIndex(programObject, "Lights");
	GLint  loc   = glGetUniformLocation(programObject, "u_NumLights");

	if (index != GL_INVALID_INDEX && loc != -1) {

		SceneManager * pSceneManager = SceneManager::GetInstance();

		int numLights = pSceneManager->m_sceneLights.size();
		numLights = (numLights > 8) ? 8 : numLights;

		glUniform1i(loc, numLights);

		glGenBuffers(1, &lightUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
		glBufferData(GL_UNIFORM_BUFFER, numLights * sizeof(Light), &pSceneManager->m_sceneLights[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	return lightUBO;
}



// Initialize the shader and program object.
void Init () {

	Clock::InitGameTime();

	// Specify buffer clear color.
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	ResourceManager::CreateInstance("Data/Models/", "Data/Textures/");
	SceneManager::CreateInstance("Data/Scene/");

	ESDevice        * pDevice          = ESDevice::GetInstance();
	ResourceManager * pResourceManager = ResourceManager::GetInstance();
	SceneManager    * pSceneManager    = SceneManager::GetInstance();

	pSceneManager->ImportScene("test_scene.xml");


	/**************************************************************************
	* Parse, compile and link shader files for DEFERRED rendering.			  *
	**************************************************************************/

	char * vShaderStr;
	char * fShaderStr;

	// Read GLSL shader file.
	vShaderStr = FileReader::ReadFile("Data/Shaders/DeferredGeometryPass.vert.glsl");
	fShaderStr = FileReader::ReadFile("Data/Shaders/DeferredGeometryPass.frag.glsl");

	// Create the program object.
	programObject1 = esLoadProgram(vShaderStr, fShaderStr);

	delete vShaderStr; 
	delete fShaderStr;

	// Read GLSL shader file.
	vShaderStr = FileReader::ReadFile("Data/Shaders/DeferredLightingPass.vert.glsl");
	fShaderStr = FileReader::ReadFile("Data/Shaders/DeferredLightingPass.frag.glsl");

	// Create the program object.
	programObject2 = esLoadProgram(vShaderStr, fShaderStr);

	delete vShaderStr; 
	delete fShaderStr;



	/**************************************************************************
	* Init MSAA'd framebuffer for geometry pass.							  *
	**************************************************************************/

	glGenFramebuffers(1, &msaaFBO);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFBO);

	glGenRenderbuffers(4, msaaRBO);

	for (unsigned int i = 0; i < 4; i++) {
		glBindRenderbuffer(GL_RENDERBUFFER, msaaRBO[i]);
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, 
			samples, 
			GL_RGBA32F, 
			ESDevice::GetInstance()->GetWindowWidth(), 
			ESDevice::GetInstance()->GetWindowHeight()
			);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, msaaRBO[i]);
	}


	glGenRenderbuffers(1, &depthRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorageMultisample(
		GL_RENDERBUFFER, 
		samples, 
		GL_DEPTH_COMPONENT32F, 
		ESDevice::GetInstance()->GetWindowWidth(), 
		ESDevice::GetInstance()->GetWindowHeight());

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



	/**************************************************************************
	* Init final framebuffer from which we sample the screen space textures. *
	**************************************************************************/

	glGenFramebuffers(1, &finalFBO);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);

	glGenTextures(4, finalTex);

	for (unsigned int i = 0; i < 4; i++) {
		glBindTexture(GL_TEXTURE_2D, finalTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pDevice->GetWindowWidth(), pDevice->GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
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



void DrawCombinedMeshes (GLuint programObject) {

	GLuint vertexVBO = -1;
	GLuint vertexVAO = -1;
	GLuint indexVBO  = -1;

	ResourceManager * pResourceManager = ResourceManager::GetInstance();
	SceneManager * pSceneManager = SceneManager::GetInstance();
	Camera * pCamera = pSceneManager->GetActiveCamera();

	int nVertices = pResourceManager->m_VertexPool.size();
	int nIndices = pResourceManager->m_IndexPool.size();


	// Create vertex data buffer if it was not created.
	if (!glIsBuffer(vertexVBO)) {

		glGenBuffers(1, &vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(Vertex), &pResourceManager->m_VertexPool[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Create index buffer of it was not created.
	if (!glIsBuffer(indexVBO)) {

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(int), &pResourceManager->m_IndexPool[0], GL_STATIC_DRAW);
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

	int k = 0;
	for (auto it = pSceneManager->m_sceneObjects.begin(); it != pSceneManager->m_sceneObjects.end(); ++it) {

		SceneObject * pObject = it->second;

		// Set textures.
		for (auto jt = pObject->m_textureNames.begin(); jt != pObject->m_textureNames.end(); ++jt) {

			Texture tex = pResourceManager->GetTextureByName(jt->c_str());


			GLuint loc = -1;
			switch (tex.m_opts.usage) {
			case DW_DIFFUSE:
				loc = glGetUniformLocation(programObject, "u_DiffuseMap");
				break;
			case DW_NORMAL:
				loc = glGetUniformLocation(programObject, "u_NormalMap");
				break;
			case DW_SPECULAR:
				loc = glGetUniformLocation(programObject, "u_SpecularMap");
				break;
			case DW_HEIGHT:
				loc = glGetUniformLocation(programObject, "u_HeightMap");
				break;
			}

			if (loc != -1) {
				glUniform1i(loc, k);
				glActiveTexture(GL_TEXTURE0 + k);
				glBindTexture(GL_TEXTURE_2D, tex.m_hTexture);
			}

			k++;
		}


		// Set material uniforms.
		GLint loc;
		loc = glGetUniformLocation(programObject, "u_Material.Diffuse");
		if (loc != -1) {
			glUniform4fv(loc, 1, pObject->m_material.Diffuse);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Ambient");
		if (loc != -1) {
			glUniform4fv(loc, 1, pObject->m_material.Ambient);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Specular");
		if (loc != -1) {
			glUniform4fv(loc, 1, pObject->m_material.Specular);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Shininess");
		if (loc != -1) {
			glUniform1f(loc, pObject->m_material.Shininess);
		}

		loc = glGetUniformLocation(programObject, "u_Material.Id");
		if (loc != -1) {
			glUniform1f(loc, static_cast<float>(pObject->m_material.Id) / 10.0f);
		}

		// Set matrix uniforms.
		loc = glGetUniformLocation(programObject, "u_ModelViewProjectionMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pObject->m_ModelViewProjectionMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelViewMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pObject->m_ModelViewMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ModelMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pObject->m_ModelMatrix);
		}

		loc = glGetUniformLocation(programObject, "u_ViewProjectionMatrix");
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, pCamera->GetViewMatrix() * pCamera->GetProjectionMatrix());
		}

		// Draw each mesh that the object is made out of.
		for (auto jt = pObject->m_modelNames.begin(); jt != pObject->m_modelNames.end(); ++jt) {

			Model m = pResourceManager->GetModelByName(jt->c_str());

			Model::VertexArraysPointer(programObject, m.m_vertexOffset);

			glDrawElementsInstanced(GL_TRIANGLES, m.m_indexCount, GL_UNSIGNED_INT,
				reinterpret_cast<void *>(m.m_indexOffset * sizeof(int)), pObject->m_instances);
		}
	}


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

	ESDevice * pDevice = ESDevice::GetInstance();

	Camera * pCamera = SceneManager::GetInstance()->GetActiveCamera();

	// Enable.
	glUseProgram(programObject);

	// Bind.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFBO);

	GLenum buffers [] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
	};

	glDrawBuffers(4, buffers);

	// Only the geometry pass updates the depth buffer.
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	GLint loc = glGetUniformLocation(programObject, "u_EyePosition");
	if (loc != -1) {
		glUniform3fv(loc, 1, pCamera->GetEyePosition());
	}

	DrawCombinedMeshes(programObject);

	glDepthMask(GL_FALSE);

	// Copy data from the multisampled renderbuffers to textures.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);

	for (unsigned int i = 0; i < 4; i++) {

		glReadBuffer(buffers[i]);
		glDrawBuffers(1, &buffers[i]);
		glBlitFramebuffer(
			0, 0, 
			pDevice->GetWindowWidth(), 
			pDevice->GetWindowHeight(), 
			0, 0, 
			pDevice->GetWindowWidth(), 
			pDevice->GetWindowHeight(),
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
		SceneManager * pSceneManager = SceneManager::GetInstance();
		Camera * pCamera = pSceneManager->GetActiveCamera();

		const char * uniformSamplerNames [] = {
			"u_DiffuseAndNx",
			"u_AmbientAndNy",
			"u_PositionAndMaterial",
			"u_SpecularAndShiny"
		};

		GLint loc;

		for (int i = 0; i < 4; i++) {
			loc = glGetUniformLocation(programObject, uniformSamplerNames[i]);
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

		if (!glIsBuffer(lightUBO)) {
			lightUBO = CreateLightParamsUniformBuffer(programObject);
		}

		if (lightUBO != -1) {

			glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4) * 3, sizeof(Vector4), LightPosition);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			GLuint blockIndex = glGetUniformBlockIndex(programObject, "Lights");
			glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, lightUBO);
		}

	}


	/**************************************************************************
	* Draw screenspace quad.												  *
	**************************************************************************/

	Vector3 QuadPositions [] = {
		Vector3(-1.0, -1.0, 0.0), Vector3( 1.0, -1.0, 0.0), Vector3( 1.0,  1.0, 0.0),
		Vector3(-1.0, -1.0, 0.0), Vector3( 1.0,  1.0, 0.0), Vector3(-1.0,  1.0, 0.0),
	};

	Vector2 QuadTexCoords [] = {
		Vector2(0.0, 0.0), Vector2(1.0, 0.0), Vector2(1.0, 1.0),
		Vector2(0.0, 0.0), Vector2(1.0, 1.0), Vector2(0.0, 1.0),
	};

	char * attrNames[] = {
		"in_Position",
		"in_TexCoord"
	};

	GLint attrLoc[2];

	// Get attribute locations.
	for (int i = 0; i < 2; i++) {
		attrLoc[i] = glGetAttribLocation(programObject, attrNames[i]);
	}

	// Bind position data to locations.
	if (attrLoc[0] != -1) {
		glVertexAttribPointer(attrLoc[0], 3, GL_FLOAT, GL_FALSE, 0, QuadPositions);
	}

	// Bind texture coordinate data to locations.
	if (attrLoc[1] != -1) {
		glVertexAttribPointer(attrLoc[1], 2, GL_FLOAT, GL_FALSE, 0, QuadTexCoords);
	}

	// Enable vertex arrays.
	for (int i = 0; i < 2; i++) {
		if (attrLoc[i] != -1) {
			glEnableVertexAttribArray(attrLoc[i]);
		}
	}

	// Draw screenspace quad.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Disable vertex arrays.
	for (int i = 0; i < 2; i++) {
		if (attrLoc[i] != -1) {
			glDisableVertexAttribArray(attrLoc[i]);
		}
	}

}

void DrawFunc () {

	Clock::UpdateFPS();
	printf("%f\n", Clock::fps);

	ESDevice * pDevice = ESDevice::GetInstance();

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the viewport
	glViewport(0, 0, pDevice->GetWindowWidth(), pDevice->GetWindowHeight());

	//ForwardLighting (programObject0);
	GeometryPass(programObject1);
	LightingPass(programObject2);

	// Swap buffers.
	pDevice->SwapBuffers();
}

float yaw   = 0.0f;
float pitch = 0.0f;

void UpdateFunc (float t) {

	SceneManager * pSceneManager = SceneManager::GetInstance();
	ResourceManager * pResourceManager = ResourceManager::GetInstance();


	Camera * pCamera = pSceneManager->GetActiveCamera();

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
		LightPosition.z -= t;
	}
	if (keyPressed[DW_KEY_ARROW_DOWN]) {
		LightPosition.z += t;
	}
	if (keyPressed[DW_KEY_ARROW_LEFT]) {
		LightPosition.x -= t;
	}
	if (keyPressed[DW_KEY_ARROW_RIGHT]) {
		LightPosition.x += t;
	}
	if (keyPressed[DW_PLUS]) {
		LightPosition.y += t;
	}
	if (keyPressed[DW_MINUS]) {
		LightPosition.y -= t;
	}

	//LightPosition *= Matrix4::Rotation(0, 3 * t, 0);

	pCamera->Rotate(yaw, pitch);
	float lambda = 100.0f;
	yaw   *= exp(-lambda * t);
	pitch *= exp(-lambda * t);
	pCamera->UpdatePosition(direction, t);

	int k = 0;
	for (auto it = pSceneManager->m_sceneObjects.begin(); it != pSceneManager->m_sceneObjects.end(); ++it) {

		SceneObject * pObject = it->second;

		if (k == 0) {
			pObject->m_RotationMatrix *= Matrix4::Rotation(0, t, 0);
		}

		pObject->ComputeModelMatrix();
		pObject->m_ModelViewMatrix           = pObject->m_ModelMatrix * pCamera->GetViewMatrix();
		pObject->m_ModelViewProjectionMatrix = pObject->m_ModelViewMatrix * pCamera->GetProjectionMatrix();

		k++;
	}
}


void KeyboardEventFunc(const KeyboardEvent & ev) {

	Camera * pCamera = SceneManager::GetInstance()->GetActiveCamera();

	Vector3 velocity = pCamera->GetCurrentVelocity();

	keyID_t key = DW_KEY_NOT_ASSIGNED;

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
	case KEY_ADD:
		key = DW_PLUS;
		break;
	case KEY_SUBTRACT:
		key = DW_MINUS;
		break;
	}

	if (key != DW_KEY_NOT_ASSIGNED) {
		if (ev.pressed) {
			if (!keyPressed[key]) {
				keyPressed[key] = true;
				pCamera->SetCurrentVelocity(velocity);
			}
		} else {
			keyPressed[key] = false;
		}
	}

}


void MouseEventFunc (const MouseEvent & ev) {

	static int prevX = 0;
	static int prevY = 0;

	if (ev.type == DW_MOUSE_MOVE && ev.left) {

		yaw   = radians(static_cast<float>( ev.x - prevX ));
		pitch = radians(static_cast<float>( ev.y - prevY ));
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
	escp.windowPositionX = 0;
	escp.windowPositionY = 0;
	escp.redSize         = 8;
	escp.greenSize       = 8;
	escp.blueSize        = 8;
	escp.alphaSize       = 0;
	escp.depthSize       = 24;
	escp.stencilSize     = 0;

	ESDevice * pDevice = ESDevice::GetInstance();

	pDevice->CreateDisplay("Darwin", escp);

	Init();

	pDevice->SetDrawFunc(DrawFunc);
	pDevice->SetUpdateFunc(UpdateFunc);

	pDevice->SetMouseEventFunc(MouseEventFunc);
	pDevice->SetKeyboardEventFunc(KeyboardEventFunc);

	pDevice->Run();

	ESDevice::DestroyInstance();

	return EXIT_SUCCESS;
}
