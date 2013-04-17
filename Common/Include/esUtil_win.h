
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


#ifndef _ESUTIL_WIN_H_
#define _ESUTIL_WIN_H_


// Create Win32 instance and window
GLboolean WinCreate (ESContext * esContext, const char * title);

// Start main windows loop
void WinLoop (ESContext * esContext);

// TGA loader win32 implementation
int WinTGALoad (const char * fileName, char ** buffer, int * width, int * height);


#endif
