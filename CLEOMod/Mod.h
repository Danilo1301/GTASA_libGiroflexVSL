#pragma once

#include "pch.h"
#include "Vehicle.h"

class Mod {
public:
    static char Version[256];
    static int m_PrevDeltaTime;
    static int m_DeltaTime;
    static uintptr_t pVehiclePool;
    static void* hGTASA;

    static CVector m_PlayerPos;
    static int hPlayerVehicle;
    static bool IsPlayerInAnyVehicle();
    static Vehicle* GetPlayerVehicle();

    static void ProcessTouch();
};