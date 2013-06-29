
/*
* Adapted from:
* ------------
* Book:      OpenGL(R) ES 2.0 Programming Guide
* Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
* ISBN-10:   0321502795
* ISBN-13:   9780321502797
* Publisher: Addison-Wesley Professional
* URLs:      http://safari.informit.com/9780321563835
*            http://www.opengles-book.com
*/


// Includes.
#include <cstdio>
#include <cstdlib>

#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include "ESDevice.h"
#include "Win32/ESDevice_Win.h"

ESDevice * ESDevice::m_pInstance = NULL;

void ESDevice::CreateInstance () {

	// Check multisampling.
	EGLint sampleBuffers = 0;
	EGLint numSamples = 0;
	if (m_flags & ES_MULTISAMPLE) {
		sampleBuffers = 1;
		if (m_flags & ES_SAMPLES_16) {
			numSamples = 16;
		} else if (m_flags & ES_SAMPLES_8) {
			numSamples = 8;
		} else if (m_flags & ES_SAMPLES_4) {
			numSamples = 4;
		} else if (m_flags & ES_SAMPLES_2) {
			numSamples = 2;
		}
	}

void ESDevice::DestroyInstance () {

	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

// Create a window.
GLboolean ESDevice::CreateDisplay (const char * windowTitle, const ESContextParams & escp) {

	m_width     = escp.windowWidth;
	m_height    = escp.windowHeight;
	m_positionX = escp.windowPositionX;
	m_positionY = escp.windowPositionY;

	
	EGLint configAttribs [] = {
		EGL_RED_SIZE,       (escp.redSize     != -1) ? escp.redSize     : EGL_DONT_CARE,
		EGL_GREEN_SIZE,     (escp.greenSize   != -1) ? escp.greenSize   : EGL_DONT_CARE,
		EGL_BLUE_SIZE,      (escp.blueSize    != -1) ? escp.blueSize    : EGL_DONT_CARE,
		EGL_ALPHA_SIZE,     (escp.alphaSize   != -1) ? escp.alphaSize   : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (escp.alphaSize	  != -1) ? escp.depthSize   : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (escp.stencilSize != -1) ? escp.stencilSize : EGL_DONT_CARE,
		EGL_NONE
	};

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, escp.esVersion,
		EGL_NONE, EGL_NONE
	};
	
	EGLBoolean bRet;

	bRet = CreateWin32(this, windowTitle); 
	if (!bRet) {
		return GL_FALSE;
	}

	bRet = CreateEGLContext(configAttribs, contextAttribs);
	if (!bRet) {
		return GL_FALSE;
	}

	return true;
}


// Start the main loop for the OpenGL ES application.
void ESDevice::Run () {

	WinLoop(this);
}

// Swap the buffers.
void ESDevice::SwapBuffers () {

	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}




// Creates an EGL rendering context and all associated elements.
GLboolean ESDevice::CreateEGLContext (EGLint configAttribs [], EGLint contextAttribs []) {

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

	// Get Display.
	m_eglDisplay = eglGetDisplay(m_eglNativeDisplay);
	if (m_eglDisplay == EGL_NO_DISPLAY) {
		return GL_FALSE;
	}

	// Initialize EGL.
	if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion)) {
		return GL_FALSE;
	}

	// Choose config.
	if (!eglChooseConfig(m_eglDisplay, configAttribs, &m_eglConfig, 1, &numConfigs)) {
		return GL_FALSE;
	}

	// Create a surface.
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglNativeWindow, NULL);
	if (m_eglSurface == EGL_NO_SURFACE) {
		return GL_FALSE;
	}

	// Create a GL context.
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttribs);
	if (m_eglContext == EGL_NO_CONTEXT) {
		return GL_FALSE;
	}   

	// Make the context current.
	if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE) {
		return GL_FALSE;
	}

	return GL_TRUE;
} 


// Log a message to the debug output for the platform.
void esLogMessage (const char * formatStr, ...) {

	va_list params;
	char buf[BUFSIZ];

	va_start(params, formatStr);
	vsprintf_s(buf, sizeof(buf), formatStr, params);

	printf("%s", buf);

	va_end(params);
}