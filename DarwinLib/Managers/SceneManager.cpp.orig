
#include "SceneManager.h"

#include <cstdlib>

SceneManager * SceneManager::m_pInstance = NULL;

SceneManager::SceneManager (const char * dir) : 
	m_directory(dir) {}

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
		}

		// Continue to next scene entity.
		xmlEntity = xmlEntity->NextSiblingElement();
	}


	return true;
}

bool SceneManager::ImportCamera (const tinyxml2::XMLElement * xmlCam) {

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

			pCamera->Rotate(yaw, pitch);
		}

		xmlCamParam = xmlCamParam->NextSiblingElement();
	}

	// Add camera to registered scene cameras.
	m_sceneCameras[cameraName] = pCamera;

	// If there is no active camera, make it the active camera.
	if (m_activeCameraName.empty()) {
		m_activeCameraName = cameraName;
	}

	return true;
}

bool SceneManager::ImportObject (const tinyxml2::XMLElement * xmlObject) {

	// TODO: Handle errors.

	using namespace tinyxml2;

	XMLElement * xmlObjectParam = const_cast<XMLElement *>( xmlObject->FirstChildElement() );

	while (xmlObjectParam) {

		const char * paramName = xmlObjectParam->Name();

		if (strncmp("Model", paramName, 5) == 0) {

			// We have a model.
			ImportModel(xmlObjectParam);

		} else if (strncmp("Texture", paramName, 7) == 0) {

			// We have a texture.
			ImportTexture(xmlObjectParam);

		} else if (strncmp("Material", paramName, 8) == 0) {

			int materialID = static_cast<int>( atof(xmlObjectParam->Attribute("id")) );
		}

		xmlObjectParam = xmlObjectParam->NextSiblingElement();
	}

	return true;
}

bool SceneManager::ImportModel (const tinyxml2::XMLElement * xmlModel) {

	// TODO: Handle errors.

	// TODO

	return true;
}


bool SceneManager::ImportTexture (const tinyxml2::XMLElement * xmlTexture) {

	// TODO: Handle errors.

	// TODO

	return true;
}

// Return pointer to the currently active camera.
// If no camera is active, returns NULL.
Camera * SceneManager::GetActiveCamera () const {

	if (m_activeCameraName.empty()) {
		return NULL;
	}

	return m_sceneCameras.at(m_activeCameraName);
}
