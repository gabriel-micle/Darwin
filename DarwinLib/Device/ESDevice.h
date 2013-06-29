
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
	int		stencilSize;

};


class ESDevice {

	friend LRESULT		WINAPI WndProc  (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend GLboolean	CreateWin32		(ESDevice * esDevice, const char * title);
	friend void			WinLoop			(ESDevice * esDevice);

// Private members.
private:

	static ESDevice *		m_pInstance;

	// Window parameters.
	GLint					m_positionX;
	GLint					m_positionY;
	GLint					m_width;
	GLint					m_height;

	// Native types.
	EGLNativeWindowType		m_eglNativeWindow;
	EGLNativeDisplayType	m_eglNativeDisplay;

	EGLDisplay				m_eglDisplay;
	EGLContext				m_eglContext;
	EGLSurface				m_eglSurface;
	EGLConfig				m_eglConfig;

	// Callbacks.
	void					(* m_pDrawFunc)			 ();
	void					(* m_pUpdateFunc)		 (float);
	void					(* m_pMouseEventFunc)	 (const MouseEvent &);
	void					(* m_pKeyboardEventFunc) (const KeyboardEvent &);



// Public methods.
public:

	static void				CreateInstance  ();
	static void				DestroyInstance ();

	inline
	static ESDevice *		GetInstance () { return m_pInstance; }


	GLboolean				CreateDisplay (const char *, const ESContextParams &);

	void					Run ();

	void					SwapBuffers ();

	void					SetDrawFunc   (void (* drawFunc) ()) { m_pDrawFunc = drawFunc; }
	void					SetUpdateFunc (void (* updateFunc) (float)) { m_pUpdateFunc = updateFunc; }

	void					SetMouseEventFunc    (void (* mouseEventFunc) (const MouseEvent &)) { m_pMouseEventFunc = mouseEventFunc; }
	void					SetKeyboardEventFunc (void (* keyboardEventFunc) (const KeyboardEvent &)) { m_pKeyboardEventFunc = keyboardEventFunc; }

	inline int				GetWindowWidth     () const { return m_width; }
	inline int				GetWindowHeight    () const { return m_height; }
	inline int				GetWindowPositionX () const { return m_positionX; }
	inline int				GetWindowPositionY () const { return m_positionY; }


// Private methods.
private:

	// Constructor.
							ESDevice () {};
	virtual					~ESDevice () {};

	// Create an EGL context.
	GLboolean				CreateEGLContext (EGLint configAttribs [], EGLint contextAttribs []);

}; // ESDevice.


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
