#pragma once

#include "pch.h"

#include "Item.h"


class Window {
public:
	std::vector<Item*> items;

	float width = 320.0f;
	float titleHeight = 20.0f;

	int titleGtxId = 0;
	CVector2D position = { 0, 0 };
	CRGBA backgroundColor = { 0, 0, 120, 255 };

	Window* parentWindow = NULL;

	Item* btnLeft = NULL;
	Item* btnRight = NULL;
	Item* btnBack = NULL;

	bool visible = true;

	int maxItemsPerPage = 5;
	int page = 0;
	bool showPageControls = false;

	Window();

	Item* AddButton(int gxtId, CRGBA color);
	Item* AddText(int gxtId, CRGBA color);
	Item* AddOptions(int gxtId);
	Item* AddFloatRange(int gxtId, float* value, float min, float max, float addBy);
	Item* AddIntRange(int gxtId, int* value, int min, int max, int addBy);

	void Update();
	void Draw();
	void Destroy();

	std::vector<Item*> GetItemsToDraw();

	void GoToPrevWindow();
	void RemoveThisWindow();
};