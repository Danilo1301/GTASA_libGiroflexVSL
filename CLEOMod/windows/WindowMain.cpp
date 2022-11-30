#include "WindowMain.h"

#include "WindowLightGroups.h"
#include "WindowSettings.h"
#include "../ModelInfos.h"
#include "../ModConfig.h"
#include "../Mod.h"
#include "../Log.h"

Window* WindowMain::m_Window = NULL;
int WindowMain::numCoronas = 1;
int WindowMain::type = 0;
int WindowMain::m_ModelId = 0;

//CVector testfloat1 = CVector(1.234f, 5.678f, 9.012f);
//float float2 = 2.5f;
//int int1 = 2;

void WindowMain::Create(int modelId)
{
	if (m_Window) return;

    Log::file << "WindowMain: Create..." << std::endl;

    m_ModelId = modelId;

    auto window = m_Window = Menu::AddWindow(29);
    window->position = CVector2D(80, 200);

    auto text_id = window->AddText(30, CRGBA(255, 255, 255));
    text_id->text->num1 = m_ModelId;

    auto button_add = window->AddButton(14);
    button_add->onClick = [window]()
    {
        WindowLightGroups::Create(window);
    };

    auto button_settings = window->AddButton(50);
    button_settings->onClick = [window]()
    {
        WindowSettings::Create(window);
    };

    auto button_close = window->AddButton(31, CRGBA(170, 70, 70));
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