#pragma once

//Interface version: 3.4.0

class IGiroflexVSL {
public:
    virtual bool HasModelInfo(int modelId) = 0;
    virtual void ToggleGiroflex(int hVehicle) = 0;
    virtual bool GetVehiclePrevLightsState(int hVehicle) = 0;
};