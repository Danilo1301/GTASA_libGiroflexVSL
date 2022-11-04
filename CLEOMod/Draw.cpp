#include "Draw.h"

#include "Log.h"
#include "Input.h"

uintptr_t Draw::pPrintString = 0;

std::vector<DrawItem*> Draw::m_DrawItems;

void Draw::CFont_PrintString(float x, float y, std::string text)
{
	char buffer[256];
	sprintf(buffer, "%s", text.c_str());

	void (*func)(float, float, char*) = (void (*)(float, float, char*)) Draw::pPrintString;
	return func(x, y, buffer);
}

void Draw::DrawBox(CVector2D pos, CVector2D size, CRGBA color)
{
    DrawItem* item = new DrawItem(eDrawType::BOX);
	item->type = eDrawType::BOX;
	item->pos = pos;
	item->color = color;
	item->size = size;

	//Log::file << "Draw: DrawBox at (" << pos.x << ", " << pos.y << "), size " << size.x << ", " << size.y << ")" << std::endl;

	m_DrawItems.push_back(item);
}

void Draw::DrawBoxWithText(int gxtId, int num1, int num2, CVector2D pos, CVector2D boxSize, CRGBA boxColor, CRGBA textColor)
{
	Draw::DrawBox(pos, boxSize, boxColor);

	CVector2D textPos = pos;
	textPos.x += boxSize.x / 2.0f;
	textPos.y += boxSize.y / 2.0f;

	Draw::DrawText(gxtId, num1, num2, textPos, textColor);
}

void Draw::DrawText(int gxtId, int num1, int num2, CVector2D pos, CRGBA color)
{
	DrawItem* item = new DrawItem(eDrawType::TEXT);
	item->type = eDrawType::TEXT;
	item->gxtId = gxtId;
	item->num1 = num1;
	item->num2 = num2;
	item->pos = pos;
	item->color = color;

	//Log::file << "Draw: DrawText " << gxtId << ", nums " << num1 << " and " << num2 << ", at (" << pos.x << ", " << pos.y << ")" << std::endl;

	m_DrawItems.push_back(item);
}