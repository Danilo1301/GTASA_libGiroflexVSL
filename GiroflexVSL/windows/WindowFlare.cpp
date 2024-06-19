#include "WindowFlare.h"

void WindowFlare::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(63, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(63, &lightGroup->useFlare);

    window->AddFloatRange(64, &lightGroup->flareDistance, 10.0f, 1000.0f, 0.2f);

    window->AddFloatRange(65, &lightGroup->flareIntensity, 0, 1, 0.01f);

    auto flareTexture = window->AddIntRange(72, &lightGroup->flareTexture, 1, 2, 1);
    flareTexture->holdToChange = false;
}