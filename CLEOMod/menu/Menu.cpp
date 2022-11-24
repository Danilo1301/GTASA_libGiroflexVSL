#include "Menu.h"

#include "../Log.h"
#include "../Input.h"

MenuPopup* Menu::m_PopUp = new MenuPopup();
CVector2D Menu::m_MenuOffset = CVector2D(0, -195.0f);
Window* Menu::m_MainWindow = new Window();
std::vector<Window*> Menu::m_Windows;

Window* Menu::AddWindow(int gxtId)
{
    Window* window = new Window();
    window->titleGtxId = gxtId;

    m_Windows.push_back(window);

    Log::file << "Menu: AddWindow" << std::endl;

    return window;
}

Window* Menu::AddWindow(int gxtId, Window* parent)
{
    Window* window = AddWindow(gxtId);
    window->parentWindow = parent;
    window->position = parent->position;

    parent->visible = false;

    return window;
}

Window* Menu::AddPositionWindow(Window* parent, CVector* vec)
{
    Window* window = AddWindow(26, parent);
    window->width = 200.0f;
    window->position = { 10, 200 };
    window->showPageControls = true;

    window->AddFloatRange(23, &vec->x, -10.0f, 10.0f, 0.01f);
    window->AddFloatRange(24, &vec->y, -10.0f, 10.0f, 0.01f);
    window->AddFloatRange(25, &vec->z, -10.0f, 10.0f, 0.01f);

    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    return window;
}


Window* Menu::AddPositionWindow(Window* parent, CVector2D* vec, float min, float max, float addBy)
{
    Window* window = AddWindow(26, parent);
    window->width = 200.0f;
    window->position = { 10, 200 };
    window->showPageControls = true;

    window->AddFloatRange(23, &vec->x, min, max, addBy);
    window->AddFloatRange(24, &vec->y, min, max, addBy);

    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    return window;
}


static int colormenu_r = 0;
static int colormenu_g = 0;
static int colormenu_b = 0;
static int colormenu_a = 0;

Window* Menu::AddColorMenu(Window* parent, CRGBA* color)
{
    Window* window = AddWindow(46, parent);
    window->width = 200.0f;
    window->position = { 10, 200 };
    window->showPageControls = true;

    colormenu_r = color->r;
    colormenu_g = color->g;
    colormenu_b = color->b;
    colormenu_a = color->a;

    auto option_r = window->AddIntRange(20, &colormenu_r, 0, 255, 1);
    auto option_g = window->AddIntRange(21, &colormenu_g, 0, 255, 1);
    auto option_b = window->AddIntRange(22, &colormenu_b, 0, 255, 1);
    auto option_a = window->AddIntRange(28, &colormenu_a, 0, 255, 1);

    option_r->onValueChange = [color]() { color->r = colormenu_r; };
    option_g->onValueChange = [color]() { color->g = colormenu_g; };
    option_b->onValueChange = [color]() { color->b = colormenu_b; };
    option_a->onValueChange = [color]() { color->a = colormenu_a; };

    window->btnBack->onClick = [window]()
    {
        window->GoToPrevWindow();
    };

    return window;
}

Window* Menu::AddConfirmWindow(Window* parent, int textGxtId, std::function<void()> ohYes, std::function<void()> ohNo)
{
    auto screenSize = Input::GetGTAScreenSize();

    Window* window = AddWindow(32, parent);
    window->width = 200.0f;
    window->position = { screenSize.x/2 - window->width/2, 200 };

    window->AddText(textGxtId, CRGBA(255, 255, 255));

    auto button_yes = window->AddButton(48);
    button_yes->onClick = [window, ohYes]()
    {
        window->GoToPrevWindow();
        ohYes();
    };

    auto button_no = window->AddButton(49, CRGBA(170, 70, 70));
    button_no->onClick = [window, ohNo]()
    {
        window->GoToPrevWindow();
        ohNo();
    };

    return window;
}

void Menu::ShowPopup(int gfxId, int val1, int val2, int time)
{
    m_PopUp->gfxId = gfxId;
    m_PopUp->val1 = val1;
    m_PopUp->val2 = val2;
    m_PopUp->timeLeft = time;
}

void Menu::Update(int dt)
{
    m_PopUp->timeLeft -= dt;
    if (m_PopUp->timeLeft < 0) m_PopUp->timeLeft = 0;


    m_MainWindow->Update();

    for (auto window : m_Windows)
    {
        window->Update();
    }
}

void Menu::Draw()
{
    m_MainWindow->showTitle = false;
    m_MainWindow->Draw();

    for (auto window : m_Windows)
    {
        window->Draw();
    }

    if (m_PopUp->timeLeft > 0)
    {
        float x = 20;
        float y = 200;

        float titleH = 20;
        float hoxH = 60;

        float w = 200;

        CRGBA white = { 255, 255, 255, 255 };

        Draw::DrawBoxWithText(32, 100, 200, { x, y }, { w, titleH }, { 0, 77, 132, 255 }, white);
        y += 20;
        Draw::DrawBoxWithText(m_PopUp->gfxId, m_PopUp->val1, m_PopUp->val2, { x, y }, { w, hoxH }, { 0, 119, 204, 255 }, white);
    }
    
    //cursor
    /*
    CVector2D pos = Input::GetTouchPos();
    CRGBA colorOn = { 0, 255, 0, 255 };
    CRGBA colorOff = { 255, 0, 0, 255 };

    Draw::DrawBox({ pos.x - 3, pos.y - 3 }, { 6, 6 }, (Input::isTouchPressed ? colorOn : colorOff));
    */
}

void Menu::RemoveWindow(Window* window)
{
    if (window == m_MainWindow) return;

    Log::file << "Menu: RemoveWindow" << std::endl;

    auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
    if (it == m_Windows.end()) return;

    //if (m_ActiveWindow == window) m_ActiveWindow = nullptr;

    window->Destroy();
    m_Windows.erase(it);
    delete window;
}

void Menu::RemoveAllWindows()
{
    while (m_Windows.size() > 0) {
        RemoveWindow(m_Windows[0]);
    }
}