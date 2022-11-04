#pragma once

#include "Draw.h"
#include "Item.h"

enum eItemType {
	ITEM_TEXT,
	ITEM_BUTTON,
	ITEM_OPTIONS
};

class Item {
public:
	DrawItem* box = new DrawItem(eDrawType::BOX);
	DrawItem* text = new DrawItem(eDrawType::TEXT);

	Item* btnLeft = NULL;
	Item* btnRight = NULL;

	int optionCurrent = 0;
	int optionMin = 0;
	int optionMax = 256;
	int addBy = 1;

	float* pResultFloat = NULL;
	int* pResultInt = NULL;

	bool holdToChange = false;

	bool useFullWidth = false;

	eItemType type;

	bool isPressed = false;
	bool isPointerOver = false;

	CVector2D position = { 0, 0 };

	std::function<void()> onClick;
	std::function<void()> onValueChange;

	bool drawBox = true;
	bool drawText = true;

	Item(eItemType type);

	void AddOptionBy(int by);

	void Update(); 
	void Draw(); 
};