#pragma once

#include <map>

struct WidgetData {
    bool isPressed = false;
    bool hasJustPressed = false;
    bool hasJustReleased = false;
    bool hasJustFastReleased = false;
    unsigned int timePressed = 0;
};

class Widgets {
public:
    static std::map<int, WidgetData> m_Widgets;

    static void Update(int dt);

    static void SetWidgetState(int widgetId, bool pressed);

    static bool IsWidgetPressed(int widgetId);
    static bool IsWidgetJustPressed(int widgetId);
    static bool IsWidgetJustReleased(int widgetId);
    static bool IsWidgetJustFastReleased(int widgetId);
};