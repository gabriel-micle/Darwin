
#pragma once


enum textureUsage_t {
	DW_DIFFUSE,
	DW_SPECULAR,
	DW_NORMAL,
};

enum textureType_t {
	DW_2D,
	DW_CUBE,
};

enum textureFormat_t {
	DW_RGBA8,
	DW_RGB8,
	DW_RGBA16,
	DW_RGB16,
	DW_RGBA32,
	DW_RGB32,
};

enum textureFilter_t {
	DW_TRILINEAR,
	DW_BILINEAR,
	DW_LINEAR,
	DW_NEAREST,
};

enum textureWrap_t {
	DW_REPEAT,
	DW_CLAMP,
};

struct TextureOpts {

	textureUsage_t	usage;
	textureFormat_t	format;
	textureFilter_t	filter;
	textureWrap_t	wrap;
	GLuint			mipmaps;

};


class Texture {

private:

	textureType_t	m_type;

	TextureOpts		m_opts;

	GLint			m_channels;
	const char *	m_name;

	GLuint			m_internalFormat;
	GLuint			m_dataFormat;
	GLuint			m_dataType;
	GLuint			m_target;


	void			AllocImage ();
	void			UploadSubData (int x, int y, int z, const char * imageData, int width, int height);

public:

	GLuint			m_hTexture;
	
	GLuint			m_sampler;

	GLint			m_width;
	GLint			m_height;

					Texture (const char * name);

	void			Generate2D   (const char * imageData, int width, int height, const TextureOpts & opts);
	void			GenerateCube (const char * imageData[6], int size, const TextureOpts & opts);

};
