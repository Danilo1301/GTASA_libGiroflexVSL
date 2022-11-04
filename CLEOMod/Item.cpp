#include "Menu.h"
#include "Input.h"

Item::Item(eItemType type)
{
	this->type = type;

	if (type == eItemType::ITEM_OPTIONS)
	{
		btnLeft = new Item(eItemType::ITEM_BUTTON);
		btnRight = new Item(eItemType::ITEM_BUTTON);

		auto item = this;

		btnLeft->onClick = [item]() {
			//item->AddOptionBy(-item->addBy);
		};

		btnRight->onClick = [item]() {
			//item->AddOptionBy(item->addBy);
		};
	}
}

void Item::AddOptionBy(int addBy)
{

}

void Item::Update()
{
	isPointerOver = Input::IsPointInsideRect(Input::GetTouchPos(), position, box->size);

	if (isPointerOver)
	{
		if (!isPressed)
		{
			isPressed = true;
			if (Input::hasTouchBeenPressedThisFrame && onClick && type == eItemType::ITEM_BUTTON) onClick();
		}
	}

	if (!isPointerOver || !Input::isTouchPressed)
	{
		isPressed = false;
	}

	//

	if (type == eItemType::ITEM_OPTIONS)
	{
		btnLeft->Update();
		btnRight->Update();
	}

	//

	if (type == eItemType::ITEM_OPTIONS)
	{
		int prevVal = optionCurrent;

		if (btnLeft->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) optionCurrent -= addBy;
			}
			else {
				if(Input::hasTouchBeenPressedThisFrame) optionCurrent -= addBy;
			}
		}

		if (btnRight->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) optionCurrent += addBy;
			}
			else {
				if (Input::hasTouchBeenPressedThisFrame) optionCurrent += addBy;
			}
		}

		if (optionCurrent > optionMax) optionCurrent = optionMax;
		if (optionCurrent < optionMin) optionCurrent = optionMin;

		if (optionCurrent != prevVal)
		{
			if(onValueChange) onValueChange();
		}

		text->num1 = optionCurrent;
	}

	//

	
}

void Item::Draw()
{
	if (type == eItemType::ITEM_BUTTON)
	{
		CRGBA boxColor = box->color;

		if (isPressed)
		{
			boxColor.r = 50;
			boxColor.g = 50;
			boxColor.b = 50;
		}

		Draw::DrawBoxWithText(text->gxtId, text->num1, text->num2, position, box->size, boxColor, text->color);
	}

	if (type == eItemType::ITEM_TEXT)
	{
		CVector2D textPos = position;
		textPos.x += box->size.x / 2.0f;
		textPos.y += box->size.y / 2.0f;

		Draw::DrawText(text->gxtId, text->num1, text->num2, textPos, text->color);
	}

	if (type == eItemType::ITEM_OPTIONS)
	{
		Draw::DrawBoxWithText(text->gxtId, text->num1, text->num2, position, box->size, box->color, text->color);

		auto btnSize = CVector2D(box->size.y + 20.0f, box->size.y);
		auto btnColor = CRGBA(255, 255, 255);

		btnLeft->box->size = btnSize;
		btnLeft->text->gxtId = 4;
		btnLeft->box->color = btnColor;
		btnLeft->position = position;
		btnLeft->Draw();

		btnRight->box->size = btnSize;
		btnRight->text->gxtId = 5;
		btnRight->box->color = btnColor;
		btnRight->position = CVector2D(position.x + box->size.x - btnSize.x, position.y);
		btnRight->Draw();
	}
}