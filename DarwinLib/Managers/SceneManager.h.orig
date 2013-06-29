
#pragma once

#include "../../Externals/tinyxml2/tinyxml2.h"

#include "Renderer/Camera.h"
#include "Device/ESDevice.h"

#include <string>
#include <map>

typedef std::map<std::string, Camera *>	SceneCameras;

class SceneManager {

// Private members.
private:
	
	static SceneManager *	m_pInstance;

	std::string				m_directory;

	std::string				m_activeCameraName;
	SceneCameras			m_sceneCameras;


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
	const char *			GetWorkingDirectory () const { return m_directory.c_str(); }
	inline
	void					SetWorkingDirectory (const char * dir) { m_directory = std::string(dir); }

	Camera *				GetActiveCamera () const;


// Private methods.
private:
							SceneManager () {};
							SceneManager (const char * dir);
	virtual					~SceneManager () {};

	bool					ImportCamera  (const tinyxml2::XMLElement * xmlCamera);
	bool					ImportObject  (const tinyxml2::XMLElement * xmlObject);
	bool					ImportModel	  (const tinyxml2::XMLElement * xmlModel);
	bool					ImportTexture (const tinyxml2::XMLElement * xmlTexture);

};
