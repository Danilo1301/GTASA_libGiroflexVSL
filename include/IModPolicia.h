#pragma once

//Interface version: 1.5.0

class IModPolicia {
public:
    virtual bool IsRadioOpen() = 0;
    virtual bool IsMenuOpen() = 0;
};