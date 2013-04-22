
#ifndef _TEXTURE_H_
#define _TEXTURE_H_


enum textureUsage_t {
	DW_DIFFUSE,
	DW_SPECULAR,
	DW_BUMP,
	DW_DEPTH,
};

enum textureType_t {
	DW_2D,
	DW_CUBE,
};

enum textureFormat_t {
	DW_RGBA8,
	DW_RGB8,
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

	int				mipmaps;
	bool			compressed;
};


class Texture {

private:

	textureType_t	type;

	TextureOpts		opts;

	int				channels;
	char *			name;

	GLuint			internalFormat;
	GLuint			dataFormat;
	GLuint			dataType;
	GLuint			target;

	void			allocImage ();
	void			uploadSubData (int x, int y, int z, const char * imageData, int width, int height);

public:

	GLuint			handle;

	int				width;
	int				height;

					Texture (const char * name);

	void			bind (int unit);

	void			generate2D (const char * imageData, int width, int height, TextureOpts & opts);
	void			generateCube (const char * imageData[6], int size, TextureOpts & opts);


};

#endif
