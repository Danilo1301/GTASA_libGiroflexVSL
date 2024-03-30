#include "WindowEditing.h"

#include "WindowMain.h"

#include "../Input.h"

bool WindowEditing::FreezeLights = false;
bool WindowEditing::ShowCurrentEditingLightGroup = false;
float WindowEditing::MoveLightsSpeed = 1.0f;
LightGroup* WindowEditing::LightGroupToShow = NULL;

void WindowEditing::Create(Window* parent, LightGroup* lightGroup)
{
    LightGroupToShow = lightGroup;

    //auto modelId = WindowMain::m_ModelId;

    auto window = Menu::AddWindow(24, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(25, &FreezeLights);
    window->AddCheckbox(26, &ShowCurrentEditingLightGroup);
    window->AddFloatRange(27, &MoveLightsSpeed, 0.0f, 2.0f, 0.01f);
}