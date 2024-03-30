#include "WindowSelectPanel.h"
#include "WindowPanel.h"
#include "WindowSoundPanel.h"

#include "../ModelInfos.h"
#include "../Globals.h"

Window* WindowSelectPanel::m_Window = NULL;

void WindowSelectPanel::Create()
{
	if (m_Window) return;

	auto window = m_Window = Menu::AddWindow(8);
	window->position = CVector2D(80, 200);

	auto button_lights_panel = window->AddButton(71);
	button_lights_panel->onClick = [window]()
	{
		Remove();

		auto veh = Globals::GetPlayerVehicle();

		//not really necessary..
		if (!ModelInfos::HasModelInfo(veh->modelId))
		{
			Menu::ShowPopup(70, 0, 0, 2000);
			return;
		}

		WindowPanel::Toggle(true);
	};

	auto button_audio_panel = window->AddButton(72);
	button_audio_panel->onClick = [window]()
	{
		Remove();
		WindowSoundPanel::Toggle(true);
	};

	auto button_close = window->AddButton(10, CRGBA(170, 70, 70));
	button_close->onClick = []()
	{
		Remove();
	};
}

void WindowSelectPanel::Remove()
{
	if (!m_Window) return;

	m_Window->RemoveThisWindow();
	m_Window = NULL;
}