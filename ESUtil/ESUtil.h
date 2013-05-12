
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



// Macros.
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl


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

// Input.
#define	ES_LEFT_BUTTON		0x0000
#define ES_MIDDLE_BUTTON	0x0001
#define ES_RIGHT_BUTTON		0x0002
#define ES_DOWN				0x0000
#define ES_UP				0x0001

class ESContext {

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

	// Window handle.
	EGLNativeWindowType  m_hWnd;

	// EGL display.
	EGLDisplay  m_eglDisplay;

	// EGL context.
	EGLContext  m_eglContext;

	// EGL surface.
	EGLSurface  m_eglSurface;

	// EGL config.
	EGLConfig	m_eglConfig;

	// Callbacks.
	void (ESCALLBACK * m_pDisplayFunc)			(ESContext *);
	void (ESCALLBACK * m_pIdleFunc)			(ESContext *, float);
	void (ESCALLBACK * m_pKeyboardFunc)		(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * m_pKeyboardUpFunc)		(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * m_pMouseFunc)			(ESContext *, int, int, int, int);
	void (ESCALLBACK * m_pMotionFunc)			(ESContext *, int, int);
	void (ESCALLBACK * m_pPassiveMotionFunc)	(ESContext *, int, int);
	
	// ESContext constructor.
	ESContext ();

	// Create a window with the specified title.
	// Returns GL_TRUE if window creation is succesful, GL_FALSE otherwise.
	GLboolean ESUTIL_API CreateDisplay (const char *);

	// Start the main loop for the OpenGL ES application.
	void ESUTIL_API MainLoop ();

	// Register a draw callback function to be used to render each frame.
	void ESUTIL_API DisplayFunc (void (ESCALLBACK * drawFunc) (ESContext *));

	// Register an update callback function to be used to update on each time step.
	void ESUTIL_API IdleFunc (void (ESCALLBACK * updateFunc) (ESContext *, float));

	// Register an keyboard input processing callback function.
	void ESUTIL_API KeyboardFunc (void (ESCALLBACK * keyFunc) (ESContext *, unsigned char, int, int));

	// Register a keyboard release processing callback function.
	void ESUTIL_API KeyboardUpFunc (void (ESCALLBACK * keyUpFunc) (ESContext *, unsigned char, int, int));

	// Register a mouse press callback function.
	void ESUTIL_API MouseFunc (void (ESCALLBACK * mouseFunc) (ESContext *, int, int, int, int));

	// Register a mouse movement when mouse button pressed callback function.
	void ESUTIL_API MotionFunc (void (ESCALLBACK * motionFunc) (ESContext *, int, int));

	// Register a mouse movement callback function.
	void ESUTIL_API PassiveMotionFunc (void (ESCALLBACK * passiveMotionFunc) (ESContext *, int, int));

	// Swaps buffers.
	void ESUTIL_API SwapBuffers ();

	// Set initial window size.
	void ESUTIL_API InitDisplaySize (GLint width, GLint height);

	// Set initial window position.
	void ESUTIL_API InitDisplayPosition (GLint x, GLint y);

	// Specifies the buffer initialization mode:
	// - ES_WINDOW_RGB			- specifies that the color buffer should have R, G, B channels.
	// - ES_WINDOW_ALPHA		- specifies that the color buffer should have alpha.
	// - ES_WINDOW_DEPTH		- specifies that a depth buffer should be created.
	// - ES_WINDOW_STENCIL		- specifies that a stencil buffer should be created.
	// - ES_WINDOW_MULTISAMPLE	- specifies that a multi-sample buffer should be created.
	void ESUTIL_API InitDisplayMode (GLuint mode);

}; // ESContext.


// Log a message to the debug output for the platform.
void ESUTIL_API esLogMessage (const char * formatStr, ...);


// Load a shader, check for compile errors, print error messages to output log.
// Type can be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
// Returns a new shader object on success or 0 on failure.
GLuint ESUTIL_API esLoadShader (GLenum type, const char * shaderSrc);


// Load a vertex and fragment shader, create a program object, link program.
// Errors output to log.
// Return a new program object linked with the vertex/fragment shader pair, 0 on failure.
GLuint ESUTIL_API esLoadProgram (const char * vertShaderSrc, const char * fragShaderSrc);
