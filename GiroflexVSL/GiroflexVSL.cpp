#include "GiroflexVSL.h"

#include "Log.h"
#include "ModelInfos.h"
#include "Vehicles.h"

bool GiroflexVSL::HasModelInfo(int modelId)
{
    return ModelInfos::HasModelInfo(modelId);
}

void GiroflexVSL::ToggleGiroflex(int hVehicle)
{   
    if(!Vehicles::HasVehicleHandle(hVehicle)) return;

    auto vehicle = Vehicles::GetVehicleByHandle(hVehicle);

    auto prevLightsState = vehicle->prevLightsState;

    vehicle->SetGiroflexEnabled(!prevLightsState);
}

bool GiroflexVSL::GetVehiclePrevLightsState(int hVehicle)
{   
    if(!Vehicles::HasVehicleHandle(hVehicle)) return false;

    auto vehicle = Vehicles::GetVehicleByHandle(hVehicle);

    return vehicle->prevLightsState;
}

static GiroflexVSL giroflexVSLLocal;
IGiroflexVSL* giroflexVSL = &giroflexVSLLocal;