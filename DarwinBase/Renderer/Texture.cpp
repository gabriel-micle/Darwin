
#include "ESUtil.h"
#include "Texture.h"
#include <cstdio>
#include <cassert>


Texture::Texture (const char * name) {



}


void Texture::bind (int unit) {

	if (!glIsTexture(handle)) {
		printf("OK");
	}
}

void Texture::generate2D (const char * imageData, int width, int height, TextureOpts & opts) {

	this->type    = DW_2D;
	this->width   = width;
	this->height  = height;
	this->opts    = opts;

	// Allocate image and set texture parameters.
	this->allocImage();

	// Upload pixel data.
	this->uploadSubData(0, 0, 0, imageData, width, height);

	// Generate mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Restore initial binding.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::allocImage () {

	// Set data format for uploading texture.
	switch (opts.format) {
	case DW_RGBA8:
		internalFormat = opts.compressed ? GL_COMPRESSED_RGBA8_ETC2_EAC : GL_RGBA8;
		dataFormat     = GL_RGBA;
		dataType       = GL_UNSIGNED_BYTE;
		channels	   = 4;
		break;
	case DW_RGB8:
		internalFormat = opts.compressed ? GL_COMPRESSED_RGB8_ETC2 : GL_RGB8;
		dataFormat     = GL_RGB;
		dataType       = GL_UNSIGNED_BYTE;
		channels       = 3;
		break;
	default:
		printf("Texture format not supported!\n");
		break;
	}


	// Create texture handle.
	glGenTextures(1, &handle);


	// Set texture targets.
	switch (type) {
	case DW_2D:
		target = GL_TEXTURE_2D;
		break;
	case DW_CUBE:
		target = GL_TEXTURE_CUBE_MAP;
		break;
	default:
		printf("Only TEXTURE_2D and TEXTURE_CUBE are supported!\n");
		break;
	}

	// Bind texture.
	glBindTexture(target, handle);
	glTexStorage2D(target, opts.mipmaps, internalFormat, width, height);

	// Set texture filtering.
	switch (opts.filter) {
	case DW_TRILINEAR:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_BILINEAR:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_LINEAR:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case DW_NEAREST:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	default:
		printf("Texture filtering must be NEAREST, LINEAR, BILINEAR or TRILINEAR!\n");
		break;
	}


	// Set texture wrap modes.
	switch (opts.wrap) {
	case DW_REPEAT:
		glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case DW_CLAMP:
		glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	default:
		printf("Texture wrap mode must be REPEAT of CLAMP!\n");
		break;
	}
}

void Texture::uploadSubData (int x, int y, int s, const char * imageData, int width, int height) {

	// Set upload target type.
	int uploadTarget;
	switch (type) {
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

	// Upload data.
	if (opts.compressed) {
		glCompressedTexSubImage2D(uploadTarget, 0, x, y, width, height, dataFormat, channels * width * height, imageData);
	} else {
		glTexSubImage2D(uploadTarget, 0, x, y, width, height, dataFormat, dataType, imageData);
	}
}
