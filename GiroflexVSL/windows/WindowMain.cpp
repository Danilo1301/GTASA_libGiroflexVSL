#include "WindowMain.h"

#include "WindowLightGroups.h"
#include "WindowSettings.h"

#include "ModConfig.h"

#include "Log.h"

Window* WindowMain::m_Window = NULL;
int WindowMain::m_ModelId = 0;

void WindowMain::Create(int modelId)
{
    if (m_Window) return;

    Log::file << "WindowMain: Create for modelId " << modelId << std::endl;

    m_ModelId = modelId;

    auto window = m_Window = Menu::AddWindow(8);
    window->position = CVector2D(80, 200);

    auto text_id = window->AddText(9, CRGBA(255, 255, 255));
    text_id->text->num1 = m_ModelId;

    /*
    for (int i = 0; i < 3; i++)
    {
        auto btn = window->AddCheckbox(14, NULL);
        btn->onValueChange = [btn]() {
            bool &value = btn->tmpCheckBoxBool;
            Menu::ShowPopup(1, value ? 1 : 0, 0, 1000);
        };
    }
    */

    auto button_add = window->AddButton(16);
    button_add->onClick = [window]()
    {
        WindowLightGroups::Create(window);
    };

    auto button_settings = window->AddButton(13);
    button_settings->onClick = [window]()
    {
        WindowSettings::Create(window);
    };

    auto button_close = window->AddButton(10, CRGBA(170, 70, 70));
    button_close->onClick = []()
    {
        WindowMain::Remove();
    };
}

void WindowMain::Remove()
{
    m_Window->RemoveThisWindow();
    m_Window = NULL;
    m_ModelId = 0;

    ModConfig::Save();
}