#pragma once

#include "Item.h"
#include "Window.h"

struct MenuPopup
{
	int gfxId = 0;
	int val1 = 0;
	int val2 = 0;
	int timeLeft = 0;
	float width = 200.0f;
};

class Menu {
public:
	static CVector2D m_MenuOffset;

	static std::vector<Window*> m_Windows;
	static Window* m_MainWindow;

	static MenuPopup* m_PopUp;

	static bool m_DrawCursor;

	static Window* AddWindow(int gxtId);
	static Window* AddWindow(int gxtId, Window* parent);
	static Window* AddPositionWindow(Window* parent, CVector* vec, float min, float max, float addBy, std::function<void()> onChange);
	static Window* AddPositionWindow(Window* parent, CVector* vec);
	static Window* AddPosition2DWindow(Window* parent, CVector2D* vec, float min, float max, float addBy, std::function<void()> onChange);
	static Window* AddPosition2DWindow(Window* parent, CVector2D* vec, float min, float max, float addBy);
	static Window* AddColorMenu(Window* parent, CRGBA* color, std::function<void()> onValueChange);
	static Window* AddColorMenu(Window* parent, CRGBA* color);
	static Window* AddConfirmWindow(Window* parent, int textGxtId, std::function<void()> ohYes, std::function<void()> ohNo);

	static void ShowPopup(int gfxId, int val1, int val2, int time, float width = 200.0f);

	static void Update(int dt);
	static void Draw();

	static void RemoveWindow(Window* window);
	static void RemoveAllWindows();
};