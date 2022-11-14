#pragma once

#include "pch.h"

class Mod {
public:
    static char Version[256];
    static int m_DeltaTime;
    static int m_FixLightsScale;

    static void RegisterCorona(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, posStruct const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay);
};