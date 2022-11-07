#include "Menu.h"
#include "Input.h"
#include "Log.h"

Item::Item(eItemType type)
{
	this->type = type;

	if (type == eItemType::ITEM_BUTTON)
	{
		drawLabel = false;
	}

	if (type == eItemType::ITEM_OPTIONS || type == eItemType::ITEM_INT_RANGE || type == eItemType::ITEM_FLOAT_RANGE)
	{
		btnLeft = new Item(eItemType::ITEM_BUTTON);

		btnRight = new Item(eItemType::ITEM_BUTTON);
	}
}

void Item::AddOptionBy(int addBy)
{

}

void Item::AddOption(int gxtId, int num1, int num2)
{
	Option option = { gxtId, num1, num2 };
	options.push_back(option);
}

void Item::Update()
{
	isPointerOver = Input::IsPointInsideRect(Input::GetTouchPos(), position, box->size);

	if (isPointerOver)
	{
		//if (Input::hasTouchBeenPressedThisFrame && onClick && type == eItemType::ITEM_BUTTON) onClick();
		if (Input::hasTouchBeenReleasedThisFrame && onClick && type == eItemType::ITEM_BUTTON) onClick();


		if (!isPressed)
		{
			isPressed = true;
		}
	}

	if (!isPointerOver || !Input::isTouchPressed)
	{
		isPressed = false;
	}

	//

	if (btnLeft) btnLeft->Update();
	if (btnRight) btnRight->Update();
	

	//

	if (type == eItemType::ITEM_OPTIONS || type == eItemType::ITEM_INT_RANGE)
	{
		if (type == eItemType::ITEM_OPTIONS)
		{
			intValueRange.min = 0;
			intValueRange.max = options.size() - 1;
		}

		int prevVal = *intValueRange.value;

		if (btnLeft->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) intValueRange.ChangeValueBy(-intValueRange.addBy);
			}
			else {
				if(Input::hasTouchBeenPressedThisFrame) intValueRange.ChangeValueBy(-intValueRange.addBy);
			}
		}

		if (btnRight->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) intValueRange.ChangeValueBy(intValueRange.addBy);
			}
			else {
				if (Input::hasTouchBeenPressedThisFrame) intValueRange.ChangeValueBy(intValueRange.addBy);
			}
		}

		if (*intValueRange.value != prevVal)
		{
			if (onValueChange)
			{
				if (type == eItemType::ITEM_OPTIONS)
				{
					Log::file << "Option changed from " << prevVal << " to " << *intValueRange.value << " - " << intValueRange.min << " / " << intValueRange.max << std::endl;
				}

				onValueChange();
			}
		}
	}


	//


	if (type == eItemType::ITEM_FLOAT_RANGE)
	{
		float prevVal = *floatValueRange.value;

		if (btnLeft->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) floatValueRange.ChangeValueBy(-floatValueRange.addBy);
			}
			else {
				if (Input::hasTouchBeenPressedThisFrame) floatValueRange.ChangeValueBy(-floatValueRange.addBy);
			}
		}

		if (btnRight->isPointerOver)
		{
			if (holdToChange)
			{
				if (Input::isTouchPressed) floatValueRange.ChangeValueBy(floatValueRange.addBy);
			}
			else {
				if (Input::hasTouchBeenPressedThisFrame) floatValueRange.ChangeValueBy(floatValueRange.addBy);
			}
		}

		if (*floatValueRange.value != prevVal)
		{
			if (onValueChange) onValueChange();
		}
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

	if (type == eItemType::ITEM_OPTIONS || type == eItemType::ITEM_INT_RANGE || type == eItemType::ITEM_FLOAT_RANGE)
	{
		text->gxtId = 1;

		if (type == eItemType::ITEM_FLOAT_RANGE && floatValueRange.value != NULL)
		{
			text->num1 = (int)floor((*floatValueRange.value) * 100.0f);
		}

		if (type == eItemType::ITEM_INT_RANGE && intValueRange.value != NULL)
		{
			text->num1 = *intValueRange.value;
		}

		if ((type == eItemType::ITEM_OPTIONS) && intValueRange.value != NULL)
		{
			if (options.size() > 0)
			{
				auto option = options[*intValueRange.value];

				text->gxtId = option.gxtId;
				text->num1 = option.num1;
				text->num2 = option.num2;
			}

			
		}

		Draw::DrawBoxWithText(text->gxtId, text->num1, text->num2, position, box->size, box->color, text->color);
	
		auto btnSize = CVector2D(box->size.y + 20.0f, box->size.y);
		auto btnColor = CRGBA(255, 255, 255);

		if (btnLeft)
		{
			btnLeft->box->size = btnSize;
			btnLeft->text->gxtId = 4;
			btnLeft->box->color = btnColor;
			btnLeft->position = position;
			btnLeft->Draw();
		}
		
		if (btnRight)
		{
			btnRight->box->size = btnSize;
			btnRight->text->gxtId = 5;
			btnRight->box->color = btnColor;
			btnRight->position = CVector2D(position.x + box->size.x - btnSize.x, position.y);
			btnRight->Draw();
		}
	}
}