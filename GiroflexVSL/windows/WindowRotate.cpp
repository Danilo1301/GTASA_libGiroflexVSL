#include "WindowRotate.h"

void WindowRotate::Create(Window* parent, LightGroup* lightGroup)
{
    auto window = Menu::AddWindow(94, parent);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    window->AddCheckbox(94, &lightGroup->rotate);

    window->AddFloatRange(53, &lightGroup->rotateDistance, 0.0f, 1000.0f, 0.01f);

    window->AddFloatRange(95, &lightGroup->rotateSpeed, 0.0f, 50.0f, 0.1f);

    window->AddCheckbox(97, &lightGroup->rotateInverse);
}