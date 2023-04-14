#include "WindowWhiteCorona.h"
#include "WindowMain.h"

#include "../Input.h"
#include "../Mod.h"

void WindowWhiteCorona::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(53, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(53, &lightGroup->useSmallWhiteCorona);

    window->AddFloatRange(44, &lightGroup->smallWhiteCoronaScale, 0.0f, 1.0f, 0.005f);

    auto coronaType = window->AddIntRange(73, &lightGroup->smallWhiteCoronaTexture, 0, 9, 1);
    coronaType->holdToChange = false;
}