#include "WindowWhiteCorona.h"

#include "WindowMain.h"

#include "../Input.h"

void WindowWhiteCorona::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(52, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(52, &lightGroup->useSmallWhiteCorona);

    window->AddFloatRange(51, &lightGroup->smallWhiteCoronaScale, 0.0f, 3.0f, 0.005f);

    auto coronaType = window->AddIntRange(72, &lightGroup->smallWhiteCoronaTexture, 0, 9, 1);
    coronaType->holdToChange = false;

    auto button_color = window->AddButton(47);
    button_color->AddColorIndicator(&lightGroup->smallWhiteCoronaColor);
    button_color->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->smallWhiteCoronaColor);
    };
}