#include "WindowMain.h"

#include "ModelInfos.h"

#include "WindowLightGroups.h"

Window* WindowMain::m_Window = NULL;
int WindowMain::numCoronas = 1;
int WindowMain::type = 0;
int WindowMain::m_ModelId = 0;

CVector testfloat1 = CVector(1.234f, 5.678f, 9.012f);

float float2 = 2.5f;
int int1 = 2;

void WindowMain::Create(int modelId)
{
	if (m_Window) return;

    m_ModelId = modelId;

    auto window = m_Window = Menu::AddWindow(14);
    window->position = CVector2D(80, 200);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        WindowMain::Remove();
    };

    auto text_id = window->AddText(1, CRGBA(255, 255, 255));
    text_id->text->num1 = m_ModelId;

    auto test1 = window->AddFloatRange(20, &float2, -10.0f, 10.0f, 0.01f);

    auto test2 = window->AddOptions(21);
    test2->AddOption(12, 4, 0);
    test2->AddOption(12, 8, 0);
    test2->AddOption(12, 12, 0);
    test2->onValueChange = [test2]() {
        Menu::ShowPopup(1, *test2->intValueRange.value, 0, 800);
    };

   /*
    auto test2 = window->AddFloatRange(10, &testfloat1.y, -10.0f, 10.0f);
    auto test3 = window->AddFloatRange(11, &testfloat1.z, -10.0f, 10.0f);
    */

    auto button_add = window->AddButton(14, { 0, 255, 0, 255 });
    button_add->onClick = [window]()
    {
        WindowLightGroups::Create(window);
    };

   /*
    auto options2 = window->AddOptions(0);
    options2->optionCurrent = type;
    options2->onValueChange = [options2]() {
        Menu::ShowPopup(0, options2->optionCurrent, 0, 800);
        WindowMain::type = options2->optionCurrent;
    };

    auto options1 = window->AddOptions(0);
    options1->optionCurrent = numCoronas;
    options1->onValueChange = [options1]() {
        Menu::ShowPopup(0, options1->optionCurrent, 0, 800);
        WindowMain::numCoronas = options1->optionCurrent;
    };
    */
}

void WindowMain::Remove()
{
    m_Window->RemoveThisWindow();
    m_Window = NULL;
    m_ModelId = 0;

}