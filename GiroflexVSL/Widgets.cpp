#include "Widgets.h"
#include "Log.h"

std::map<int, WidgetData> Widgets::m_Widgets;

void Widgets::Update(int dt)
{
    for(auto& pair : m_Widgets)
    {
        int widgetId = pair.first;
        WidgetData* widgetData = &m_Widgets[widgetId]; //imporant, using pointer instead

        if(widgetData->isPressed)
        {
            widgetData->timePressed += dt;
            Log::Level(LOG_LEVEL::LOG_UPDATE) << "Widget " << widgetId << " pressTime: " << widgetData->timePressed << std::endl;
        }

        widgetData->hasJustPressed = false;
        widgetData->hasJustReleased = false;
        widgetData->hasJustFastReleased = false;
    }
}

void Widgets::SetWidgetState(int widgetId, bool pressed)
{
    if (m_Widgets.find(widgetId) == m_Widgets.end())
    {
        WidgetData data;
        data.isPressed = false;
        data.timePressed = 0;
        m_Widgets[widgetId] = data;
    }

    auto widgetData = &m_Widgets[widgetId];

    if (widgetData->isPressed != pressed)
    {
        if (pressed)
        {
            widgetData->hasJustPressed = true;
            Log::Level(LOG_LEVEL::LOG_BOTH) << "Input: Widget " << widgetId << " just pressed" << std::endl;
        }
        else {
            widgetData->hasJustReleased = true;
            if(widgetData->timePressed < 200) widgetData->hasJustFastReleased = true;
            Log::Level(LOG_LEVEL::LOG_BOTH) << "Input: Widget " << widgetId << " just released after " << widgetData->timePressed << " ms" << std::endl;
        }

        widgetData->isPressed = pressed;
        widgetData->timePressed = 0;
    }
}

bool Widgets::IsWidgetPressed(int widgetId)
{
    return m_Widgets[widgetId].isPressed;
}

bool Widgets::IsWidgetJustPressed(int widgetId)
{
    return m_Widgets[widgetId].hasJustPressed;
}

bool Widgets::IsWidgetJustReleased(int widgetId)
{
    return m_Widgets[widgetId].hasJustReleased;
}


bool Widgets::IsWidgetJustFastReleased(int widgetId)
{
    return m_Widgets[widgetId].hasJustFastReleased;
}