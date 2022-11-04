#pragma once

#include "pch.h"

enum eDrawType {
	BOX,
	TEXT
};

class DrawItem {
public:
	eDrawType type;
	int gxtId = 0;
	int num1 = 0;
	int num2 = 0;
	CVector2D pos = { 0, 0 };
	CVector2D size = { 0, 0 };
	CRGBA color = CRGBA(255, 255, 255, 255);

	DrawItem(eDrawType type)
	{
		this->type = type;

		if (type == eDrawType::BOX)
		{
			color = CRGBA(0, 0, 0);
		}

		if (type == eDrawType::TEXT)
		{
			color = CRGBA(255, 255, 255);
		}
	}
};

class Draw {
public:
	static uintptr_t pPrintString;

	static std::vector<DrawItem*> m_DrawItems;

	static void CFont_PrintString(float x, float y, std::string text);

	static void DrawBox(CVector2D pos, CVector2D size, CRGBA color);
	static void DrawText(int gxtId, int num1, int num2, CVector2D pos, CRGBA color);
	static void DrawBoxWithText(int gxtId, int num1, int num2, CVector2D pos, CVector2D boxSize, CRGBA boxColor, CRGBA textColor);

};