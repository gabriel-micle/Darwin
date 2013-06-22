
#pragma once

enum mouseEventType_t {
	DW_MOUSE_NONE,
	DW_LBUTTON_DOWN,
	DW_RBUTTON_DOWN,
	DW_MBUTTON_DOWN,
	DW_LBUTTON_UP,
	DW_RBUTTON_UP,
	DW_MBUTTON_UP,
	DW_MOUSE_MOVE,
	DW_MOUSE_WHEEL,
};

struct MouseEvent {

	// The type of event.
	mouseEventType_t type;

	// X position of cursor.
	int x;

	// Y position of cursor.
	int y;

	// True if Ctrl is also pressed.
	bool ctrl;

	// True if Shift is also pressed.
	bool shift;

	// Scroll distance.
	float scroll;

	// True if left mouse button is also pressed.
	bool left;

	// True if right mouse button is also pressed.
	bool right;

	// True is middle mouse button is also pressed.
	bool middle;
};
