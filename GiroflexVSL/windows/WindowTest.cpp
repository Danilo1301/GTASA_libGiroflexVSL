#include "WindowTest.h"

Window* WindowTest::m_Window = NULL;

void WindowTest::Create()
{
	if (m_Window) return;

    auto window = m_Window = Menu::AddWindow(6);
    window->position = CVector2D(80, 200);

    auto text_id = window->AddText(0);
    text_id->text->num1 = 522;

    auto button_close = window->AddButton(10, CRGBA(170, 70, 70));
    button_close->onClick = []()
    {
        WindowTest::Remove();
    };
}

void WindowTest::Remove()
{
    m_Window->RemoveThisWindow();
    m_Window = NULL;
}