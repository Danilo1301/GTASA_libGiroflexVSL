
#include "Widgets.h"
#include "Log.h"

std::map<int, WidgetData> Widgets::m_Widgets;
const int Widgets::DOUBLE_CLICK_TIME = 120;

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
        } else {
            widgetData->timeNotPressed += dt;
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

            auto timePresed = widgetData->timePressed;
            auto timeNotPresed = widgetData->timeNotPressed;
            auto isQuickClick = timePresed < DOUBLE_CLICK_TIME;

            if(isQuickClick)
            {
                widgetData->hasJustFastReleased = true;
                widgetData->quickClicks++;
            }

            auto tookTooLong = timeNotPresed > DOUBLE_CLICK_TIME;

            if(tookTooLong)
            {
                if(isQuickClick) widgetData->quickClicks = 1;
                else widgetData->quickClicks = 0;
            }

            Log::Level(LOG_LEVEL::LOG_BOTH) << "Input: Widget " << widgetId << " just released after " << widgetData->timePressed << " ms (quickclicks: " << widgetData->quickClicks << ")" << std::endl;
            //Log::Level(LOG_LEVEL::LOG_BOTH) << "time not pressed" << widgetData->timeNotPressed << std::endl;

            widgetData->timeNotPressed = 0;
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

bool Widgets::IsWidgetDoubleClicked(int widgetId)
{
    if(!IsWidgetJustReleased(widgetId)) return false;

    if(m_Widgets[widgetId].quickClicks == 0) return false;

    return m_Widgets[widgetId].quickClicks % 2 == 0;
}
