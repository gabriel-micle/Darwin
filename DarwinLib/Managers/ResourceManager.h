
#pragma once


struct Vertex;
class  Model;
class  Texture;

class ResourceManager {

// Private members.
private:

	static ResourceManager *			m_pInstance;

	Vertex *							m_VertexPool;
	int *								m_IndexPool;

	std::map<std::string, Model *>		m_ModelMap;
	std::map<std::string, Texture *>	m_TextureMap; 


// Public methods.
public:

	static void							CreateInstance ();
	static void							DestroyInstance ();

	static ResourceManager *			GetInstance () { return m_pInstance; }

	void								AddModel   (const char * name, Model *);
	void								AddTexture (const char * name, Texture *);


// Private methods.
private:

										ResourceManager ();
	virtual								~ResourceManager ();
};
