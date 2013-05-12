
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
#include <Windowsx.h>

#include "../ESUtil.h"
#include <cstdio>

// Main window procedure.
LRESULT WINAPI ESWindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ESContext * esContext = (ESContext *) (LONG_PTR) GetWindowLongPtr(hWnd, GWL_USERDATA);
	POINT		p;


	switch (uMsg) {

	case WM_CREATE:
		break;

	case WM_PAINT:
		if (esContext && esContext->m_pDisplayFunc) {
			esContext->m_pDisplayFunc(esContext);
		}
		ValidateRect(esContext->m_hWnd, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		//ExitProcess(0);
		break;


	case WM_KEYDOWN:
		GetCursorPos(&p);
		if (esContext && esContext->m_pKeyboardFunc) {
			esContext->m_pKeyboardFunc(esContext, (unsigned char) wParam, (int) p.x, (int) p.y);
		}
		break;

	case WM_KEYUP:
		GetCursorPos(&p);
		if (esContext && esContext->m_pKeyboardUpFunc) {
			esContext->m_pKeyboardUpFunc(esContext, (unsigned char) wParam, (int) p.x, (int) p.y);
		}
		break;

	case WM_LBUTTONDOWN:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_LEFT_BUTTON, ES_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_LBUTTONUP:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_LEFT_BUTTON, ES_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_RBUTTONDOWN:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_RIGHT_BUTTON, ES_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_RBUTTONUP:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_RIGHT_BUTTON, ES_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_MBUTTONDOWN:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_MIDDLE_BUTTON, ES_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_MBUTTONUP:
		if (esContext && esContext->m_pMouseFunc) {
			esContext->m_pMouseFunc(esContext, ES_MIDDLE_BUTTON, ES_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;

	case WM_MOUSEMOVE:
		if (wParam == 0) {
			if (esContext && esContext->m_pPassiveMotionFunc) {
				esContext->m_pPassiveMotionFunc(esContext, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			} 
		} else {
			if (esContext && esContext->m_pMotionFunc) {
				esContext->m_pMotionFunc(esContext, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}

		}
		break;

	default:
		break;
	} 

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
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
	windowRect.left   = esContext->m_positionX;
	windowRect.top    = esContext->m_positionY;
	windowRect.right  = esContext->m_positionX + esContext->m_width;
	windowRect.bottom = esContext->m_positionY + esContext->m_height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->m_hWnd = CreateWindow(
		"opengles3.0",
		title,
		wStyle,
		esContext->m_positionX,
		esContext->m_positionY,
		esContext->m_width,
		esContext->m_height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the ESWindowProc.
	SetWindowLongPtr(esContext->m_hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext);


	if (esContext->m_hWnd == NULL) {
		return GL_FALSE;
	}

	ShowWindow(esContext->m_hWnd, TRUE);

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
			SendMessage(esContext->m_hWnd, WM_PAINT, 0, 0);
		}

		// Call update function if registered
		if (esContext->m_pIdleFunc != NULL)
			esContext->m_pIdleFunc(esContext, deltaTime);
	}
}
