
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
#include "../../Input/Input.h"

#include <cstdio>

// Main window procedure.
LRESULT WINAPI WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ESContext * esContext = (ESContext *) (LONG_PTR) GetWindowLongPtr(hWnd, GWL_USERDATA);


	/***********************
	 * Handle mouse events.
	 ***********************/

	MouseEvent mouseEvent;
	memset(&mouseEvent, 0, sizeof(MouseEvent));

	switch (uMsg) {
	case WM_LBUTTONDOWN:
		mouseEvent.type = DW_LBUTTON_DOWN;
		break;
	case WM_LBUTTONUP:
		mouseEvent.type = DW_LBUTTON_UP;
		break;
	case WM_RBUTTONDOWN:
		mouseEvent.type = DW_RBUTTON_DOWN;
		break;
	case WM_RBUTTONUP:
		mouseEvent.type = DW_RBUTTON_UP;
		break;
	case WM_MBUTTONDOWN:
		mouseEvent.type = DW_MBUTTON_DOWN;
		break;
	case WM_MBUTTONUP:
		mouseEvent.type = DW_MBUTTON_UP;
		break;
	case WM_MOUSEMOVE:
		mouseEvent.type = DW_MOUSE_MOVE;
		break;
	case WM_MOUSEWHEEL:
		mouseEvent.type = DW_MOUSE_WHEEL;
		break;
	}

	if (mouseEvent.type != DW_MOUSE_NONE) {

		mouseEvent.x      = GET_X_LPARAM(lParam);
		mouseEvent.y      = GET_Y_LPARAM(lParam);
		mouseEvent.ctrl   = (LOWORD(wParam) & MK_CONTROL) != 0;
		mouseEvent.shift  = (LOWORD(wParam) & MK_SHIFT)   != 0;
		mouseEvent.left   = (LOWORD(wParam) & MK_LBUTTON) != 0;
		mouseEvent.right  = (LOWORD(wParam) & MK_RBUTTON) != 0;
		mouseEvent.middle = (LOWORD(wParam) & MK_MBUTTON) != 0;
		mouseEvent.scroll = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);

		if (esContext && esContext->m_pMouseEventFunc) {
			esContext->m_pMouseEventFunc(esContext, mouseEvent);
		}
	}


	/***********************
	 * Handle other events.
	 ***********************/

	switch (uMsg) {

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;


	case WM_MOVE:
		if (esContext) {
			esContext->m_positionX = LOWORD(lParam);
			esContext->m_positionY = HIWORD(lParam);
		}
		break;
	
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		{
			KeyboardEvent keyEvent;
			memset(&keyEvent, 0, sizeof(KeyboardEvent));

			keyEvent.keyCode = static_cast<keyCode_t>(wParam);
			keyEvent.pressed = (uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN);

			keyEvent.shift = (GetKeyState(VK_SHIFT)   & 0x80) != 0;
			keyEvent.ctrl  = (GetKeyState(VK_CONTROL) & 0x80) != 0;

			if (esContext && esContext->m_pKeyboardEventFunc) {
				esContext->m_pKeyboardEventFunc(esContext, keyEvent);
			}
		}
		break;
	
	default:
		break;
	} 

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}



// Create Win32 instance and window.
GLboolean CreateWin32 (ESContext * esContext, const char * title) {


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

	esContext->m_eglNativeWindow = CreateWindow(
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
	SetWindowLongPtr(esContext->m_eglNativeWindow, GWL_USERDATA, (LONG) (LONG_PTR) esContext);


	if (esContext->m_eglNativeWindow == NULL) {
		return GL_FALSE;
	}

	esContext->m_eglNativeDisplay = GetDC(esContext->m_eglNativeWindow);

	ShowWindow(esContext->m_eglNativeWindow, TRUE);

	return GL_TRUE;
}



const int TICKS_PER_SECOND = 100;
const int SKIP_TICKS       = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP    = 5;


// Start main windows loop.
// Based on deWiTTERS Game Loop: Constant Game Speed with Maximum FPS.
void WinLoop (ESContext * esContext) {

	MSG   msg      = {0};
	DWORD lastTime = GetTickCount();

	DWORD next_game_tick = GetTickCount();
	int loops;

	while (true) {

		if (GetActiveWindow() == esContext->m_eglNativeWindow) {

			loops = 0;
			while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {

				DWORD currTime  = GetTickCount();
				float deltaTime = static_cast<float>(currTime - lastTime) / 1000.0f;

				lastTime = currTime;

				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						return;
					} else {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}

				// Call update function if registered.
				if (esContext && esContext->m_pUpdateFunc) {
					esContext->m_pUpdateFunc(esContext, deltaTime);
				}

				next_game_tick += SKIP_TICKS;
				loops++;

			}

			// Call draw function.
			if (esContext && esContext->m_pDrawFunc) {
				esContext->m_pDrawFunc(esContext);
			}
		}
	}
}
