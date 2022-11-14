#pragma once

#include "../pch.h"

#include "Item.h"
#include "Window.h"

struct MenuPopup
{
	int gfxId = 0;
	int val1 = 0;
	int val2 = 0;
	int timeLeft = 0;
};

class Menu {
public:
	static MenuPopup* m_PopUp;

	static Window* m_MainWindow;
	static std::vector<Window*> m_Windows;



	static Window* AddWindow(int gxtId);
	static Window* AddWindow(int gxtId, Window* parent);
	static Window* AddPositionWindow(Window* parent, CVector* vec);
	static Window* AddColorMenu(Window* parent, CRGBA* color);
	static Window* AddConfirmWindow(Window* parent, int textGxtId, std::function<void()> ohYes, std::function<void()> ohNo);

	static void ShowPopup(int gfxId, int val1, int val2, int time);

	static void Update(int dt);
	static void Draw();

	static void RemoveWindow(Window* window);
	static void RemoveAllWindows();
};