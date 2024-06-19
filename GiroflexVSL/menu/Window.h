#pragma once

#include "Item.h"

class Window {
public:
	static CVector2D m_DefaultWindowPosition;
	static float m_DefaultWindowWidth;

	std::vector<Item*> items;
	std::vector<Item*> floatingItems;

	float width;
	float titleHeight = 20.0f;

	int titleGtxId = 0;
	CVector2D position = { 0, 0 };

	Window* parentWindow = NULL;

	Item* btnLeft = NULL;
	Item* btnRight = NULL;
	Item* btnBack = NULL;

	bool visible = true;

	int maxItemsPerPage = 5;
	int page = 0;
	bool showPageControls = false;
	bool showTitle = true;

	bool waitingForTouchRelease = false;

	bool canBeRemoved = false;

	Window();

	void AddItem(Item* item);
	void AddFloatingItem(Item* item);
	Item* AddButton(int gxtId, int num1, int num2, CRGBA color);
	Item* AddButton(int gxtId);
	Item* AddButton(int gxtId, int num1, int num2);
	Item* AddButton(int gxtId, CRGBA color);
	Item* AddFloatingButton(int gxtId, int num1, int num2, CVector2D position, CVector2D size, CRGBA color);
	Item* AddFloatingButton(int gxtId, int num1, int num2, CVector2D position, CVector2D size);
	Item* AddCheckbox(int gxtId, bool* value);
	Item* AddText(int gxtId, int num1, int num2, CRGBA color);
	Item* AddText(int gxtId);
	Item* AddOptions(int gxtId);
	Item* AddFloatRange(int gxtId, float* value, float min, float max, float addBy);
	Item* AddIntRange(int gxtId, int* value, int min, int max, int addBy);

	void Update();
	void Draw();
	void Destroy();

	std::vector<Item*> GetItemsToDraw();
	std::vector<Item*> GetTotalItems();

	void GoToPrevWindow();
	void RemoveThisWindow();

	int GetMaxPages();
};