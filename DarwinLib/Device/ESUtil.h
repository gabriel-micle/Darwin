
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


#pragma once


// Includes.
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include "../Input/Input.h"


// Macros.


struct ESContextParams {

	int		esVersion;

	int		windowWidth;
	int		windowHeight;

	int		windowPositionX;
	int		windowPositionY;

	int		redSize;
	int		greenSize;
	int		blueSize;
	int		alphaSize;
	int		depthSize;
	int		samples;
	bool	stencil;

};

// Buffer modes.
#define ES_RGB				0x0000
#define ES_ALPHA			0x0001 
#define ES_DEPTH			0x0002 
#define ES_STENCIL			0x0004
#define	ES_MULTISAMPLE		0x0008
#define	ES_SAMPLES_2		0x0010
#define ES_SAMPLES_4		0x0020
#define	ES_SAMPLES_8		0x0040
#define ES_SAMPLES_16		0x0080

class ESContext {

// Public members.
public:

	// Window x position.
	GLint		m_positionX;

	// Window y position.
	GLint		m_positionY;

	// Window width.
	GLint       m_width;

	// Window height.
	GLint       m_height;

	// Buffer flags.
	GLuint		m_flags;

	// Native window.
	EGLNativeWindowType  m_eglNativeWindow;

	// Native display.
	EGLNativeDisplayType m_eglNativeDisplay;

	// EGL display.
	EGLDisplay  m_eglDisplay;

	// EGL context.
	EGLContext  m_eglContext;

	// EGL surface.
	EGLSurface  m_eglSurface;

	// EGL config.
	EGLConfig	m_eglConfig;

	// Callbacks.
	void (* m_pDrawFunc)		  (ESContext *);
	void (* m_pUpdateFunc)		  (ESContext *, float);
	void (* m_pMouseEventFunc)	  (ESContext *, const MouseEvent &);
	void (* m_pKeyboardEventFunc) (ESContext *, const KeyboardEvent &);

	static ESContext *	m_pInstance;	


// Public methods.
public:

	// Constructor.
	ESContext () {};

	// Create a window with the specified title.
	// Returns GL_TRUE if window creation is succesful, GL_FALSE otherwise.
	GLboolean CreateDisplay (const char *);

	//
	static bool CreateDisplay (const char * windowTitle, const ESContextParams & escp); 


	// Start the main loop for the OpenGL ES application.
	void Run ();

	// Swaps buffers.
	void SwapBuffers ();

	// Set initial window size.
	void InitDisplaySize (GLint width, GLint height);

	// Set initial window position.
	void InitDisplayPosition (GLint x, GLint y);

	// Specifies the buffer initialization mode.
	void InitDisplayMode (GLuint mode);

	// Register a draw callback function to be used to render each frame.
	void SetDrawFunc (void (* drawFunc) (ESContext *)) {
		m_pDrawFunc = drawFunc;
	}

	// Register an update callback function to be used to update on each time step.
	void SetUpdateFunc (void (* updateFunc) (ESContext *, float)) {
		m_pUpdateFunc = updateFunc;
	}

	// Register callback function that handles all mouse events.
	void SetMouseEventFunc (void (* mouseEventFunc) (ESContext *, const MouseEvent &)) {
		m_pMouseEventFunc = mouseEventFunc;
	}

	// Register callback function that handles all keyboard events.
	void SetKeyboardEventFunc (void (* keyboardEventFunc) (ESContext *, const KeyboardEvent &)) {
		m_pKeyboardEventFunc = keyboardEventFunc;
	}


// Private methods.
private:

	// Create an EGL context.
	GLboolean CreateEGLContext (EGLint configAttribs [], EGLint contextAttribs []);

}; // ESContext.


// Log a message to the debug output for the platform.
void esLogMessage (const char * formatStr, ...);


// Load a shader, check for compile errors, print error messages to output log.
// Type can be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
// Returns a new shader object on success or 0 on failure.
GLuint esLoadShader (GLenum type, const char * shaderSrc);


// Load a vertex and fragment shader, create a program object, link program.
// Errors output to log.
// Return a new program object linked with the vertex/fragment shader pair, 0 on failure.
GLuint esLoadProgram (const char * vertShaderSrc, const char * fragShaderSrc);
