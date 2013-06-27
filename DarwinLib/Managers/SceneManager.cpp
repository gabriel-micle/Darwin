
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

		if (strncmp(entityName, "Camera", 6) == 0) {

			// We have a camera.
			ImportCamera(xmlEntity);


		} else if (strncmp(entityName, "Object", 6) == 0) {

			// We have a scene object.
		}

		// Continue to next scene entity.
		xmlEntity = xmlEntity->NextSiblingElement();
	}


	return true;
}

bool SceneManager::ImportCamera (const tinyxml2::XMLElement * xmlCam) {

	using namespace tinyxml2;

	XMLElement * xmlCamParam = const_cast<XMLElement *>( xmlCam->FirstChildElement() );

	while (xmlCamParam) {

		const char * paramName = xmlCamParam->Name(); 

		if (strncmp("Perspective", paramName, 11) == 0) {

			float fovY  = static_cast<float>( atof(xmlCamParam->Attribute("fovY")) );
			float zNear = static_cast<float>( atof(xmlCamParam->Attribute("zNear")) );
			float zFar  = static_cast<float>( atof(xmlCamParam->Attribute("zFar")) );

		} else if (strncmp("Position", paramName, 8) == 0) {

			Vector3 eyePosition = Vector3(
				static_cast<float>( atof(xmlCamParam->Attribute("x")) ),
				static_cast<float>( atof(xmlCamParam->Attribute("y")) ),
				static_cast<float>( atof(xmlCamParam->Attribute("z")) )
				);

		} else if (strncmp("Rotation", paramName, 8) == 0) {

			float pitch = static_cast<float>( atof(xmlCamParam->Attribute("x")) );
			float yaw   = static_cast<float>( atof(xmlCamParam->Attribute("y")) );
		}

		xmlCamParam = xmlCamParam->NextSiblingElement();
	}

	return true;
}