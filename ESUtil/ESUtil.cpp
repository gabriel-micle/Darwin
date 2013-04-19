
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
#include <stdio.h>
#include <stdlib.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include "ESUtil.h"
#include "Win32/ESUtil_Win.h"



// Creates an EGL rendering context and all associated elements.
EGLBoolean CreateEGLContext (EGLNativeWindowType hWnd, EGLDisplay * eglDisplay,
							 EGLContext * eglContext, EGLSurface * eglSurface,
							 EGLint attribList[])
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig  config;

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE };

	// Get Display.
	display = eglGetDisplay(GetDC(hWnd));
	if (display == EGL_NO_DISPLAY) {
		return EGL_FALSE;
	}

	// Initialize EGL.
	if (!eglInitialize(display, &majorVersion, &minorVersion)) {
		return EGL_FALSE;
	}

	// Get configs.
	if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
		return EGL_FALSE;
	}

	// Choose config.
	if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs)) {
		return EGL_FALSE;
	}

	// Create a surface.
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType) hWnd, NULL);
	if (surface == EGL_NO_SURFACE) {
		return EGL_FALSE;
	}

	// Create a GL context.
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		return EGL_FALSE;
	}   

	// Make the context current.
	if (!eglMakeCurrent(display, surface, surface, context)) {
		return EGL_FALSE;
	}

	* eglDisplay = display;
	* eglSurface = surface;
	* eglContext = context;

	return EGL_TRUE;
} 


ESContext::ESContext () {

	positionX = 0;
	positionY = 0;

	width  = 640;
	height = 480;

	flags = ES_WINDOW_RGB;

	this->idleFunc          = NULL;
	this->displayFunc       = NULL;
	this->keyboardFunc      = NULL;
	this->keyboardUpFunc    = NULL;
	this->mouseFunc         = NULL;
	this->motionFunc        = NULL;
	this->passiveMotionFunc = NULL;
}


// Create a window.
GLboolean ESUTIL_API ESContext::esCreateWindow (const char * title) {

	EGLint attribList[] = {
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};

	if (!WinCreate (this, title)) {
		return GL_FALSE;
	}

	if (!CreateEGLContext(hWnd, &eglDisplay, &eglContext, &eglSurface, attribList)) {
		return GL_FALSE;
	}

	return GL_TRUE;
}


// Start the main loop for the OpenGL ES application.
void ESUTIL_API ESContext::esMainLoop () {

	WinLoop(this);
}

// Swap the buffers.
void ESUTIL_API ESContext::esSwapBuffers () {

	eglSwapBuffers(eglDisplay, eglSurface);
}

// Set initial window position.
void ESUTIL_API ESContext::esInitWindowPosition (GLint x, GLint y) {

	positionX = x;
	positionY = y;
}

// Set initial window size.
void ESUTIL_API ESContext::esInitWindowSize (GLint w, GLint h) {

	width  = w;
	height = h;
}

// Specifies the buffer initialization mode: 
// - ES_WINDOW_RGB			- specifies that the color buffer should have R, G, B channels.
// - ES_WINDOW_ALPHA		- specifies that the color buffer should have alpha.
// - ES_WINDOW_DEPTH		- specifies that a depth buffer should be created.
// - ES_WINDOW_STENCIL		- specifies that a stencil buffer should be created.
// - ES_WINDOW_MULTISAMPLE	- specifies that a multi-sample buffer should be created.
void ESUTIL_API ESContext::esInitDisplayMode (GLuint mode) {

	flags = mode;
}


// Set display function.
void ESUTIL_API ESContext::esDisplayFunc (void (ESCALLBACK * displayFunc) (ESContext *)) {

	this->displayFunc = displayFunc;
}

// Set idle function.
void ESUTIL_API ESContext::esIdleFunc (void (ESCALLBACK * idleFunc) (ESContext *, float)) {

	this->idleFunc = idleFunc;
}

// Set keyboard function.
void ESUTIL_API ESContext::esKeyboardFunc (void (ESCALLBACK * keyboardFunc) (ESContext *, unsigned char, int, int)) {

	this->keyboardFunc = keyboardFunc;
}

// Set keyboard up function.
void ESUTIL_API ESContext::esKeyboardUpFunc (void (ESCALLBACK * keyboardUpFunc) (ESContext *, unsigned char, int, int)) {

	this->keyboardUpFunc = keyboardUpFunc;
}

// Set mouse button function.
void ESUTIL_API ESContext::esMouseFunc (void (ESCALLBACK * mouseFunc) (ESContext *, int, int, int, int)) {

	this->mouseFunc = mouseFunc;
}

// Set mouse movement when mouse button pressed function.
void ESUTIL_API ESContext::esMotionFunc (void (ESCALLBACK * motionFunc) (ESContext *, int, int)) {

	this->motionFunc = motionFunc;
}

// Set mouse movement function.
void ESUTIL_API ESContext::esPassiveMotionFunc (void (ESCALLBACK * passiveMotionFunc) (ESContext *, int, int)) {

	this->passiveMotionFunc = passiveMotionFunc;
}


// Log a message to the debug output for the platform.
void ESUTIL_API esLogMessage (const char * formatStr, ...) {

	va_list params;
	char buf[BUFSIZ];

	va_start(params, formatStr);
	vsprintf_s(buf, sizeof(buf), formatStr, params);

	printf("%s", buf);

	va_end(params);
}


// Loads a 24-bit TGA image from a file.
char * ESUTIL_API esLoadTGA (char * fileName, int * width, int * height) {

	char * buffer;

	if (WinTGALoad(fileName, &buffer, width, height)) {
		return buffer;
	}

	return NULL;
}
