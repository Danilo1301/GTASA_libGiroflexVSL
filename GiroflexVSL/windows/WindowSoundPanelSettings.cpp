#include "WindowSoundPanelSettings.h"

#include "WindowSoundPanel.h"

#include "../ModConfig.h"

Window* WindowSoundPanelSettings::m_Window = NULL;

void WindowSoundPanelSettings::Create()
{
	if (m_Window) return;

	auto window = m_Window = Menu::AddWindow(6);
	window->position = CVector2D(350, 100);
	window->showPageControls = true;

	auto style = window->AddIntRange(85, &WindowSoundPanel::m_style, 1, 4, 1);
	style->holdToChange = false;
	style->onValueChange = []() {
		WindowSoundPanel::RecreateButtons();
	};

	auto showOnEnterVehicle = window->AddCheckbox(86, &WindowSoundPanel::m_showOnEnterVehicle);
	showOnEnterVehicle->onValueChange = []() {
		WindowSoundPanel::Toggle(WindowSoundPanel::m_showOnEnterVehicle);
	};

	window->AddCheckbox(87, &WindowSoundPanel::m_showButtonToggleLights);

	auto button_position = window->AddButton(9);
	button_position->onClick = [window]() {
		Menu::AddPosition2DWindow(window, &WindowSoundPanel::m_position, -10000.0f, 10000.0f, 0.5f, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};
	
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

	auto button_color = window->AddButton(47);
	button_color->AddColorIndicator(&WindowSoundPanel::m_buttonDefaultColor);
	button_color->onClick = [window]() {
		Menu::AddColorMenu(window, &WindowSoundPanel::m_buttonDefaultColor, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto button_active_color = window->AddButton(88);
	button_active_color->AddColorIndicator(&WindowSoundPanel::m_buttonActiveColor);
	button_active_color->onClick = [window]() {
		Menu::AddColorMenu(window, &WindowSoundPanel::m_buttonActiveColor, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto button_outline_color = window->AddButton(89);
	button_outline_color->AddColorIndicator(&WindowSoundPanel::m_buttonOutlineColor);
	button_outline_color->onClick = [window]() {
		Menu::AddColorMenu(window, &WindowSoundPanel::m_buttonOutlineColor, []() {
			WindowSoundPanel::RecreateButtons();
		});
	};

	auto size = window->AddFloatRange(51, &WindowSoundPanel::m_buttonSize, 1.0f, 100.0f, 0.05f);
	size->onValueChange = []() {
		WindowSoundPanel::RecreateButtons();
	};

	auto allow_multiple_sounds = window->AddCheckbox(90, &WindowSoundPanel::m_allowMultipleSounds);

	auto button_close = window->AddButton(7, CRGBA(170, 70, 70));
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