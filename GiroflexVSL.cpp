#include "GiroflexVSL.h"

#include "menu/Draw.h"
#include "menu/Menu.h"
#include "Input.h"

#include "windows/WindowTest.h"

extern CVector2D *m_vecCachedPos;

const char* GiroflexVSL::m_Version = "3.0.0";

void GiroflexVSL::Update(int dt)
{
    while (Draw::m_DrawItems.size() > 0)
    {
        auto dw = Draw::m_DrawItems[0];
        Draw::m_DrawItems.erase(std::find(Draw::m_DrawItems.begin(), Draw::m_DrawItems.end(), dw));
        delete dw;
    }

    //

    Menu::Update(dt);

    Menu::Draw();

    Input::Update(dt);

    GiroflexVSL::ProcessMenuButtons(dt);
    //

    Draw::DrawBoxWithText(2, 1, 2, {0, 0}, {50, 50}, {255, 0, 0, 255}, {0, 0, 0, 255});

    //

    if (Menu::m_DrawCursor)
    {
        Draw::DrawText(2, m_vecCachedPos->x, m_vecCachedPos->y, CVector2D(20, 300), CRGBA(255, 255, 0));
        Draw::DrawText(2, (int)Menu::m_MenuOffset.x, (int)Menu::m_MenuOffset.y, CVector2D(20, 320), CRGBA(255, 255, 0));
        Draw::DrawText(1, Draw::m_DrawItems.size(), 0, CVector2D(20, 340), CRGBA(255, 255, 0));
        Draw::DrawText(1, dt, 0, CVector2D(20, 360), CRGBA(255, 255, 0));
    }
}

void GiroflexVSL::ProcessMenuButtons(int dt)
{
    GiroflexVSL::ProcessTestMenuButtons(dt);

    //test menu, 5 and 6
    if (Input::GetTouchIdState(6) && Input::GetTouchIdState(5))
    {
        if (Input::GetTouchIdPressTime(6) > 500)
        {
            
        }
    }
}

void GiroflexVSL::ProcessTestMenuButtons(int dt)
{
    //test menu, 5 and 6
    if (Input::GetTouchIdState(6) && Input::GetTouchIdState(5))
    {
        if (Input::GetTouchIdPressTime(6) > 500)
        {
            WindowTest::Create();
        }
    }
}