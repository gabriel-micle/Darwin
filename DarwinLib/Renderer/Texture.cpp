
#include "Device/ESDevice.h"
#include "Texture.h"

#include <cstdio>
#include <cassert>


void Texture::Bind (int unit) {

}

void Texture::Generate2D (const char * imageData, int width, int height, const TextureOpts & opts) {

	m_type    = DW_2D;
	m_width   = width;
	m_height  = height;
	m_opts    = opts;

	// Allocate image and set texture parameters.
	AllocImage();

	// Upload pixel data.
	UploadSubData(0, 0, 0, imageData, m_width, m_height);

	// Generate mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Restore initial binding.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateCube (const char * imageData[6], int size, const TextureOpts & opts) {

	m_type   = DW_CUBE;
	m_width  = size;
	m_height = size;
	m_opts   = opts;

	// Allocate image and set texture parameters.
	AllocImage();

	// Upload image data.
	for (int s = 0; s < 6; s++) {
		UploadSubData(0, 0, s, imageData[s], m_width, m_width);
	}

	// Generate mipmaps.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Restore binding.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void Texture::AllocImage () {

	// Set data format for uploading texture.
	switch (m_opts.format) {
	case DW_RGBA8:
		m_internalFormat = GL_RGBA8;
		m_dataFormat     = GL_RGBA;
		m_dataType       = GL_UNSIGNED_BYTE;
		m_channels	     = 4;
		break;
	case DW_RGB8:
		m_internalFormat = GL_RGB8;
		m_dataFormat     = GL_RGB;
		m_dataType       = GL_UNSIGNED_BYTE;
		m_channels       = 3;
		break;
	case DW_RGBA16:
		m_internalFormat = GL_RGBA16F;
		m_dataFormat     = GL_RGBA;
		m_dataType       = GL_HALF_FLOAT;
		m_channels       = 4;
		break;
	case DW_RGB16:
		m_internalFormat = GL_RGB16F;
		m_dataFormat     = GL_RGBA;
		m_dataType       = GL_HALF_FLOAT;
		m_channels       = 3;
		break;
	case DW_RGBA32:
		m_internalFormat = GL_RGBA32F;
		m_dataFormat     = GL_RGBA;
		m_dataType       = GL_FLOAT;
		m_channels       = 4;
		break;
	case DW_RGB32:
		m_internalFormat = GL_RGB32F;
		m_dataFormat     = GL_RGBA;
		m_dataType       = GL_FLOAT;
		m_channels       = 3;
		break;
	default:
		printf("Texture format not supported!\n");
		break;
	}


	// Create texture m_hTexture.
	glGenTextures(1, &m_hTexture);


	// Set texture targets.
	int numSides;
	int uploadTarget;
	switch (m_type) {
	case DW_2D:
		m_target = GL_TEXTURE_2D;
		uploadTarget = GL_TEXTURE_2D;
		numSides = 1;
		break;
	case DW_CUBE:
		m_target = GL_TEXTURE_CUBE_MAP;
		uploadTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		numSides = 6;
		break;
	default:
		printf("Only TEXTURE_2D and TEXTURE_CUBE are supported!\n");
		break;
	}

	// Bind texture.
	glBindTexture(m_target, m_hTexture);

	for (int s = 0; s < numSides; s++) {
		glTexStorage2D(uploadTarget + s, m_opts.mipmaps, m_internalFormat, m_width, m_height);
	}

	// Set texture filtering.
	switch (m_opts.filter) {
	case DW_TRILINEAR:
		glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_BILINEAR:
		glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_LINEAR:
		glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_NEAREST:
		glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	default:
		printf("Texture filtering must be NEAREST, LINEAR, BILINEAR or TRILINEAR!\n");
		break;
	}


	// Set texture wrap modes.
	switch (m_opts.wrap) {
	case DW_REPEAT:
		glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case DW_CLAMP:
		glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	default:
		printf("Texture wrap mode must be REPEAT of CLAMP!\n");
		break;
	}
}

void Texture::UploadSubData (int x, int y, int s, const char * imageData, int width, int height) {

	assert(x >= 0 && x < m_width && y >= 0 && y < m_height);

	// Set upload m_target m_type.
	int uploadTarget;
	switch (m_type) {
	case DW_2D:
		uploadTarget = GL_TEXTURE_2D;
		break;
	case DW_CUBE:
		uploadTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_X + s;
		break;
	default:
		printf("Type must be 2D or CUBE!\n");
		break;
	}

	glBindTexture(m_target, m_hTexture);

	// Upload data.
	glTexSubImage2D(uploadTarget, 0, x, y, width, height, m_dataFormat, m_dataType, imageData);
}
