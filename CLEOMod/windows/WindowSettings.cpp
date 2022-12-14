#include "WindowSettings.h"
#include "WindowMain.h"
#include "../Input.h"
#include "../Mod.h"
#include "../Patterns.h"

bool WindowSettings::EditScreenModeEnabled = false;

void WindowSettings::ToggleEditScreenPos(bool enabled)
{
	EditScreenModeEnabled = enabled;
}

void WindowSettings::Create(Window* parent)
{
	auto window = Menu::AddWindow(26, parent);
	window->showPageControls = true;
	window->btnBack->onClick = [window]()
	{
		window->GoToPrevWindow();
	};

	window->AddCheckbox(57, &Menu::m_DrawCursor);
	
	window->AddIntRange(58, &Patterns::m_TimeBetweenPatterns, 1, 100000, 10);
}

void WindowSettings::Update()
{
	if (!EditScreenModeEnabled) return;

	float speed = 0.05f;

	if (Input::GetTouchIdState(3))
	{
		Menu::m_MenuOffset.x -= speed * Mod::m_DeltaTime;
	}

	if (Input::GetTouchIdState(9))
	{
		Menu::m_MenuOffset.x += speed * Mod::m_DeltaTime;
	}
}

void WindowSettings::Draw()
{
	if (!EditScreenModeEnabled) return;

	auto screenSize = Input::GetGTAScreenSize();
	auto btnSize = CVector2D(150, 150);

	Draw::DrawBoxWithText(51, 0, 0, CVector2D(0, 0), screenSize, CRGBA(0, 0, 20, 230), CRGBA(255, 255, 255));

	Draw::DrawBoxWithText(4, 0, 0, CVector2D(0, screenSize.y - btnSize.y), btnSize, CRGBA(0, 0, 100, 255), CRGBA(255, 255, 255));

	Draw::DrawBoxWithText(5, 0, 0, CVector2D(screenSize.x - btnSize.x, screenSize.y - btnSize.y), btnSize, CRGBA(0, 0, 100, 255), CRGBA(255, 255, 255));
}