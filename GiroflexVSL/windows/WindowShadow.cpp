#include "WindowShadow.h"

void WindowShadow::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(46, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(46, &lightGroup->renderShadow);

    auto shadow_intensity = window->AddFloatRange(47, &lightGroup->shadowIntensity, 0, 1, 0.01f);
    shadow_intensity->holdToChange = true;

    window->AddFloatRange(48, &lightGroup->shadowSize, 0.0f, 10.0f, 0.01f);

    //window->AddCheckbox(0, &lightGroup->useCustomShadowPosition);

    window->AddFloatRange(38, &lightGroup->shadowPositionX, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(39, &lightGroup->shadowPositionY, -10.0f, 10.0f, 0.01f);

    window->AddFloatRange(63, &lightGroup->shadowRotation, -1000.0f, 1000.0f, 0.01f);

    window->AddCheckbox(64, &lightGroup->shadowFlipTextures);

    auto coronaTexture = window->AddIntRange(62, &lightGroup->shadowTexture, 0, 100, 1);
    coronaTexture->holdToChange = false;
}