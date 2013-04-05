//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

//
/// \file ESUtil.h
/// \brief A utility library for OpenGL ES.  This library provides a
///        basic common framework for the example applications in the
///        OpenGL ES 2.0 Programming Guide.
//
#ifndef ESUTIL_H
#define ESUTIL_H

///
//  Includes
//
#include <GLES3/gl3.h>
#include <EGL/egl.h>

#ifdef __cplusplus

extern "C" {
#endif


	///
	//  Macros
	//
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl


	/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
	/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
	/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
	/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
	/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8


	///
	// Types
	//


	typedef struct
	{
		/// Window x position;
		GLint		positionX;

		/// Window y position;
		GLint		positionY;

		/// Window width
		GLint       width;

		/// Window height
		GLint       height;

		/// Window handle
		EGLNativeWindowType  hWnd;

		/// EGL display
		EGLDisplay  eglDisplay;

		/// EGL context
		EGLContext  eglContext;

		/// EGL surface
		EGLSurface  eglSurface;

		/// Callbacks
		void (ESCALLBACK * drawFunc) (void *);
		void (ESCALLBACK * keyFunc) (void *, unsigned char, int, int);
		void (ESCALLBACK * keyUpFunc) (void *, unsigned char, int, int);
		void (ESCALLBACK * idleFunc) (void *, float deltaTime);
	} ESContext;


	///
	//  Public Functions
	//

	//
	///
	/// \brief Initialize ES framework context.  This must be called before calling any other functions.
	/// \param esContext Application context
	//
	void ESUTIL_API esInitContext ( ESContext *esContext );

	//
	/// \brief Create a window with the specified parameters
	/// \param esContext Application context
	/// \param title Name for title bar of window
	/// \param x X coordinate of upper left corner position
	/// \param y Y coordinate of upper left corner position
	/// \param width Width in pixels of window to create
	/// \param height Height in pixels of window to create
	/// \param flags Bitfield for the window creation flags 
	///         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
	///         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
	///         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
	///         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
	///         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
	/// \return GL_TRUE if window creation is succesful, GL_FALSE otherwise
	GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint x, GLint y, GLint width, GLint height, GLuint flags );

	//
	/// \brief Start the main loop for the OpenGL ES application
	/// \param esContext Application context
	//
	void ESUTIL_API esMainLoop ( ESContext *esContext );

	//
	/// \brief Register a draw callback function to be used to render each frame
	/// \param esContext Application context
	/// \param drawFunc Draw callback function that will be used to render the scene
	//
	void ESUTIL_API esDisplayFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) ( ESContext* ) );

	//
	/// \brief Register an update callback function to be used to update on each time step
	/// \param esContext Application context
	/// \param updateFunc Update callback function that will be used to render the scene
	//
	void ESUTIL_API esIdleFunc ( ESContext *esContext, void (ESCALLBACK *updateFunc) ( ESContext*, float ) );

	//
	/// \brief Register an keyboard input processing callback function
	/// \param esContext Application context
	/// \param keyFunc Key callback function for application processing of keyboard input
	//
	void ESUTIL_API esKeyboardFunc ( ESContext *esContext, 
		void (ESCALLBACK *keyFunc) ( ESContext*, unsigned char, int, int ) );

	//
	/// \brief Register a keyboard release processing callback function
	/// \param esContext Application context
	/// \param keyUpFunc Key callback function for application processing of keyboard input
	//
	void ESUTIL_API esKeyboardUpFunc ( ESContext *esContext, 
		void (ESCALLBACK *keyUpFunc) ( ESContext*, unsigned char, int, int ) );




	//
	/// \brief Log a message to the debug output for the platform
	/// \param formatStr Format string for error log.  
	//
	void ESUTIL_API esLogMessage ( const char *formatStr, ... );

	//
	///
	/// \brief Load a shader, check for compile errors, print error messages to output log
	/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	/// \param shaderSrc Shader source string
	/// \return A new shader object on success, 0 on failure
	//
	GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc );

	//
	///
	/// \brief Load a vertex and fragment shader, create a program object, link program.
	///        Errors output to log.
	/// \param vertShaderSrc Vertex shader source code
	/// \param fragShaderSrc Fragment shader source code
	/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
	//
	GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );


	//
	/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores 
	///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
	/// \param numSlices The number of slices in the sphere
	/// \param vertices If not NULL, will contain array of float3 positions
	/// \param normals If not NULL, will contain array of float3 normals
	/// \param texCoords If not NULL, will contain array of float2 texCoords
	/// \param indices If not NULL, will contain the array of indices for the triangle strip
	/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
	///         if it is not NULL ) as a GL_TRIANGLE_STRIP
	//
	int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals, 
		GLfloat **texCoords, GLuint **indices );

	//
	/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
	///        the results in the arrays.  Generate index list for a TRIANGLES
	/// \param scale The size of the cube, use 1.0 for a unit cube.
	/// \param vertices If not NULL, will contain array of float3 positions
	/// \param normals If not NULL, will contain array of float3 normals
	/// \param texCoords If not NULL, will contain array of float2 texCoords
	/// \param indices If not NULL, will contain the array of indices for the triangle strip
	/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
	///         if it is not NULL ) as a GL_TRIANGLES
	//
	int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals, 
		GLfloat **texCoords, GLuint **indices );

	//
	/// \brief Loads a 24-bit TGA image from a file
	/// \param fileName Name of the file on disk
	/// \param width Width of loaded image in pixels
	/// \param height Height of loaded image in pixels
	///  \return Pointer to loaded image.  NULL on failure. 
	//
	char* ESUTIL_API esLoadTGA ( char *fileName, int *width, int *height );


#ifdef __cplusplus
}
#endif

#endif // ESUTIL_H