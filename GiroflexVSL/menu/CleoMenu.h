#pragma once

#include <map>

#include "Window.h"

class CleoMenu {
public:
    static std::map<int, Window*> m_Windows;
    static std::map<int, Item*> m_Items;

    static Window* GetWindowById(int windowId);
    static Item* GetItemById(int itemId);

	static int AddWindow(int gxtId, float x, float y, float width);
	static void RemoveWindow(int windowId);
	static int AddButton(int gxtId, int windowId, int num1, int num2);
	static bool HasButtonJustBeenPressed(int buttonId);
	static void SetButtonColor(int buttonId, int r, int g, int b);
};