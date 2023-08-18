#include "SoundPanelButton.h"

#include "../Input.h"

SoundPanelButton::SoundPanelButton()
{

}

void SoundPanelButton::Update(int dt)
{
	//input
	isPointerOver = Input::IsPointInsideRect(Input::GetTouchPos(), position, size);

	if (isPointerOver)
	{
		if (Input::hasTouchBeenReleasedThisFrame)
		{
			if (canBeActivated)
			{
				isActive = !isActive;
			}

			if (onClick) onClick();
		}

		if (Input::isTouchPressed)
		{
			if (!isPressed)
			{
				isPressed = true;
				if (onPressBegin) onPressBegin();
			}
		}
	}

	if (!isPointerOver || !Input::isTouchPressed)
	{
		if (isPressed)
		{
			isPressed = false;
			if (onPressEnd) onPressEnd();
		}
	}

}

void SoundPanelButton::Draw()
{
	CRGBA bgColor = color;
	if (isActive) bgColor = activeColor;

	Draw::DrawSpriteWithText(
		sprite.spriteId,
		text.gxtId,
		text.num1,
		text.num2,
		position,
		size,
		bgColor,
		text.color,
		text.textAlign
	);
}

void SoundPanelButton::Destroy()
{

}