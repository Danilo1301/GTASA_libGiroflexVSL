#pragma once

#include "pch.h"

#include "Item.h"

class Window {
public:
	std::vector<Item*> items;

	float width = 280.0f;
	float titleHeight = 20.0f;

	int titleGtxId = 0;
	CVector2D position = { 0, 0 };
	CRGBA backgroundColor = { 0, 0, 120, 255 };

	Window* parentWindow = NULL;

	Item* btnLeft = NULL;
	Item* btnRight = NULL;
	Item* btnBack = NULL;

	bool visible = true;

	int maxItemsPerPage = 8;
	int page = 0;
	bool showPageControls = false;

	Window();

	Item* AddButton(int gxtId, CRGBA color);
	Item* AddText(int gxtId, CRGBA color);
	Item* AddOptions(int gxtId);

	void Update();
	void Draw();
	void Destroy();

	std::vector<Item*> GetItemsToDraw();

	void GoToPrevWindow();
	void RemoveThisWindow();
};