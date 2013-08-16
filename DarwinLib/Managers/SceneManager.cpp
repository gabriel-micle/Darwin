

#include "Renderer/Renderer.h"
#include "IO/IO.h"
#include "Managers.h"

#include <cstdlib>

SceneManager * SceneManager::m_pInstance = NULL;

SceneManager::SceneManager (const char * dir) :
	m_directory	(dir) {}

void SceneManager::CreateInstance () {

	if (m_pInstance == NULL) {
		m_pInstance = new SceneManager();
	}
}

void SceneManager::CreateInstance (const char * dir) {
	if (m_pInstance == NULL) {
		m_pInstance = new SceneManager(dir);
	}
}

void SceneManager::DestroyInstance () {

	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool SceneManager::ImportScene (const char * fileName) {

	using namespace tinyxml2;

	std::string fullFilePath = m_directory;
	fullFilePath.append(fileName);

	XMLDocument * xmlDoc = new XMLDocument();

	XMLError xmlError = xmlDoc->LoadFile(fullFilePath.c_str());
	if (XML_NO_ERROR != xmlError) {
		printf("XML load error!\n");
		delete xmlDoc;
		xmlDoc = NULL;
		return false;
	}

	bool bRet = ImportScene(xmlDoc);

	delete xmlDoc;
	xmlDoc = NULL;

	return bRet;
}

bool SceneManager::ImportScene (FILE * pFile) {

	using namespace tinyxml2;

	XMLDocument * xmlDoc = new XMLDocument();

	XMLError xmlError = xmlDoc->LoadFile(pFile);
	if (XML_NO_ERROR != xmlError) {
		printf("XML load error!\n");
		delete xmlDoc;
		xmlDoc = NULL;
		return false;
	}

	bool bRet = ImportScene(xmlDoc);

	delete xmlDoc;
	xmlDoc = NULL;

	return bRet;

}

bool SceneManager::ImportScene (const tinyxml2::XMLDocument * xmlDoc) {

	using namespace tinyxml2;

	// Get root element of the XML document.
	XMLElement * xmlElem = const_cast<XMLElement *>( xmlDoc->RootElement() );


	XMLElement * xmlEntity = xmlElem->FirstChildElement();

	while (xmlEntity) {

		const char * entityName = xmlEntity->Name();

		if (strncmp("Camera", entityName, 6) == 0) {

			// We have a camera.
			ImportCamera(xmlEntity);

		} else if (strncmp("Object", entityName, 6) == 0) {

			// We have a scene object.
			ImportObject(xmlEntity);

		} else if (strncmp("Light", entityName, 5) == 0) {

			ImportLight(xmlEntity);
		}

		// Continue to next scene entity.
		xmlEntity = xmlEntity->NextSiblingElement();
	}


	return true;
}

void SceneManager::ImportCamera (const tinyxml2::XMLElement * xmlCam) {

	// TODO: Handle errors.

	using namespace tinyxml2;

	// Create a new camera.
	Camera * pCamera = new Camera();

	// Get camera name.
	std::string cameraName(xmlCam->Attribute("name"));

	XMLElement * xmlCamParam = const_cast<XMLElement *>( xmlCam->FirstChildElement() );

	while (xmlCamParam) {

		const char * paramName = xmlCamParam->Name(); 

		if (strncmp("Perspective", paramName, 11) == 0) {

			// Register perspective transform.
			float fovY  = static_cast<float>( atof(xmlCamParam->Attribute("fovY")) );
			float zNear = static_cast<float>( atof(xmlCamParam->Attribute("zNear")) );
			float zFar  = static_cast<float>( atof(xmlCamParam->Attribute("zFar")) );

			float width  = static_cast<float>( ESDevice::GetInstance()->GetWindowWidth() );
			float height = static_cast<float>( ESDevice::GetInstance()->GetWindowHeight() );
			float aspect = width / height;

			pCamera->SetPerspective(fovY, aspect, zNear, zFar);

		} else if (strncmp("Position", paramName, 8) == 0) {

			// Register camera position.
			Vector3 eyePosition = Vector3(
				static_cast<float>( atof(xmlCamParam->Attribute("x")) ),
				static_cast<float>( atof(xmlCamParam->Attribute("y")) ),
				static_cast<float>( atof(xmlCamParam->Attribute("z")) )
				);

			pCamera->SetPosition(eyePosition);

		} else if (strncmp("Rotation", paramName, 8) == 0) {

			// Register camera rotation.
			float pitch = static_cast<float>( atof(xmlCamParam->Attribute("x")) );
			float yaw   = static_cast<float>( atof(xmlCamParam->Attribute("y")) );

			pCamera->Rotate(radians(yaw), radians(pitch));
		}

		xmlCamParam = xmlCamParam->NextSiblingElement();
	}

	// Add camera to registered scene cameras.
	m_sceneCameras[cameraName] = pCamera;

	// If there is no active camera, make it the active camera.
	if (m_activeCameraName.empty()) {
		m_activeCameraName = cameraName;
	}

}

void SceneManager::ImportObject (const tinyxml2::XMLElement * xmlObject) {

	// TODO: Handle errors.

	using namespace tinyxml2;

	// Create a new scene object.
	SceneObject * pSceneObject = new SceneObject();

	const char * objectName = xmlObject->Attribute("name");
	const char * instances  = xmlObject->Attribute("instances");

	if (instances != NULL) {
		pSceneObject->m_instances = atoi(instances);
	}

	XMLElement * xmlObjectParam = const_cast<XMLElement *>( xmlObject->FirstChildElement() );

	while (xmlObjectParam) {

		const char * paramName = xmlObjectParam->Name();

		if (strncmp("Models", paramName, 5) == 0) {

			// We have the list of models.
			pSceneObject->m_modelNames = ImportModels(xmlObjectParam);

		} else if (strncmp("Textures", paramName, 7) == 0) {

			// We have the list of textures.
			pSceneObject->m_textureNames = ImportTextures(xmlObjectParam);

		} else if (strncmp("Material", paramName, 8) == 0) {

			// We have the material ID.
			pSceneObject->m_material = ImportMaterial(xmlObjectParam);

		} else if (strncmp("Position", paramName, 8) == 0) {

			// We have the object's position in the world.
			float x = static_cast<float>( atof(xmlObjectParam->Attribute("x")) );
			float y = static_cast<float>( atof(xmlObjectParam->Attribute("y")) );
			float z = static_cast<float>( atof(xmlObjectParam->Attribute("z")) );

			pSceneObject->m_TranslationMatrix[3][0] = x;
			pSceneObject->m_TranslationMatrix[3][1] = y;
			pSceneObject->m_TranslationMatrix[3][2] = z;

		} else if (strncmp("Rotation", paramName, 8) == 0) {

			// We have the object's rotation in the world.
			float x = static_cast<float>( atof(xmlObjectParam->Attribute("x")) );
			float y = static_cast<float>( atof(xmlObjectParam->Attribute("y")) );
			float z = static_cast<float>( atof(xmlObjectParam->Attribute("z")) );

			pSceneObject->m_RotationMatrix = Matrix4::Rotation(radians(x), radians(y), radians(z));

		} else if (strncmp("Scale", paramName, 5) == 0) {

			// We have the object's scale in the world.
			float x = static_cast<float>( atof(xmlObjectParam->Attribute("x")) );
			float y = static_cast<float>( atof(xmlObjectParam->Attribute("y")) );
			float z = static_cast<float>( atof(xmlObjectParam->Attribute("z")) );

			pSceneObject->m_ScaleMatrix[0][0] = x;
			pSceneObject->m_ScaleMatrix[1][1] = y;
			pSceneObject->m_ScaleMatrix[2][2] = z;
		}

		xmlObjectParam = xmlObjectParam->NextSiblingElement();
	}

	m_sceneObjects[std::string(objectName)] = pSceneObject;

}

StringArray SceneManager::ImportModels (const tinyxml2::XMLElement * xmlModels) {

	// TODO: Handle errors.
	using namespace tinyxml2;

	StringArray modelNames;

	XMLElement * xmlSingleModel = const_cast<XMLElement *>( xmlModels->FirstChildElement() );

	while (xmlSingleModel) {

		std::string modelName = ImportModel(xmlSingleModel);

		modelNames.push_back(modelName);

		xmlSingleModel = xmlSingleModel->NextSiblingElement();
	}

	return modelNames;
}

StringArray SceneManager::ImportTextures (const tinyxml2::XMLElement * xmlTextures) {

	// TODO: Handle errors.
	using namespace tinyxml2;

	StringArray textureNames;

	XMLElement * xmlSingleTexture = const_cast<XMLElement *>( xmlTextures->FirstChildElement() );

	while (xmlSingleTexture) {

		std::string textureName = ImportTexture(xmlSingleTexture);

		textureNames.push_back(textureName);

		xmlSingleTexture = xmlSingleTexture->NextSiblingElement();
	}

	return textureNames;
}

std::string SceneManager::ImportModel (const tinyxml2::XMLElement * xmlModel) {

	// TODO: Handle errors.
	using namespace tinyxml2;

	const char * modelName = xmlModel->Attribute("name");
	const char * modelExt  = xmlModel->Attribute("extension");

	ResourceManager * pResourceManager = ResourceManager::GetInstance();

	std::string filePath = 
		pResourceManager->GetModelDirectory() +
		modelName + "." + modelExt;

	if (strncmp("obj", modelExt, 3) == 0) {

		if (!pResourceManager->FindModelByName(modelName)) {

			// Import model from Wavefront Object file.
			Model model;

			// Calculate new model offsets for vertex and index data.
			model.m_indexOffset  = pResourceManager->m_IndexPool.size();
			model.m_vertexOffset = pResourceManager->m_VertexPool.size();

			// Add new model to resources.
			Wavefront::ImportOBJ(filePath.c_str(), 
				pResourceManager->m_VertexPool, pResourceManager->m_IndexPool);

			// Calculate number of vertices and number of indices for new model.
			model.m_indexCount  = pResourceManager->m_IndexPool.size()  - model.m_indexOffset;
			model.m_vertexCount = pResourceManager->m_VertexPool.size() - model.m_vertexOffset;

			pResourceManager->AddModel(modelName, model);
		}

	} else {

		// Unsupported file type.
		printf("Model format not supported!\n");
		return "";
	}

	return std::string(modelName);
}


std::string SceneManager::ImportTexture (const tinyxml2::XMLElement * xmlTexture) {

	// TODO: Handle errors.
	using namespace tinyxml2;

	Texture texture;
	TextureOpts opts;

	const char * textureUsage  = xmlTexture->Name();
	const char * textureName   = xmlTexture->Attribute("name");
	const char * textureExt    = xmlTexture->Attribute("extension");
	const char * textureType   = xmlTexture->Attribute("type");
	const char * textureFilter = xmlTexture->Attribute("filter");
	const char * textureWrap   = xmlTexture->Attribute("wrap");

	// Set texture shader usage.
	if (strncmp("Diffuse", textureUsage, 7) == 0) {
		opts.usage = DW_DIFFUSE;
	} else if (strncmp("Normal", textureUsage, 6) == 0) {
		opts.usage = DW_NORMAL;
	} else if (strncmp("Specular", textureUsage, 7) == 0) {
		opts.usage = DW_SPECULAR;
	} else if (strncmp("Height", textureUsage, 6) == 0) {
		opts.usage = DW_HEIGHT;
	}

	// Set texture filter.
	if (textureFilter != NULL) {
		if (strncmp("nearest", textureFilter, 7) == 0) {
			opts.filter = DW_NEAREST;
		} else if (strncmp("linear", textureFilter, 6) == 0) {
			opts.filter = DW_LINEAR;
		} else if (strncmp("bilinear", textureFilter, 8) == 0) {
			opts.filter = DW_BILINEAR;
		} else if (strncmp("trilinear", textureFilter, 9) == 0) {
			opts.filter = DW_TRILINEAR;
		}
	} else {
		opts.filter = DW_NEAREST;
	}

	// Set texture wrap.
	if (textureWrap != NULL) {
		if (strncmp("clamp", textureWrap, 5) == 0) {
			opts.wrap = DW_CLAMP;
		} else if (strncmp("repeat", textureWrap, 6) == 0) {
			opts.wrap = DW_REPEAT;
		}
	} else {
		opts.wrap = DW_CLAMP;
	}

	// Set mipmaps.
	opts.mipmaps = 4;

	ResourceManager * pResourceManager = ResourceManager::GetInstance();

	std::string filePath =
		pResourceManager->GetTextureDirectory() +
		textureName + "." + textureExt;;

	if (strncmp("tga", textureExt, 3) == 0) {

		// Import image from Truevision Targa file.
		if (strncmp("2d", textureType, 2) == 0) {

			int width, height, channels;
			char * pixels = Truevision::ImportTGA(filePath.c_str(), &width, &height, &channels);

			opts.format = (channels == 3) ? DW_RGB8 : DW_RGBA8;

			texture.Generate2D(pixels, width, height, opts);

			pResourceManager->AddTexture(textureName, texture);

		} else if (strncmp("cube", textureType, 4) == 0) {

			// TODO.
			int width, height, channels;
			char * pixels[6];
		}

	} else {

		// Unsupported image type.
		printf("Image format not supported!\n");
		return "";
	}

	return std::string(textureName);
}

void SceneManager::ImportLight (const tinyxml2::XMLElement * xmlLight) {

	using namespace tinyxml2;

	Light light;

	XMLElement * xmlLightParam = const_cast<XMLElement *>( xmlLight->FirstChildElement() );

	const char * lightType = xmlLight->Attribute("type");

	if (strncmp("spot", lightType, 4) == 0) {
		light.Position.w = 1.0f;
	} else if (strncmp("point", lightType, 5) == 0) {
		light.Position.w = 1.0f;
	} else if (strncmp("directional", lightType, 11) == 0) {
		light.Position.w = 0.0f;
	}

	while (xmlLightParam) {

		const char * paramName = xmlLightParam->Name();

		if (strncmp("Position", paramName, 8) == 0) {

			light.Position.x = static_cast<float>( atof(xmlLightParam->Attribute("x")) );
			light.Position.y = static_cast<float>( atof(xmlLightParam->Attribute("y")) );
			light.Position.z = static_cast<float>( atof(xmlLightParam->Attribute("z")) );

		} else if (strncmp("Diffuse", paramName, 7) == 0) {

			light.Diffuse.x = static_cast<float>( atof(xmlLightParam->Attribute("r")) );
			light.Diffuse.y = static_cast<float>( atof(xmlLightParam->Attribute("g")) );
			light.Diffuse.z = static_cast<float>( atof(xmlLightParam->Attribute("b")) );

		} else if (strncmp("Ambient", paramName, 7) == 0) {

			light.Ambient.x = static_cast<float>( atof(xmlLightParam->Attribute("r")) );
			light.Ambient.y = static_cast<float>( atof(xmlLightParam->Attribute("g")) );
			light.Ambient.z = static_cast<float>( atof(xmlLightParam->Attribute("b")) );

		} else if (strncmp("Specular", paramName, 8) == 0) {

			light.Specular.x = static_cast<float>( atof(xmlLightParam->Attribute("r")) );
			light.Specular.y = static_cast<float>( atof(xmlLightParam->Attribute("g")) );
			light.Specular.z = static_cast<float>( atof(xmlLightParam->Attribute("b")) );

		} else {

			if (light.Position.w == 1.0f) {

				if (strncmp("Attenuation", paramName, 11) == 0) {

					light.Attenuation.x = static_cast<float>( atof(xmlLightParam->Attribute("kc")) );
					light.Attenuation.y = static_cast<float>( atof(xmlLightParam->Attribute("kl")) );
					light.Attenuation.z = static_cast<float>( atof(xmlLightParam->Attribute("kq")) );

				} else if (strncmp("SpotExponent", paramName, 12) == 0) {

					light.SpotExponent = static_cast<float>( atof(xmlLightParam->Attribute("value")) );

				} else if (strncmp("SpotCutoff", paramName, 10) == 0) {

					light.SpotCutoff = static_cast<float>( atof(xmlLightParam->Attribute("value")) );

				} else if (strncmp("SpotDirection", paramName, 13) == 0) {

					light.SpotDirection.x = static_cast<float>( atof(xmlLightParam->Attribute("x")) );
					light.SpotDirection.y = static_cast<float>( atof(xmlLightParam->Attribute("y")) );
					light.SpotDirection.z = static_cast<float>( atof(xmlLightParam->Attribute("z")) );

				} // if

			}

		} // if

		xmlLightParam = xmlLightParam->NextSiblingElement();

	} // while

	m_sceneLights.push_back(light);
}

Material SceneManager::ImportMaterial (const tinyxml2::XMLElement * xmlMaterial) {

	using namespace tinyxml2;

	Material material;

	material.Id = atoi(xmlMaterial->Attribute("id"));

	XMLElement * xmlMaterialParam = const_cast<XMLElement *>(xmlMaterial->FirstChildElement());

	while (xmlMaterialParam) {

		const char * paramName = xmlMaterialParam->Name();

		if (strncmp("Ambient", paramName, 7) == 0) {

			material.Ambient.x = static_cast<float>( atof(xmlMaterialParam->Attribute("r")) );
			material.Ambient.y = static_cast<float>( atof(xmlMaterialParam->Attribute("g")) );
			material.Ambient.z = static_cast<float>( atof(xmlMaterialParam->Attribute("b")) );

		} else if (strncmp("Diffuse", paramName, 7) == 0) {

			material.Diffuse.x = static_cast<float>( atof(xmlMaterialParam->Attribute("r")) );
			material.Diffuse.y = static_cast<float>( atof(xmlMaterialParam->Attribute("g")) );
			material.Diffuse.z = static_cast<float>( atof(xmlMaterialParam->Attribute("b")) );

		} else if (strncmp("Specular", paramName, 8) == 0) {

			material.Specular.x = static_cast<float>( atof(xmlMaterialParam->Attribute("r")) );
			material.Specular.y = static_cast<float>( atof(xmlMaterialParam->Attribute("g")) );
			material.Specular.z = static_cast<float>( atof(xmlMaterialParam->Attribute("b")) );

		} else if (strncmp("Shininess", paramName, 9) == 0) {

			material.Shininess = static_cast<float>( atof(xmlMaterialParam->Attribute("value")) );
		}

		xmlMaterialParam = xmlMaterialParam->NextSiblingElement();
	}

	return material;
}

// Return pointer to the currently active camera.
// If no camera is active, returns NULL.
Camera * SceneManager::GetActiveCamera () const {

	if (m_activeCameraName.empty()) {
		return NULL;
	}

	return m_sceneCameras.at(m_activeCameraName);
}

void SceneManager::SetActiveCamera (const char * name) {

	std::string cameraName(name);
}
