#include "Input.h"

#include "Log.h"

std::map<int, TouchData> Input::touchStates;
uintptr_t Input::pTouchPos = 0;
uintptr_t Input::pScreenGetWidth= 0;
uintptr_t Input::pScreenGetHeight= 0;
uintptr_t Input::pScreenResX= 0;
uintptr_t Input::pScreenResY= 0;
bool Input::isTouchPressed = false;
bool Input::hasTouchBeenPressedThisFrame = false;
bool Input::hasTouchBeenReleasedThisFrame = false;

char* strValue;

int OS_ScreenGetHeight()
{
    int (*func)() = (int (*)()) Input::pScreenGetHeight;
    return func();
}

int OS_ScreenGetWidth()
{
    int (*func)() = (int (*)()) Input::pScreenGetWidth;
    return func();
}

void Input::Update(float dt)
{
    hasTouchBeenPressedThisFrame = false;
    hasTouchBeenReleasedThisFrame = false;

    int numPresses = 0;

    for (auto pair : touchStates)
    {
        int id = pair.first;
        auto touchData = &touchStates[id];

        if (touchData->isPressed)
        {
            numPresses++;
            touchData->timePressed += dt;
        }
    }

    if (isTouchPressed)
    {
        if (numPresses == 0)
        {
            isTouchPressed = false;
            //Log::file << "Input: Touch released" << std::endl;
            hasTouchBeenReleasedThisFrame = true;
        }
    }
}

CVector2D Input::GetScreenSize()
{
    return { (float)OS_ScreenGetWidth(), (float)OS_ScreenGetHeight() };
}

CVector2D Input::GetGTAScreenSize()
{
    return { 1030.0f, 448.0f };
}

CVector2D Input::GetTouchPos()
{
    auto orgPos = (CVector2D*)pTouchPos;
    auto gtaScreenSize = GetGTAScreenSize();
    auto screenSize = GetScreenSize();

    return {
        orgPos->x / (screenSize.x / gtaScreenSize.x),
        orgPos->y / (screenSize.y / gtaScreenSize.y)
    };
}

void Input::SetTouchState(int touchId, bool pressed)
{
    if (touchStates.find(touchId) == touchStates.end())
    {
        TouchData data;
        data.isPressed = false;
        data.timePressed = 0;
        touchStates[touchId] = data;
    }

    if (touchStates[touchId].isPressed != pressed)
    {
        if (pressed)
        {
            //Log::file << "Input: Touch " << touchId << " pressed" << std::endl;
        }
        else {
            //Log::file << "Input: Touch " << touchId << " released after " << touchStates[touchId].timePressed << " ms" << std::endl;
        }

        touchStates[touchId].isPressed = pressed;
        touchStates[touchId].timePressed = 0;
    }

    if (pressed)
    {
        if (!Input::isTouchPressed)
        {
            isTouchPressed = true;
            hasTouchBeenPressedThisFrame = true;

            //Log::file << "Input: Touch pressed" << std::endl;
        }
    }
}

bool Input::IsPointInsideRect(CVector2D pos, CVector2D rectPos, CVector2D rectSize)
{
    if (pos.x >= rectPos.x && pos.x <= rectPos.x + rectSize.x)
    {
        if (pos.y >= rectPos.y && pos.y <= rectPos.y + rectSize.y)
        {
            return true;
        }
    }

    return false;
}

bool Input::GetTouchIdState(int touchId)
{
    if (touchStates.find(touchId) == touchStates.end()) return false;

    return touchStates[touchId].isPressed;
}

int Input::GetTouchIdPressTime(int touchId)
{
    if (touchStates.find(touchId) == touchStates.end()) return 0;

    return touchStates[touchId].timePressed;
}