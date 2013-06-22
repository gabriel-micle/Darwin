
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



// Creates an EGL rendering context and all associated elements.
EGLBoolean CreateEGLContext (EGLNativeWindowType eglNativeWindow,
							 EGLDisplay * eglDisplay,
							 EGLContext * eglContext, 
							 EGLSurface * eglSurface,
							 EGLConfig * eglConfig, 
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
	display = eglGetDisplay(GetDC(eglNativeWindow));
	if (display == EGL_NO_DISPLAY) {
		return EGL_FALSE;
	}

	// Initialize EGL.
	if (!eglInitialize(display, &majorVersion, &minorVersion)) {
		return EGL_FALSE;
	}

	/*
	// Get configs.
	if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
		return EGL_FALSE;
	}
	*/

	// Choose config.
	if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs)) {
		return EGL_FALSE;
	}

	// Create a surface.
	surface = eglCreateWindowSurface(display, config, eglNativeWindow, NULL);
	if (surface == EGL_NO_SURFACE) {
		return EGL_FALSE;
	}


	// Create a GL context.
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		return EGL_FALSE;
	}   

	// Make the context current.
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		return EGL_FALSE;
	}

	* eglDisplay = display;
	* eglSurface = surface;
	* eglContext = context;
	* eglConfig  = config;

	return EGL_TRUE;
} 


ESContext::ESContext () :

	m_positionX			(0),
	m_positionY			(0),
	m_width				(640),
	m_height			(480),

	m_flags				(ES_RGB),

	m_pIdleFunc			(NULL),
	m_pDisplayFunc		(NULL),
	m_pMouseEventFunc	(NULL),
	m_pKeyboardEventFunc(NULL)

{}


// Create a window.
GLboolean ESUTIL_API ESContext::CreateDisplay (const char * title) {

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

	EGLint attribList[] = {
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

	if (!WinCreate (this, title)) {
		return GL_FALSE;
	}

	if (!CreateEGLContext(m_hWnd, &m_eglDisplay, &m_eglContext, &m_eglSurface, &m_eglConfig, attribList)) {
		return GL_FALSE;
	}

	return GL_TRUE;
}


// Start the main loop for the OpenGL ES application.
void ESUTIL_API ESContext::MainLoop () {

	WinLoop(this);
}

// Swap the buffers.
void ESUTIL_API ESContext::SwapBuffers () {

	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

// Set initial window position.
void ESUTIL_API ESContext::InitDisplayPosition (GLint x, GLint y) {

	m_positionX = x;
	m_positionY = y;
}

// Set initial window size.
void ESUTIL_API ESContext::InitDisplaySize (GLint w, GLint h) {

	m_width  = w;
	m_height = h;
}

// Specifies the buffer initialization mode: 
// - ES_WINDOW_RGB			- specifies that the color buffer should have R, G, B channels.
// - ES_WINDOW_ALPHA		- specifies that the color buffer should have alpha.
// - ES_WINDOW_DEPTH		- specifies that a depth buffer should be created.
// - ES_WINDOW_STENCIL		- specifies that a stencil buffer should be created.
// - ES_WINDOW_MULTISAMPLE	- specifies that a multi-sample buffer should be created.
void ESUTIL_API ESContext::InitDisplayMode (GLuint mode) {

	m_flags = mode;
}


// Set display function.
void ESUTIL_API ESContext::DisplayFunc (void (ESCALLBACK * displayFunc) (ESContext *)) {

	this->m_pDisplayFunc = displayFunc;
}

// Set idle function.
void ESUTIL_API ESContext::IdleFunc (void (ESCALLBACK * idleFunc) (ESContext *, float)) {

	this->m_pIdleFunc = idleFunc;
}


// Register callback function that handles all mouse events.
void ESUTIL_API ESContext::MouseEventFunc (void (ESCALLBACK * mouseEventFunc) (ESContext *, const MouseEvent &)) {

	this->m_pMouseEventFunc = mouseEventFunc;
}

// Register callback function that handles all keyboard events.
void ESUTIL_API ESContext::KeyboardEventFunc (void (ESCALLBACK * keyboardEventFunc) (ESContext *, const KeyboardEvent &)) {

	this->m_pKeyboardEventFunc = keyboardEventFunc;
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
