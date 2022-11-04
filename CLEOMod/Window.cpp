#include "Window.h"

#include "Log.h"
#include "Input.h"
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

		int maxPages = (int)std::ceil((float)window->items.size() / (float)window->maxItemsPerPage);
		if (window->page >= maxPages-1) window->page = maxPages-1;
	};

	btnLeft->onClick = [window]()
	{
		//int maxPages = (int)std::ceil((float)window->items.size() / (float)window->maxItemsPerPage);

		window->page -= 1;
		if (window->page < 0) window->page = 0;
	};
}

Item* Window::AddButton(int gxtId, CRGBA color)
{
	Item* item = new Item(eItemType::ITEM_BUTTON);

	item->text->gxtId = gxtId;

	item->box->color = color;
	item->box->size = { 200, 35 };

	items.push_back(item);

	Log::file << "Window: AddButton" << std::endl;

	return item;
}


Item* Window::AddOptions(int gxtId)
{
	Item* item = new Item(eItemType::ITEM_OPTIONS);
	item->text->gxtId = gxtId;

	item->box->color = CRGBA(120, 120, 120);
	item->box->size = { 200, 35 };

	items.push_back(item);

	Log::file << "Window: AddOptions" << std::endl;

	return item;
}

Item* Window::AddText(int gxtId, CRGBA color)
{
	Item* item = new Item(eItemType::ITEM_TEXT);

	item->text->gxtId = gxtId;
	item->text->color = color;
	item->useFullWidth = true;
	item->box->size.y = 25;

	items.push_back(item);

	Log::file << "Window: AddText" << std::endl;

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

	Draw::DrawBoxWithText(999, 0, 0, pos, CVector2D(width, titleHeight), CRGBA(52, 52, 109), CRGBA(255, 255, 255));

	//

	if (showPageControls)
	{
		auto btnSize = CVector2D(50.0f, 50.0f);
		auto btnColor = CRGBA(255, 255, 255);

		btnLeft->box->size = btnSize;
		btnLeft->text->gxtId = 4;
		btnLeft->box->color = btnColor;
		btnLeft->position = CVector2D(position.x - 30.0f - btnSize.x, position.y + 50.0f);
		btnLeft->Draw();

		btnRight->box->size = btnSize;
		btnRight->text->gxtId = 5;
		btnRight->box->color = btnColor;
		btnRight->position = CVector2D(position.x + width + 30.0f, position.y + 50.0f);
		btnRight->Draw();

		btnSize = CVector2D(100.0f, 50.0f);

		btnBack->box->size = btnSize;
		btnBack->text->gxtId = 7;
		btnBack->box->color = btnColor;
		btnBack->position = CVector2D(position.x + width + 30.0f, position.y + 150.0f);
		btnBack->Draw();
	}
	//

	pos.y += titleHeight;

	
	auto itemsToDraw = GetItemsToDraw();

    for (auto item : itemsToDraw)
    {
		float padding = 2.0f;

		if (item->useFullWidth) item->box->size.x = width;

		Draw::DrawBox(pos, CVector2D(width, item->box->size.y + padding*2), CRGBA(0, 119, 204));

		pos.y += padding;

		float totalPadding = width - item->box->size.x;

		item->position = CVector2D(pos.x + totalPadding/2, pos.y);

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