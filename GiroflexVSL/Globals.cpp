#include "Globals.h"

#include "Vehicles.h"

CVector Globals::m_PlayerPos = CVector(0, 0, 0);
int Globals::hPlayerVehicle = -1;
int Globals::hPrevUsedPlayerVehicle = -1;
bool Globals::m_UsingMultiSiren = false;

bool Globals::IsPlayerInAnyVehicle()
{
    if (hPlayerVehicle <= 0) return false;
    if (!Vehicles::HasVehicleHandle(hPlayerVehicle)) return false;
    return true;
}

Vehicle* Globals::GetPlayerVehicle()
{
    if (!IsPlayerInAnyVehicle()) return NULL;
    return Vehicles::m_Vehicles.at(hPlayerVehicle);
}
