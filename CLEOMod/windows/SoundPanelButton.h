#pragma once

#include "../pch.h"
#include "../menu/Draw.h"

class SoundPanelButton
{
public:
	CVector2D position = { 0, 0 };
	CVector2D size = { 50.0f, 50.0f };

	DrawItem sprite = DrawItem(eDrawType::SPRITE);
	DrawItem text = DrawItem(eDrawType::TEXT);

	CRGBA color = CRGBA(255, 255, 255);
	CRGBA activeColor = CRGBA(255, 0, 0);

	bool isPointerOver = false;
	bool isPressed = false;

	bool canBeActivated = true;
	bool isActive = false;

	std::function<void()> onClick;
	std::function<void()> onPressBegin;
	std::function<void()> onPressEnd;

	SoundPanelButton();

	void Update(int dt);
	void Draw();
	void Destroy();
};