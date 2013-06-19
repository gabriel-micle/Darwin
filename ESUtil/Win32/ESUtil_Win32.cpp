
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
LRESULT WINAPI WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ESContext * esContext = (ESContext *) (LONG_PTR) GetWindowLongPtr(hWnd, GWL_USERDATA);
	POINT		p;


	switch (uMsg) {

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
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


	HINSTANCE hInstance = GetModuleHandle(NULL);

	LPCSTR className = "OpenGLES3.0";

	WNDCLASS wndclass = {0};
	wndclass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc   = (WNDPROC) WndProc;
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = className; 

	if (!RegisterClass(&wndclass)) { 
		return FALSE;
	}

	// Adjust window rectangle so that the client area has the correct number of pixels.
	RECT windowRect;
	windowRect.left   = esContext->m_positionX;
	windowRect.top    = esContext->m_positionY;
	windowRect.right  = esContext->m_positionX + esContext->m_width;
	windowRect.bottom = esContext->m_positionY + esContext->m_height;

	DWORD wStyle = WS_POPUP | WS_BORDER | WS_VISIBLE | WS_SYSMENU | WS_CAPTION;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->m_hWnd = CreateWindow(
		className,
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

	// Set the ESContext* to the GWL_USERDATA so that it is available to the WndProc.
	SetWindowLongPtr(esContext->m_hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext);


	if (esContext->m_hWnd == NULL) {
		return GL_FALSE;
	}

	ShowWindow(esContext->m_hWnd, TRUE);

	return GL_TRUE;
}



const int TICKS_PER_SECOND = 100;
const int SKIP_TICKS       = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP    = 5;


// Start main windows loop.
void WinLoop (ESContext * esContext) {

	MSG   msg      = {0};
	bool  done     = false;
	DWORD lastTime = GetTickCount();

	DWORD next_game_tick = GetTickCount();
	int loops;

	while (!done) {

		if (GetActiveWindow() == esContext->m_hWnd) {

			loops = 0;
			while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {

				DWORD currTime  = GetTickCount();
				float deltaTime = (float) (currTime - lastTime) / 1000.0f;

				lastTime = currTime;

				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						done = true;

					} else {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

				}

				// Call update function if registered.
				if (esContext && esContext->m_pIdleFunc) {
					esContext->m_pIdleFunc(esContext, deltaTime);
				}

				next_game_tick += SKIP_TICKS;
				loops++;

			}

			// Call draw function.
			if (esContext && esContext->m_pDisplayFunc) {
				esContext->m_pDisplayFunc(esContext);
			}
		}
	}
}
