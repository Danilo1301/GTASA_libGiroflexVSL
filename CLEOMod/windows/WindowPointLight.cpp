#include "WindowPointLight.h"

#include "../Input.h"
#include "../Mod.h"

void WindowPointLight::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(39, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(39, &lightGroup->renderPointLight);

    window->AddFloatRange(40, &lightGroup->pointLightDistance, 0.0f, 100.0f, 0.2f);

    window->AddFloatRange(41, &lightGroup->pointLightIntensity, 0.0f, 1.0f, 0.01f);
}