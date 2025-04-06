#pragma once

class Mod {
public:
    static const char* m_Version;
    static float m_DeltaTime;

    static void Update(int dt);
    static void ProcessMenuButtons(int dt);
    static void ProcessTestMenuButtons(int dt);
};