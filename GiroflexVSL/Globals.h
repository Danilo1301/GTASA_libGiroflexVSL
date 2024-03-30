#pragma once

#include "pch.h"

#include "Vehicle.h"

class Globals {
public:
    static CVector m_PlayerPos;
    static int hPlayerVehicle;

    static bool IsPlayerInAnyVehicle();
    static Vehicle* GetPlayerVehicle();
};