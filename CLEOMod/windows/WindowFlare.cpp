#include "WindowFlare.h"
#include "WindowMain.h"

#include "../Input.h"
#include "../Mod.h"

void WindowFlare::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(54, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(62, &lightGroup->useFlare);

    window->AddFloatRange(63, &lightGroup->flareDistance, 10.0f, 1000.0f, 0.2f);

    window->AddFloatRange(64, &lightGroup->flareIntensity, 0, 1, 0.01f);
}