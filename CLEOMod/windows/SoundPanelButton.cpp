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

	//
	if (blinkOnActive) blink = isActive;

	//blink
	blinkState += 0.004f * dt;
	if (blinkState >= 1.0f) blinkState = 0.0f;
}

void SoundPanelButton::Draw()
{
	CRGBA bgColor = color;
	if (isActive) bgColor = activeColor;

	if (sprite.spriteId != -1)
	{
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
	else {
		if (drawOutline)
		{
			CRGBA bgcolor = outlineColor;
			float outlineSize = 3.0f;

			if (blink)
			{
				bgcolor = CRGBA(
					ucharIntensity(bgcolor.r, blinkState),
					ucharIntensity(bgcolor.g, blinkState),
					ucharIntensity(bgcolor.b, blinkState)
				);
			}

			Draw::DrawBox(position, size, bgcolor);

			Draw::DrawBoxWithText(
				text.gxtId,
				text.num1,
				text.num2,
				position + CVector2D(outlineSize, outlineSize),
				size - CVector2D(outlineSize * 2, outlineSize * 2),
				bgColor,
				text.color
			);
		}
		else {
			Draw::DrawBoxWithText(
				text.gxtId,
				text.num1,
				text.num2,
				position,
				size,
				bgColor,
				text.color
			);
		}
		
	}
}

void SoundPanelButton::Destroy()
{

}