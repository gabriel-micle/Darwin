
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


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "esUtil.h"

// Main window procedure.
LRESULT WINAPI ESWindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ESContext * esContext = (ESContext *) (LONG_PTR) GetWindowLongPtr(hWnd, GWL_USERDATA);
	POINT		p;

	LRESULT lRet = 1;

	if (esContext == NULL) {
		return lRet;
	}

	switch (uMsg) {

	case WM_CREATE:
		break;

	case WM_PAINT:
		if (esContext->displayFunc) {
			esContext->displayFunc(esContext);
		}
		ValidateRect(esContext->hWnd, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);             
		break; 

	case WM_KEYDOWN:
		GetCursorPos(&p);
		if (esContext->keyboardFunc) {
			esContext->keyboardFunc(esContext, (unsigned char) wParam, (int) p.x, (int) p.y);
		}
		break;

	case WM_KEYUP:
		GetCursorPos(&p);
		if (esContext->keyboardUpFunc) {
			esContext->keyboardUpFunc(esContext, (unsigned char) wParam, (int) p.x, (int) p.y);
		}
		break;

	case WM_LBUTTONDOWN:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_LEFT_BUTTON, ES_DOWN, (int) p.x, (int) p.y);
		}
		break;

	case WM_LBUTTONUP:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_LEFT_BUTTON, ES_UP, (int) p.x, (int) p.y);
		}
		break;

	case WM_LBUTTONDBLCLK:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_LEFT_BUTTON, ES_DOUBLE_CLICK, (int) p.x, (int) p.y);
		}
		break;

	case WM_RBUTTONDOWN:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_RIGHT_BUTTON, ES_DOWN, (int) p.x, (int) p.y);
		}
		break;

	case WM_RBUTTONUP:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_RIGHT_BUTTON, ES_UP, (int) p.x, (int) p.y);
		}
		break;

	case WM_RBUTTONDBLCLK:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_RIGHT_BUTTON, ES_DOUBLE_CLICK, (int) p.x, (int) p.y);
		}
		break;

	case WM_MBUTTONDOWN:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_MIDDLE_BUTTON, ES_DOWN, (int) p.x, (int) p.y);
		}
		break;

	case WM_MBUTTONUP:
		GetCursorPos(&p);
		if (esContext && esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_MIDDLE_BUTTON, ES_UP, (int) p.x, (int) p.y);
		}
		break;

	case WM_MBUTTONDBLCLK:
		GetCursorPos(&p);
		if (esContext->mouseFunc) {
			esContext->mouseFunc(esContext, ES_MIDDLE_BUTTON, ES_DOUBLE_CLICK, (int) p.x, (int) p.y);
		}
		break;


	case WM_MOUSEMOVE:
		GetCursorPos(&p);
		if (wParam == 0) {
			if (esContext->passiveMotionFunc) {
				esContext->passiveMotionFunc(esContext, (int) p.x, (int) p.x);
			} 
		} else {
			if (esContext->motionFunc) {
				esContext->motionFunc(esContext, (int) p.x, (int) p.y);
			}

		}
		break;

	case WM_TOUCH:
		GetCursorPos(&p);
		if (esContext && esContext->mouseFunc) {
			esContext->mouseFunc(esContext, 700, 800, (int) p.x, (int) p.y);
		}
		break;

	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	} 

	return lRet; 
}



// Create Win32 instance and window.
GLboolean WinCreate (ESContext * esContext, const char * title) {

	WNDCLASS  wndclass = {0}; 
	DWORD	  wStyle   = 0;
	RECT	  windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC) ESWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = "opengles3.0"; 

	if (!RegisterClass(&wndclass)) { 
		return FALSE;
	}

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust window rectangle so that the client area has the correct number of pixels.
	windowRect.left   = esContext->positionX;
	windowRect.top    = esContext->positionY;
	windowRect.right  = esContext->positionX + esContext->width;
	windowRect.bottom = esContext->positionY + esContext->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->hWnd = CreateWindow(
		"opengles3.0",
		title,
		wStyle,
		esContext->positionX,
		esContext->positionY,
		esContext->width,
		esContext->height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the ESWindowProc.
	SetWindowLongPtr(esContext->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext);


	if (esContext->hWnd == NULL) {
		return GL_FALSE;
	}

	ShowWindow(esContext->hWnd, TRUE);

	return GL_TRUE;
}


// Start main windows loop.
void WinLoop (ESContext * esContext) {

	MSG   msg      = {0};
	bool  done     = false;
	DWORD lastTime = GetTickCount();

	while (!done) {

		int   gotMsg    = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD currTime  = GetTickCount();
		float deltaTime = (float)(currTime - lastTime) / 1000.0f;

		lastTime = currTime;

		if (gotMsg) {
			if (msg.message == WM_QUIT) {
				done = true;

			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		} else {
			SendMessage(esContext->hWnd, WM_PAINT, 0, 0);
		}

		// Call update function if registered
		if (esContext->idleFunc != NULL)
			esContext->idleFunc(esContext, deltaTime);
	}
}
