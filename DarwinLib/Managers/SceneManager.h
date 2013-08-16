
#pragma once

#include "../../Externals/tinyxml2/tinyxml2.h"

#include "Renderer/Light.h"
#include "Renderer/SceneObject.h"

class Camera;
class ESDevice;
class ResourceManager;

#include <string>
#include <map>
#include <vector>

typedef std::map<std::string, Camera *>			SceneCameras;
typedef std::map<std::string, SceneObject *>	SceneObjects;
typedef std::vector<Light>						SceneLights;

typedef std::vector<std::string>				StringArray;

class SceneManager {

// Private members.
private:
	
	static SceneManager *	m_pInstance;

	std::string				m_directory;

	std::string				m_activeCameraName;
	SceneCameras			m_sceneCameras;


public:

	SceneObjects			m_sceneObjects;
	SceneLights				m_sceneLights;


// Public methods.
public:

	static void				CreateInstance ();
	static void				CreateInstance (const char * dir);
	static void				DestroyInstance ();

	inline 
	static SceneManager *	GetInstance () { return m_pInstance; }

	bool					ImportScene (const char * fileName);
	bool					ImportScene (FILE * pFile);
	bool					ImportScene (const tinyxml2::XMLDocument * xmlDoc);

	inline
	std::string				GetWorkingDirectory () const { return m_directory; }
	inline
	void					SetWorkingDirectory (const char * dir) { m_directory = std::string(dir); }

	Camera *				GetActiveCamera () const;

	void					SetActiveCamera (const char * name);

	SceneObject *			GetObjectByName (const char * name) const { return m_sceneObjects.at(std::string(name)); }


// Private methods.
private:
							SceneManager	() {};
							SceneManager	(const char * dir);
	virtual					~SceneManager	() {};

	void					ImportCamera	(const tinyxml2::XMLElement * xmlCamera);
	void					ImportLight		(const tinyxml2::XMLElement * xmlLight);
	void					ImportObject	(const tinyxml2::XMLElement * xmlObject);

	Material				ImportMaterial	(const tinyxml2::XMLElement * xmlMaterial);

	StringArray				ImportModels	(const tinyxml2::XMLElement * xmlModels);
	StringArray				ImportTextures	(const tinyxml2::XMLElement * xmlTextures);

	std::string				ImportModel		(const tinyxml2::XMLElement * xmlModel);
	std::string				ImportTexture	(const tinyxml2::XMLElement * xmlTexture);

};
