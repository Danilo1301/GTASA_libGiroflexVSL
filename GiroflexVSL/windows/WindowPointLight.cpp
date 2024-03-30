#include "WindowPointLight.h"

void WindowPointLight::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(49, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(49, &lightGroup->renderPointLight);

    window->AddFloatRange(50, &lightGroup->pointLightDistance, 0.0f, 100.0f, 0.2f);

    window->AddFloatRange(51, &lightGroup->pointLightIntensity, 0.0f, 1.0f, 0.01f);
}