#include "WindowShadow.h"

void WindowShadow::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(56, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(56, &lightGroup->renderShadow);

    auto shadow_intensity = window->AddFloatRange(57, &lightGroup->shadowIntensity, 0, 1, 0.01f);
    shadow_intensity->holdToChange = true;

    window->AddFloatRange(58, &lightGroup->shadowSize, 0.0f, 10.0f, 0.01f);

    //window->AddCheckbox(0, &lightGroup->useCustomShadowPosition);

    window->AddFloatRange(10, &lightGroup->shadowPositionX, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(11, &lightGroup->shadowPositionY, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(73, &lightGroup->shadowRotation, -1000.0f, 1000.0f, 0.01f);

    window->AddCheckbox(74, &lightGroup->shadowFlipTextures);

    auto coronaTexture = window->AddIntRange(72, &lightGroup->shadowTexture, 0, 100, 1);
    coronaTexture->holdToChange = false;
}