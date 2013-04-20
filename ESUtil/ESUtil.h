
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


#ifndef _ESUTIL_H_
#define _ESUTIL_H_


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
	GLint		positionX;

	// Window y position.
	GLint		positionY;

	// Window width.
	GLint       width;

	// Window height.
	GLint       height;

	// Buffer flags.
	GLuint		flags;

	// Window handle.
	EGLNativeWindowType  hWnd;

	// EGL display.
	EGLDisplay  eglDisplay;

	// EGL context.
	EGLContext  eglContext;

	// EGL surface.
	EGLSurface  eglSurface;

	// EGL config.
	EGLConfig	eglConfig;

	// Callbacks.
	void (ESCALLBACK * displayFunc)			(ESContext *);
	void (ESCALLBACK * idleFunc)			(ESContext *, float);
	void (ESCALLBACK * keyboardFunc)		(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * keyboardUpFunc)		(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * mouseFunc)			(ESContext *, int, int, int, int);
	void (ESCALLBACK * motionFunc)			(ESContext *, int, int);
	void (ESCALLBACK * passiveMotionFunc)	(ESContext *, int, int);
	
	// ESContext constructor.
	ESContext ();

	// Create a window with the specified title.
	// Returns GL_TRUE if window creation is succesful, GL_FALSE otherwise.
	GLboolean ESUTIL_API esCreateWindow (const char * title);

	// Start the main loop for the OpenGL ES application.
	void ESUTIL_API esMainLoop ();

	// Register a draw callback function to be used to render each frame.
	void ESUTIL_API esDisplayFunc (void (ESCALLBACK * drawFunc) (ESContext *));

	// Register an update callback function to be used to update on each time step.
	void ESUTIL_API esIdleFunc (void (ESCALLBACK * updateFunc) (ESContext *, float));

	// Register an keyboard input processing callback function.
	void ESUTIL_API esKeyboardFunc (void (ESCALLBACK * keyFunc) (ESContext *, unsigned char, int, int));

	// Register a keyboard release processing callback function.
	void ESUTIL_API esKeyboardUpFunc (void (ESCALLBACK * keyUpFunc) (ESContext *, unsigned char, int, int));

	// Register a mouse press callback function.
	void ESUTIL_API esMouseFunc (void (ESCALLBACK * mouseFunc) (ESContext *, int, int, int, int));

	// Register a mouse movement when mouse button pressed callback function.
	void ESUTIL_API esMotionFunc (void (ESCALLBACK * motionFunc) (ESContext *, int, int));

	// Register a mouse movement callback function.
	void ESUTIL_API esPassiveMotionFunc (void (ESCALLBACK * passiveMotionFunc) (ESContext *, int, int));

	// Swaps buffers.
	void ESUTIL_API esSwapBuffers ();

	// Set initial window size.
	void ESUTIL_API esInitWindowSize (GLint width, GLint height);

	// Set initial window position.
	void ESUTIL_API esInitWindowPosition (GLint x, GLint y);

	// Specifies the buffer initialization mode:
	// - ES_WINDOW_RGB			- specifies that the color buffer should have R, G, B channels.
	// - ES_WINDOW_ALPHA		- specifies that the color buffer should have alpha.
	// - ES_WINDOW_DEPTH		- specifies that a depth buffer should be created.
	// - ES_WINDOW_STENCIL		- specifies that a stencil buffer should be created.
	// - ES_WINDOW_MULTISAMPLE	- specifies that a multi-sample buffer should be created.
	void ESUTIL_API esInitDisplayMode (GLuint mode);

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


// Loads a 24-bit TGA image from a file.
char * ESUTIL_API esLoadTGA (char * fileName, int * width, int * height );


#endif
