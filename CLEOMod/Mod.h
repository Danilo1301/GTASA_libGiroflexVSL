#pragma once

#include "pch.h"

class Mod {
public:
    static char Version[256];
    static int m_PrevDeltaTime;
    static int m_DeltaTime;

    static float m_CoronaLerpNormal;
    static float m_CoronaLerpTooMuch;
    static int m_CoronaFixMinChange;

    static void RegisterCorona(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, posStruct const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay);
};