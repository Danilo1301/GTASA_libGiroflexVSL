#pragma once

#include <map>

#include <SimpleGTA.h>

struct TouchData
{
	bool isPressed;
	int timePressed;
};

class Input {
public:
	static std::map<int, TouchData> touchStates;
	static bool isTouchPressed;
	static bool hasTouchBeenPressedThisFrame;
	static bool hasTouchBeenReleasedThisFrame;

	static void Update(float dt);
	static CVector2D GetScreenSize();
	static CVector2D GetGTAScreenSize();
	static CVector2D GetTouchPos();
	static void SetTouchState(int touchId, bool pressed);
	static bool IsPointInsideRect(CVector2D pos, CVector2D rectPos, CVector2D rectSize);

	static bool GetTouchIdState(int touchId);
	static int GetTouchIdPressTime(int touchId);
};