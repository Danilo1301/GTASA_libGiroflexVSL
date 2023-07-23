#include "WindowShadow.h"

#include "../Input.h"
#include "../Mod.h"

void WindowShadow::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(36, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(36, &lightGroup->renderShadow);

    auto shadow_intensity = window->AddFloatRange(37, &lightGroup->shadowIntensity, 0, 1, 0.01f);
    shadow_intensity->holdToChange = true;

    window->AddFloatRange(38, &lightGroup->shadowSize, 0.0f, 10.0f, 0.01f);

    //window->AddCheckbox(0, &lightGroup->useCustomShadowPosition);

    window->AddFloatRange(23, &lightGroup->shadowPositionX, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(24, &lightGroup->shadowPositionY, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(77, &lightGroup->shadowRotation, 0.0f, 360.0f, 0.01f);

    auto coronaTexture = window->AddIntRange(73, &lightGroup->shadowTexture, 3, 6, 1);
    coronaTexture->holdToChange = false;
}