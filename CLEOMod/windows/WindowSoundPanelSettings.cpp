#include "WindowSoundPanelSettings.h"
#include "WindowSoundPanel.h"
#include "../ModConfig.h"

Window* WindowSoundPanelSettings::m_Window = NULL;

void WindowSoundPanelSettings::Create()
{
	if (m_Window) return;

	auto window = m_Window = Menu::AddWindow(29);
	window->position = CVector2D(80, 200);
	window->showPageControls = true;

	auto allow_multiple_sounds = window->AddCheckbox(83, &WindowSoundPanel::m_allowMultipleSounds);

	/*
	for (int i = 0; i < WindowSoundPanel::AudioButtons.size(); i++)
	{
		auto audioButton = WindowSoundPanel::AudioButtons[i];

		auto volume = window->AddFloatRange(82, &audioButton->volume, 0.0f, 1.0f, 0.01f);
		volume->label->num1 = i + 1;
	}

	
	auto size = window->AddFloatRange(44, &WindowSoundPanel::ButtonSize, 0.0f, 100.0f, 0.05f);
	size->onValueChange = []() {
		WindowSoundPanel::RecreateButtons();
	};

	

	

	
	*/

	auto button_color = window->AddButton(9);
	button_color->AddColorIndicator(&WindowSoundPanel::m_buttonDefaultColor);
	button_color->onClick = [window]() {
		Menu::AddColorMenu(window, &WindowSoundPanel::m_buttonDefaultColor, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto button_active_color = window->AddButton(10);
	button_active_color->AddColorIndicator(&WindowSoundPanel::m_buttonActiveColor);
	button_active_color->onClick = [window]() {
		Menu::AddColorMenu(window, &WindowSoundPanel::m_buttonActiveColor, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto button_position = window->AddButton(26);
	button_position->onClick = [window]() {
		Menu::AddPosition2DWindow(window, &WindowSoundPanel::m_position, -10000.0f, 10000.0f, 0.5f, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto button_close = window->AddButton(31, CRGBA(170, 70, 70));
	button_close->onClick = []()
	{
		Remove();
		ModConfig::SaveSettings();
	};
}

void WindowSoundPanelSettings::Remove()
{
	if (!m_Window) return;

	m_Window->RemoveThisWindow();
	m_Window = NULL;
}