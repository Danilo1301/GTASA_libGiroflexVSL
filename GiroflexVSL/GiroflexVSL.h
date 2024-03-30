#pragma once

class GiroflexVSL {
public:
    static const char* m_Version;

    static void Update(int dt);
    static void ProcessMenuButtons(int dt);
    static void ProcessTestMenuButtons(int dt);
};