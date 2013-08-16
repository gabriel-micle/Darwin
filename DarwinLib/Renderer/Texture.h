
#pragma once

#include <string>

enum textureUsage_t {
	DW_DIFFUSE,
	DW_SPECULAR,
	DW_NORMAL,
	DW_HEIGHT,
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

	unsigned int	mipmaps;

	TextureOpts () {};

	TextureOpts (
		textureUsage_t	_usage, 
		textureFormat_t _format, 
		textureFilter_t _filter, 
		textureWrap_t	_wrap, 
		unsigned int	_mipmaps) :
		usage			(_usage),
		format			(_format),
		filter			(_filter),
		wrap			(_wrap),
		mipmaps			(_mipmaps) {};
};


class Texture {

// Public members.
public:

	GLuint			m_hTexture;
	TextureOpts		m_opts;

	int				m_width;
	int				m_height;
	int				m_channels;


// Private members.
private:

	std::string		m_textureName;

	textureType_t	m_type;

	GLuint			m_internalFormat;
	GLuint			m_dataFormat;
	GLuint			m_dataType;
	GLuint			m_target;


// Public methods.
public:
					Texture () {}
	virtual			~Texture () {}

	void			Bind (int unit);

	void			Generate2D (const char * imageData, int width, int height, const TextureOpts & opts);
	void			GenerateCube (const char * imageData[6], int size, const TextureOpts & opts);


// Private methods.
private:

	void			AllocImage ();
	void			UploadSubData (int x, int y, int z, const char * imageData, int width, int height);

};




