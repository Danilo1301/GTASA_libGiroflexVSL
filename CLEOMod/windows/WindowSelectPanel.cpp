#include "WindowSelectPanel.h"
#include "WindowPanel.h"
#include "WindowSoundPanel.h"

#include "../Mod.h"
#include "../ModelInfos.h"

Window* WindowSelectPanel::m_Window = NULL;

void WindowSelectPanel::Create()
{
	if (m_Window) return;

	auto window = m_Window = Menu::AddWindow(29);
	window->position = CVector2D(80, 200);

	auto button_lights_panel = window->AddButton(80);
	button_lights_panel->onClick = [window]()
	{
		Remove();

		auto veh = Mod::GetPlayerVehicle();

		//not really necessary..
		if (!ModelInfos::HasModelInfo(veh->modelId))
		{
			Menu::ShowPopup(91, 0, 0, 2000);
			return;
		}

		WindowPanel::Toggle(true);
	};

	auto button_audio_panel = window->AddButton(81);
	button_audio_panel->onClick = [window]()
	{
		Remove();
		WindowSoundPanel::Toggle(true);
	};

	auto button_close = window->AddButton(31, CRGBA(170, 70, 70));
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