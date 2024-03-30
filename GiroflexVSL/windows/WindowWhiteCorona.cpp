#include "WindowWhiteCorona.h"

#include "WindowMain.h"

#include "../Input.h"

void WindowWhiteCorona::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(42, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(42, &lightGroup->useSmallWhiteCorona);

    window->AddFloatRange(41, &lightGroup->smallWhiteCoronaScale, 0.0f, 1.0f, 0.005f);

    auto coronaType = window->AddIntRange(62, &lightGroup->smallWhiteCoronaTexture, 0, 9, 1);
    coronaType->holdToChange = false;

    auto button_color = window->AddButton(30);
    button_color->AddColorIndicator(&lightGroup->smallWhiteCoronaColor);
    button_color->onClick = [lightGroup, window]() {
        Menu::AddColorMenu(window, &lightGroup->smallWhiteCoronaColor);
    };
}