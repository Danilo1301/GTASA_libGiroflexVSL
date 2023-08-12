#pragma once

#include "../pch.h"
#include "../AudioStream.h"

class SoundPanelButton
{
public:
	CVector2D position = { 0, 0 };
	CVector2D size = { 50.0f, 50.0f };
	CRGBA outlineColor = CRGBA(255, 0, 0, 150);
	CRGBA color = CRGBA(255, 80, 80, 150);

	bool blink = false;
	float blinkState = 0.0f;

	float volume = 0.5f;

	int gxtId = 1;
	int gxtNum1 = 0;
	int gxtNum2 = 0;

	AudioStream* audioStream = NULL;

	std::function<void()> onClick;
	std::function<void()> onPressBegin;
	std::function<void()> onPressEnd;

	bool isPointerOver = false;
	bool isPressed = false;

	SoundPanelButton();

	void Update(int dt);
	void Draw();
};

class WindowSoundPanel {
public:
	static std::vector<SoundPanelButton*> Buttons;
	static std::vector<SoundPanelButton*> AudioButtons;
	static bool Visible;

	static int PrevActiveButtonIndex;

	static bool AllowMultipleSounds;
	static CRGBA ButtonColor;
	static CRGBA ButtonOutlineColor;
	static CVector2D OffsetPosition;
	static float ButtonSize;

	static void Toggle(bool state);
	static void Create();
	static void DestroyButtons();
	static void RecreateButtons();
	static void Update(int dt);
	static void Draw();

	static void StopAllSounds();

	static SoundPanelButton* AddButton();
	static void ToggleButton(int index);
};