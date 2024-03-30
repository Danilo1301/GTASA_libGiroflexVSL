#include "WindowFlare.h"

void WindowFlare::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(53, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(53, &lightGroup->useFlare);

    window->AddFloatRange(54, &lightGroup->flareDistance, 10.0f, 1000.0f, 0.2f);

    window->AddFloatRange(55, &lightGroup->flareIntensity, 0, 1, 0.01f);

    auto flareTexture = window->AddIntRange(62, &lightGroup->flareTexture, 1, 2, 1);
    flareTexture->holdToChange = false;
}