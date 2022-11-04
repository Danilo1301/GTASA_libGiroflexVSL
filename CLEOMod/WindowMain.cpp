#include "WindowMain.h"

#include "ModelInfos.h"

Window* WindowMain::m_Window = NULL;
int WindowMain::numCoronas = 1;
int WindowMain::type = 0;

void WindowMain::Create()
{
	if (m_Window) return;

    auto window = m_Window = Menu::AddWindow(14);
    window->position = CVector2D(300, 150);
    window->showPageControls = true;
    window->btnBack->onClick = [window]()
    {
        window->RemoveThisWindow();
        m_Window = NULL;
    };

    auto text_id = window->AddText(0, CRGBA(255, 255, 255));
    text_id->text->num1 = 1;

    auto text_lightgroups = window->AddText(0, CRGBA(255, 255, 255));
    text_lightgroups->text->num1 = 1;

    auto button_add = window->AddButton(13, { 0, 255, 0, 255 });
    button_add->text->num1 = 13;
    button_add->onClick = [window]() {

        auto window2 = Menu::AddWindow(14, window);
        window2->AddText(1, CRGBA(255, 255, 255));


        window2->showPageControls = true;
        window2->btnBack->onClick = [window2]() {
            window2->GoToPrevWindow();
        };
    };


   /*
    window->AddText(1, CRGBA(255, 255, 255));
    window->AddText(0, CRGBA(255, 255, 255));
    window->AddText(2, CRGBA(255, 255, 255));
    window->AddText(3, CRGBA(255, 255, 255));
    window->AddText(4, CRGBA(255, 255, 255));
    */

  
   /*
    window->AddText(999, CRGBA(255, 255, 255));

    auto button2 = window->AddButton(1, { 200, 290 }, { 0, 0, 255, 255 });
    button2->text->num1 = 11;
    button2->text->num2 = 22;

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
