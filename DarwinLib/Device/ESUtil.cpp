
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

#include "ESUtil.h"
#include "Win32/ESUtil_Win.h"


// Create a window.
GLboolean ESContext::CreateDisplay (const char * windowTitle) {

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

	EGLint configAttribs [] = {
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     (m_flags & ES_ALPHA)	?  8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (m_flags & ES_DEPTH)	? 24 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (m_flags & ES_STENCIL)	?  1 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, sampleBuffers,
		EGL_SAMPLES,		16,
		EGL_NONE
	};

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE };

	EGLBoolean bRet;

	bRet = CreateWin32(this, windowTitle); 
	if (!bRet) {
		return EGL_FALSE;
	}

	bRet = CreateEGLContext(configAttribs, contextAttribs);
	if (!bRet) {
		return EGL_FALSE;
	}

	return bRet;
}


// Start the main loop for the OpenGL ES application.
void ESContext::Run () {

	WinLoop(this);
}

// Swap the buffers.
void ESContext::SwapBuffers () {

	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

// Set initial window position.
void ESContext::InitDisplayPosition (GLint x, GLint y) {

	m_positionX = x;
	m_positionY = y;
}

// Set initial window size.
void ESContext::InitDisplaySize (GLint w, GLint h) {

	m_width  = w;
	m_height = h;
}

// Specifies the buffer initialization mode: 
// - ES_WINDOW_RGB			- specifies that the color buffer should have R, G, B channels.
// - ES_WINDOW_ALPHA		- specifies that the color buffer should have alpha.
// - ES_WINDOW_DEPTH		- specifies that a depth buffer should be created.
// - ES_WINDOW_STENCIL		- specifies that a stencil buffer should be created.
// - ES_WINDOW_MULTISAMPLE	- specifies that a multi-sample buffer should be created.
void ESContext::InitDisplayMode (GLuint mode) {

	m_flags = mode;
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


// Creates an EGL rendering context and all associated elements.
GLboolean ESContext::CreateEGLContext (EGLint configAttribs [], EGLint contextAttribs []) {

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