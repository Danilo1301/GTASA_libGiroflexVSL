#include "CleoMenu.h"

#include "Menu.h"

#include "Log.h"

std::map<int, Window*> CleoMenu::m_Windows;
std::map<int, Item*> CleoMenu::m_Items;

Window* CleoMenu::GetWindowById(int windowId)
{
    if(m_Windows.find(windowId) == m_Windows.end()) return NULL;
    return m_Windows[windowId];
}

Item* CleoMenu::GetItemById(int itemId)
{
    if(m_Items.find(itemId) == m_Items.end()) return NULL;
    return m_Items[itemId];
}

int CleoMenu::AddWindow(int gxtId, float x, float y, float width)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: AddWindow" << std::endl;

    auto window = Menu::AddWindow(gxtId);
    window->position = CVector2D(x, y);
    window->width = width;
    window->showPageControls = true;

    int i = 1;
    while (m_Windows.find(i) != m_Windows.end())
    {
        //Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: checked ID " << i << std::endl;
        i++;
    }

    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: Found free ID " << i << std::endl;

    m_Windows[i] = window;

    return i;
}

void CleoMenu::RemoveWindow(int windowId)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: RemoveWindow " << windowId << std::endl;

    auto window = GetWindowById(windowId);

    std::vector<int> toRemove;

    for(auto item : window->items)
    {
        auto it = m_Items.begin();
        while (it != m_Items.end()) {
            if (it->second == item) {
                toRemove.push_back(it->first);
            }
            ++it;
        }
    }

    for(auto id : toRemove)
    {
        m_Items.erase(id);

        Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: Item ID " << id << " is now free" << std::endl;
    }

    window->RemoveThisWindow();

    m_Windows.erase(windowId);

    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: Window ID " << windowId << " is now free" << std::endl;
}

int CleoMenu::AddButton(int gxtId, int windowId, int num1, int num2)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: AddButton" << std::endl;

    auto window = GetWindowById(windowId);

    auto button = window->AddButton(gxtId, num1, num2);

    int i = 1;
    while (m_Items.find(i) != m_Items.end())
    {
        //Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: checked ID " << i << std::endl;
        i++;
    }

    Log::Level(LOG_LEVEL::LOG_BOTH) << "CleoMenu: Found free ID " << i << std::endl;

    m_Items[i] = button;

    return i;
}

bool CleoMenu::HasButtonJustBeenPressed(int buttonId)
{
    auto button = GetItemById(buttonId);

    if(!button) return false;

    return button->hasPressedThisFrame;
}

void CleoMenu::SetButtonColor(int buttonId, int r, int g, int b)
{
    auto button = GetItemById(buttonId);

    button->box->color = CRGBA(r, g, b, 255);
}