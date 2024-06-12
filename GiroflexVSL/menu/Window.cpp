
#include "../Log.h"
#include "../Input.h"
#include "Menu.h"

Window::Window()
{
	btnLeft = new Item(eItemType::ITEM_BUTTON);
	btnRight = new Item(eItemType::ITEM_BUTTON);
	btnBack = new Item(eItemType::ITEM_BUTTON);

	auto window = this;

	btnRight->onClick = [window]()
	{
		window->page += 1;

		int maxPages = window->GetMaxPages();
		if (window->page >= maxPages-1) window->page = maxPages-1;
	};

	btnLeft->onClick = [window]()
	{
		window->page -= 1;
		if (window->page < 0) window->page = 0;
	};
}



Item* Window::AddButton(int gxtId, CRGBA color)
{
	Item* item = new Item(eItemType::ITEM_BUTTON);
	item->drawLabel = false;
	item->useFullWidth = true;

	item->text->gxtId = gxtId;

	item->box->color = color;
	item->box->size = { 200, 35 };

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddButton" << std::endl;

	return item;
}


Item* Window::AddButton(int gxtId)
{
	return AddButton(gxtId, CRGBA(0, 0, 50));	
}

Item* Window::AddCheckbox(int gxtId, bool* value)
{
	Item* item = new Item(eItemType::CHECKBOX);
	item->drawBox = false;
	item->useFullWidth = true;

	item->pCheckBoxBool = value;
	if (value == NULL) item->pCheckBoxBool = &item->tmpCheckBoxBool;

	item->label->gxtId = gxtId;

	item->box->size = { 200, 35 };

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddCheckbox" << std::endl;

	return item;
}

Item* Window::AddOptions(int gxtId)
{
	Item* item = new Item(eItemType::ITEM_OPTIONS);
	item->label->gxtId = gxtId;

	item->intValueRange.value = &item->optionsValue;

	item->box->color = CRGBA(120, 120, 120);
	item->box->size = { 178, 35 };

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddOptions" << std::endl;

	return item;
}

Item* Window::AddFloatRange(int gxtId, float* value, float min, float max, float addBy)
{
	Item* item = new Item(eItemType::ITEM_FLOAT_RANGE);
	item->label->gxtId = gxtId;
	item->floatValueRange.value = value;
	item->floatValueRange.min = min;
	item->floatValueRange.max = max;
	item->floatValueRange.addBy = addBy;
	item->holdToChange = true;

	item->box->color = CRGBA(120, 120, 120);
	item->box->size = { 150, 35 };

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddFloatRange" << std::endl;

	return item;
}


Item* Window::AddIntRange(int gxtId, int* value, int min, int max, int addBy)
{
	Item* item = new Item(eItemType::ITEM_INT_RANGE);
	item->label->gxtId = gxtId;
	item->intValueRange.value = value;
	item->intValueRange.min = min;
	item->intValueRange.max = max;
	item->intValueRange.addBy = addBy;
	item->holdToChange = true;

	item->box->color = CRGBA(120, 120, 120);
	item->box->size = { 150, 35 };

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddIntRange" << std::endl;

	return item;
}

Item* Window::AddText(int gxtId, CRGBA color)
{
	Item* item = new Item(eItemType::ITEM_TEXT);

	item->drawLabel = false;

	item->text->gxtId = gxtId;
	item->text->color = color;
	item->useFullWidth = true;
	item->box->size.y = 25;

	items.push_back(item);

	Log::Level(LOG_LEVEL::LOG_BOTH) << "Window: AddText" << std::endl;

	return item;
}


void Window::Update()
{
	if (!visible) return;

	if (showPageControls)
	{
		btnLeft->Update();
		btnRight->Update();
		btnBack->Update();
	}

	auto itemsToDraw = GetItemsToDraw();

    for (auto item : itemsToDraw)
    {
        item->Update();
    }
}

void Window::Draw()
{
	if (!visible) return;

	CVector2D pos = position;

	if(showTitle)
		Draw::DrawBoxWithText(titleGtxId, 0, 0, pos, CVector2D(width, titleHeight), titleBoxColor, CRGBA(255, 255, 255), eTextAlign::ALIGN_LEFT);

	//

	if (showPageControls)
	{
		auto btnSize = CVector2D(40.0f, 40.0f);
		auto btnColor = backgroundColor;

		bool canGoLeft = page > 0;
		bool canGoRight = page < GetMaxPages() - 1;

		if (canGoLeft)
		{
			btnLeft->box->size = btnSize;
			btnLeft->text->gxtId = 4;
			btnLeft->box->color = btnColor;
			btnLeft->position = CVector2D(position.x - 20.0f - btnSize.x, position.y + 20.0f);
			btnLeft->Draw();
		}
		
		if (canGoRight)
		{
			btnRight->box->size = btnSize;
			btnRight->text->gxtId = 5;
			btnRight->box->color = btnColor;
			btnRight->position = CVector2D(position.x + width + 20.0f, position.y + 20.0f);
			btnRight->Draw();
		}
		
		btnSize = CVector2D(80.0f, 40.0f);

		if (parentWindow != NULL)
		{
			btnBack->box->size = btnSize;
			btnBack->text->gxtId = 7;
			btnBack->box->color = btnColor;
			btnBack->position = CVector2D(position.x + width + 20.0f, position.y + 80.0f);
			btnBack->Draw();
		}
	}
	//

	pos.y += titleHeight;

	
	auto itemsToDraw = GetItemsToDraw();

    for (auto item : itemsToDraw)
    {
		float padding = 2.0f;

		if (item->useFullWidth) item->box->size.x = width - padding*2;

		Draw::DrawBox(pos, CVector2D(width, item->box->size.y + padding * 2), backgroundColor);

		pos.y += padding;

		if (item->drawLabel)
		{
			Draw::DrawText(item->label->gxtId, item->label->num1, item->label->num2, CVector2D(pos.x, pos.y + item->box->size.y / 2), item->label->color, eTextAlign::ALIGN_LEFT);
		}


		float totalPadding = width - item->box->size.x;

		//draw at center
		//item->position = CVector2D(pos.x + totalPadding/2, pos.y); 

		//draw at right
		item->position = CVector2D(pos.x + totalPadding - padding, pos.y);

		item->Draw();

		pos.y += item->box->size.y;
		pos.y += padding;
    }
}

void Window::Destroy()
{

}

std::vector<Item*> Window::GetItemsToDraw()
{
	//int maxPages = (int)std::ceil((float)items.size() / (float)maxItemsPerPage);
	int startIndex = (page * maxItemsPerPage);
	//int endIndex = startIndex + maxItemsPerPage;

	std::vector<Item*> itemsToDraw;

	for (size_t i = 0; i < items.size(); i++)
	{
		if ((int)i >= startIndex && (int)i < (startIndex + maxItemsPerPage))
		{
			auto item = items[i];

			itemsToDraw.push_back(item);
			//totalHeight += item->m_Size.y;
		}
	}

	return itemsToDraw;
}

void Window::GoToPrevWindow()
{
	if (!parentWindow) return;
	parentWindow->visible = true;
	RemoveThisWindow();
}

void Window::RemoveThisWindow()
{
	Menu::RemoveWindow(this);
}

int Window::GetMaxPages()
{
	return (int)std::ceil((float)items.size() / (float)maxItemsPerPage);
}