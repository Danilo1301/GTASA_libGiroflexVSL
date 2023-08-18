#pragma once

#include "../pch.h"
#include "../AudioStream.h"

#include "SoundPanelButton.h"

struct AudioStreamData
{
	AudioStream* audioStream = NULL;
	SoundPanelButton* button = NULL;
};

class WindowSoundPanel {
public:
	static std::vector<SoundPanelButton*> m_buttons;
	static std::vector<AudioStreamData> m_audioStreamData;
	static int m_prevActiveIndex;
	static bool m_allowMultipleSounds;
	static CRGBA m_buttonDefaultColor;
	static CRGBA m_buttonActiveColor;
	static CVector2D m_position;

	static bool m_visible;


	static void Toggle(bool state);
	static void Create();
	static void Update(int dt);
	static void Draw();
	static void DestroyButtons();

	static SoundPanelButton* AddButton();
	static void RecreateButtons();

	static void AddButtonToAudioList(SoundPanelButton* button, int audioId);
	static void ToggleAudioButton(int index);
	static void StopAllSounds();
};