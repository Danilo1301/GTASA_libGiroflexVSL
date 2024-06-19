#pragma once

#include "Draw.h"

#include <functional>

enum eItemType {
	ITEM_TEXT,
	ITEM_BUTTON,
	ITEM_OPTIONS,
	ITEM_INT_RANGE,
	ITEM_FLOAT_RANGE,
	CHECKBOX
};


struct Option
{
	int gxtId = 1;
	int num1 = 3;
	int num2 = 4;
};

struct ColorIndicator
{
	CRGBA* color;
};

template<class T>
class ValueRange {
public:
	T* value = NULL;
	T min = 0;
	T max = 255;
	T addBy = 1;

	void ChangeValueBy(T amount)
	{
		*value += amount;

		if (*value < min) *value = min;
		if (*value > max) *value = max;
	}
};

class Item {
public:
	DrawItem* box = new DrawItem(eDrawType::BOX);
	DrawItem* text = new DrawItem(eDrawType::TEXT);
	DrawItem* label = new DrawItem(eDrawType::TEXT);

	Item* btnLeft = NULL;
	Item* btnRight = NULL;

	std::vector<Option> options;

	std::vector<ColorIndicator> colorIndicators;

	std::vector<DrawItem*> extraTexts;

	/*
	int optionCurrent = 0;
	int optionMin = 0;
	int optionMax = 256;
	int addBy = 1;
	*/

	int optionsValue = 0;

	ValueRange<int> intValueRange;
	ValueRange<float> floatValueRange;

	bool* pCheckBoxBool = NULL;
	bool tmpCheckBoxBool = false;

	//float* pResultFloat = NULL;
	//int* pResultInt = NULL;

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
	bool drawLabel = true;

	float btnWidth = 50.0f;

	bool waitingForTouchRelease = false;

	bool hasPressedThisFrame = false;

	Item(eItemType type);

	void AddColorIndicator(CRGBA* color);

	/*
	Adds a text on the right side of the item
	*/
	void AddExtraText(int gxtId, int num1, int num2, CVector2D offsetFromRight);

	void AddOptionBy(int by);

	void AddOption(int gxtId, int num1, int num2);

	void Update(); 
	void Draw(); 
};