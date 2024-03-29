
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


// Create Win32 instance and window.
GLboolean CreateWin32 (ESDevice * esDevice, const char * title);

// Start main windows loop.
void WinLoop (ESDevice * esDevice);
