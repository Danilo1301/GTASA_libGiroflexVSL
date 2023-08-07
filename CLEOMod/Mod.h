#pragma once

#include "pch.h"
#include "Vehicle.h"

enum eCoronaFixFPS
{
    FPS_30,
    FPS_45,
    FPS_60,
    FPS_AUTO
};



class Mod {
public:
    static char Version[256];
    static int m_PrevDeltaTime;
    static int m_DeltaTime;
    static eCoronaFixFPS CoronaFixFPS;
    static uintptr_t* pVehiclePool;
    static void* hGTASA;
    static bool HasShownCredits;
    static CVector m_PlayerPos;
    static int hPlayerVehicle;
    static bool IsPlayerInAnyVehicle();
    static Vehicle* GetPlayerVehicle();
    static void SaveCfg();

    static void ProcessTouch();

    static void* ModGetVehicleFromRef(int h);
    static int ModGetVehicleRef(int pVehicle);
};