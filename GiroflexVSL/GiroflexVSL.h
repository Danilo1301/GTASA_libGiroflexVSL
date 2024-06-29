#pragma once

#include "IGiroflexVSL.h"

class GiroflexVSL : public IGiroflexVSL {
public:
    bool HasModelInfo(int modelId);
    void ToggleGiroflex(int hVehicle);
    bool GetVehiclePrevLightsState(int hVehicle);
};

extern IGiroflexVSL* giroflexVSL;