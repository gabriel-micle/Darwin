
#include "Headers.h"

// Handle to a program object
GLuint programObject;

GLfloat vVertices[] = {  
	0.0f,  0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f 
};

GLuint vbo;
GLuint vao;

///
// Initialize the shader and program object
//
void Init (ESContext *esContext) {

	// Display OpenGL version.
	const GLubyte * version = glGetString(GL_VERSION);
	printf("%s\n", version);

	char * vShaderStr = ReadFile("./Shaders/Hello.vert.glsl");
	char * fShaderStr = ReadFile("./Shaders/Hello.frag.glsl");

	// Create the program object.
	programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Create vertex buffer.
	glGenBuffers(1, &vbo);

	// Load vertex position data into buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Get position location.
	glUseProgram(programObject);
	GLuint loc = glGetAttribLocation(programObject, "inPosition");

	// Create vertex array.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(loc);

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	ReadWavefrontOBJ("./Data/Models/Batman.obj");

}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{

	// Set the viewport
	glViewport ( 0, 0, esContext->width, esContext->height );

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );

	// Use the program object
	glUseProgram (programObject);

	glDrawArrays ( GL_TRIANGLES, 0, 3 );

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}


int main (int argc, char * argv[]) {

	ESContext esContext;

	esInitContext (&esContext);

	esCreateWindow (&esContext, "Hello Triangle", 300, 300, 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_ALPHA);

	Init ( &esContext );

	esDisplayFunc (&esContext, Draw);

	esMainLoop (&esContext);
}
