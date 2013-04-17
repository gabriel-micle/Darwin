
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



#define ES_WINDOW_RGB			0
#define ES_WINDOW_ALPHA			1 
#define ES_WINDOW_DEPTH			2 
#define ES_WINDOW_STENCIL		4
#define ES_WINDOW_MULTISAMPLE	8



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

	// Callbacks.
	void (ESCALLBACK * drawFunc)	(ESContext *);
	void (ESCALLBACK * keyFunc)		(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * keyUpFunc)	(ESContext *, unsigned char, int, int);
	void (ESCALLBACK * idleFunc)	(ESContext *, float);
	
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

	// Register a keyboard release processing callback function
	void ESUTIL_API esKeyboardUpFunc (void (ESCALLBACK * keyUpFunc) (ESContext *, unsigned char, int, int));

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
