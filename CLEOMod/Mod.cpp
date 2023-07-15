#include "Mod.h"

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include <stdint.h>
#include <dlfcn.h>

#include "Log.h"
#include "Vehicles.h"
#include "eDrawInfoType.h"
#include "Input.h"
#include "Patterns.h"
#include "ModelInfos.h"

#include "menu/Draw.h"
#include "menu/Menu.h"

#include "windows/WindowMain.h"
#include "windows/WindowSettings.h"

#include "opcodes.h"

char Mod::Version[256] = "2.6.1";
int Mod::m_PrevDeltaTime = 0;
int Mod::m_DeltaTime = 0;
eCoronaFixFPS Mod::CoronaFixFPS = eCoronaFixFPS::FPS_AUTO;
uintptr_t Mod::pVehiclePool = 0;
void* Mod::hGTASA = 0;

CVector Mod::m_PlayerPos = CVector(0, 0, 0);
int Mod::hPlayerVehicle = -1;



MYMODCFG(net.danilo1301.giroflex, Giroflex, Mod::Version, Danilo1301)
BEGIN_DEPLIST()
//ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0.0.6)
END_DEPLIST()




uintptr_t pGTASA = 0;

unsigned int uniqueLightId = 65487;

bool canTurnSirenOn = true;
bool canTurnPanelOn = true;

ConfigEntry* cfgMenuOffsetX = NULL;
ConfigEntry* cfgTimeBetweenPatterns = NULL;
ConfigEntry* cfgCoronaFpsFix = NULL;

//float menuOffsets[3] = { -195.0f, 0.0f, 195.0f };

bool Mod::IsPlayerInAnyVehicle()
{
    if (hPlayerVehicle <= 0) return false;
    if (!Vehicles::HasVehicleHandle(hPlayerVehicle)) return false;
    return true;
}

Vehicle* Mod::GetPlayerVehicle()
{
    return Vehicles::m_Vehicles.at(hPlayerVehicle);
}

void Mod::SaveCfg()
{
    cfgMenuOffsetX->SetInt((int)Menu::m_MenuOffset.x);
    cfgTimeBetweenPatterns->SetInt(Patterns::m_TimeBetweenPatterns);
    cfgCoronaFpsFix->SetInt((int)Mod::CoronaFixFPS);

    cfg->Save();
}

void Mod::ProcessTouch()
{
    if (Input::GetTouchIdState(5) && Input::GetTouchIdState(9))
    {
        if (Input::GetTouchIdPressTime(5) > 500 && Input::GetTouchIdPressTime(9) > 500)
        {

            if (IsPlayerInAnyVehicle())
            {
                auto veh = Mod::GetPlayerVehicle();

                if (ModelInfos::HasModelInfo(veh->modelId))
                {
                    if (canTurnPanelOn)
                    {
                        canTurnPanelOn = false;

                        WindowPanel::Toggle(!WindowPanel::Visible);
                    }
                }
                else {
                    Menu::ShowPopup(16, 0, 0, 1000);
                }
            }
            else {
                Menu::ShowPopup(16, 0, 0, 1000);
            }
        }
    }
    else {
        canTurnPanelOn = true;
    }

    if (Input::GetTouchIdState(6) && Input::GetTouchIdState(5))
    {
        if (Input::GetTouchIdPressTime(6) > 500)
        {
            if (!IsPlayerInAnyVehicle())
            {
                Menu::ShowPopup(16, 0, 0, 1000);
                return;
            }

            auto vehicle = GetPlayerVehicle();

            if (canTurnSirenOn)
            {
                canTurnSirenOn = false;

                vehicle->SetGiroflexEnabled(!vehicle->prevLightsState);
            }

            if (Input::GetTouchIdPressTime(6) > 1000)
            {
                if (!WindowMain::m_Window)
                {
                    vehicle->SetGiroflexEnabled(true);

                    WindowMain::Create(vehicle->modelId);

                }
            }
        }
    }
    else {
        canTurnSirenOn = true;
    }
}

//---------------------------------------------------------------------------------------------------
//test

const char* optionsGiroflexEditMode[] = {
    "OFF",
    "ON"
};
void OnGiroflexEditModeChanged(int oldVal, int newVal)
{
    Log::file << "OnGiroflexEditModeChanged - changed to " << newVal << std::endl;

    if (newVal == 1)
    {
        WindowSettings::ToggleEditScreenPos(true);
    }
    else {
        WindowSettings::ToggleEditScreenPos(false);

        Mod::SaveCfg();
    }
}
//---------------------------------------------------------------------------------------------------


std::string CheckModVersion(std::vector<std::string> GUIDs, std::vector<std::string> versions)
{
    for (int ii = GUIDs.size() - 1; ii >= 0; ii--)
    {
        auto GUID = GUIDs[ii];

        for (int i = versions.size() - 1; i >= 0; i--)
        {
            std::string version = versions[i];

            Log::file << "Checking " << GUID << " " << version << "..." << std::endl;

            if (aml->HasModOfVersion(GUID.c_str(), version.c_str()))
                return version;
        }
    }
    return "";
}


extern "C" void OnModPreLoad()
{

    ModConfig::MakePaths();

    std::string configPath = aml->GetConfigPath();

    bool insideCleo = false;
    //if (configPath.find("rockstargames") != std::string::npos) insideCleo = false;

    if (insideCleo)
    {
        Log::file.open("/storage/emulated/0/cleo/giroflex.log", std::fstream::out | std::fstream::trunc);
        //Log::opcodes.open("/storage/emulated/0/cleo/giroflex_opcodes.log", std::fstream::out | std::fstream::trunc);
    }
    else {
        Log::file.open(configPath + "/giroflex/giroflex.log", std::fstream::out | std::fstream::trunc);
        //Log::opcodes.open(configPath + "/giroflex/giroflex_opcodes.log", std::fstream::out | std::fstream::trunc);
    }

    Log::file << "Preload..." << std::endl;
    //Log::opcodes << "Preload..." << std::endl;

    logger->SetTag("Giroflex");
}


//

//

extern "C" void OnModLoad()
{
    Log::file << "Load..." << std::endl;

    std::string cleoVersion = CheckModVersion(
        { "net.rusjj.cleolib", "net.rusjj.cleomod" },
        { "2.0.1", "2.0.1.1", "2.0.1.2", "2.0.1.3" }
    );

    std::string sautilsVersion = CheckModVersion(
        { "net.rusjj.gtasa.utils" },
        { "1.1", "1.2", "1.2.1", "1.3.0", "1.3.1" }
    );

    std::string amlVersion = CheckModVersion(
        { "net.rusjj.aml" },
        { "1.0.0.0", "1.0.0.1", "1.0.0.2", "1.0.0.3", "1.0.0.4", "1.0.0.5", "1.0.0.6" }
    );


    /*
      tested aml
      1.0.0.0
      1.0.0.6
      tested cleo
      2.0.1.3
      2.0.1 2
      2.0.1.1 ?
      2.0.1  ?
    */

    cfgMenuOffsetX = cfg->Bind("menu_offset_x", -195, "General");
    cfgTimeBetweenPatterns = cfg->Bind("time_between_patterns", Patterns::m_TimeBetweenPatterns, "General");
    cfgCoronaFpsFix = cfg->Bind("corona_fps_fix", Mod::CoronaFixFPS, "General");

    Menu::m_MenuOffset.x = (float)cfgMenuOffsetX->GetInt();
    Patterns::m_TimeBetweenPatterns = cfgTimeBetweenPatterns->GetInt();
    Mod::CoronaFixFPS = (eCoronaFixFPS)cfgCoronaFpsFix->GetInt();

    Mod::SaveCfg();

    //cfg->Bind("pos.x", 45, "523");
    //cfg->Save();

    Log::file << "------------------------" << std::endl;
    Log::file << "Giroflex version: " << Mod::Version << std::endl;
    Log::file << "SAUtils version: " << sautilsVersion << "  (expected 1.3.1)" << std::endl;
    Log::file << "AML version: " << amlVersion << "  (expected 1.0.0.6)" << std::endl;
    Log::file << "CLEO version: " << cleoVersion << "  (expected 2.0.1.3)" << std::endl;
    Log::file << "Config: " << aml->GetConfigPath() << std::endl;

    //CLEO
    void* cleoInterface = GetInterface("CLEO");
    if (!cleoInterface)
    {
        Log::file << "CLEO interface not found!" << std::endl;
        return;
    }

    if (aml->HasModOfVersion("net.rusjj.cleomod", "2.0.1.3")) //2.0.1.3 or above
    {
        cleo2013 = (cleo_ifs_t*)cleoInterface;
        Log::file << "CLEO interface: (cleo_ifs_t)" << std::endl;
    }
    else {
        cleo2012 = (ICLEO*)cleoInterface;
        Log::file << "CLEO interface: (ICLEO)" << std::endl;
    }

    Log::file << "------------------------" << std::endl;




    //opcodes
    Log::file << "Registering opcodes..." << std::endl;

    if (cleo2012)
    {
        __reg_op_func2012(SEND_PLAYER_POSITION, SEND_PLAYER_POSITION);
        __reg_op_func2012(SEND_CAR_POSITION, SEND_CAR_POSITION);
        __reg_op_func2012(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
        __reg_op_func2012(PROCESS_GIROFLEX_LIB, PROCESS_GIROFLEX_LIB);
        __reg_op_func2012(RUN_TEST, RUN_TEST);
        //__reg_op_func2012(TOGGLE_GIROFLEX_MENU, TOGGLE_GIROFLEX_MENU);
        //__reg_op_func2012(TOGGLE_GIROFLEX, TOGGLE_GIROFLEX);
        __reg_op_func2012(SEND_CURRENT_VEHICLE, SEND_CURRENT_VEHICLE);
        __reg_op_func2012(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
        __reg_op_func2012(REGISTER_GIROFLEX_CORONA, REGISTER_GIROFLEX_CORONA);
        __reg_op_func2012(SEND_CAR_VELOCITY, SEND_CAR_VELOCITY);
    }
    else {
        __reg_op_func2013(SEND_PLAYER_POSITION, SEND_PLAYER_POSITION);
        __reg_op_func2013(SEND_CAR_POSITION, SEND_CAR_POSITION);
        __reg_op_func2013(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
        __reg_op_func2013(PROCESS_GIROFLEX_LIB, PROCESS_GIROFLEX_LIB);
        __reg_op_func2013(RUN_TEST, RUN_TEST);
        //__reg_op_func2013(TOGGLE_GIROFLEX_MENU, TOGGLE_GIROFLEX_MENU);
        //__reg_op_func2013(TOGGLE_GIROFLEX, TOGGLE_GIROFLEX);
        __reg_op_func2013(SEND_CURRENT_VEHICLE, SEND_CURRENT_VEHICLE);
        __reg_op_func2013(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
        __reg_op_func2013(REGISTER_GIROFLEX_CORONA, REGISTER_GIROFLEX_CORONA);
        __reg_op_func2013(SEND_CAR_VELOCITY, SEND_CAR_VELOCITY);

    }

    //libGTASA
    Log::file << "Loading libGTASA..." << std::endl;
    void* hGTASA = Mod::hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    pGTASA = aml->GetLib("libGTASA.so");

    //

    Log::file << "Finding pointers..." << std::endl;

    Input::pScreenGetWidth = aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv");
    Input::pScreenGetHeight = aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv");

    //SET_TO(ScreenGetInches, aml->GetSym(hGTASA, "_Z18OS_ScreenGetInchesv"));
    SET_TO(RegisterCorona, aml->GetSym(hGTASA, "_ZN8CCoronas14RegisterCoronaEjP7CEntityhhhhRK7CVectorffhhhhhfbfbfbb"));

    Draw::pPrintString = aml->GetSym(hGTASA, "_ZN5CFont11PrintStringEffPt");
    Input::pTouchPos = aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE");
    Mod::pVehiclePool = aml->GetSym(hGTASA, "_ZN6CPools15ms_pVehiclePoolE");

    

    //

    Log::file << "pScreenGetHeight = " << Input::pScreenGetHeight << std::endl;
    Log::file << "pScreenGetWidth = " << Input::pScreenGetWidth << std::endl;
    Log::file << "RegisterCorona = " << &RegisterCorona << std::endl;

    Log::file << "pTouchPos = " << Input::pTouchPos << std::endl;
    Log::file << "pVehiclePool = " << Mod::pVehiclePool << std::endl;

    //Log::file << "ScreenGetInches() = " << ScreenGetInches() << std::endl;

    Log::file << "Loaded" << std::endl;

    

    ModConfig::Load();

    if (Patterns::m_Patterns.size() == 0)
    {
        auto pattern1 = Patterns::CreatePattern("1_lights-1");
        pattern1->AddStep({ 0 }, 300);
        pattern1->AddStep({ 1 }, 300);
        /*
        pattern.push({values: [0], time: 300});
        pattern.push({values: [1], time: 300});
        */

        auto pattern2 = Patterns::CreatePattern("1_lights-2");
        pattern2->AddStep({ 1 }, 80);
        pattern2->AddStep({ 0 }, 80);
        pattern2->AddStep({ 1 }, 80);
        pattern2->AddStep({ 0 }, 600);
        /*
        pattern.push({values: [1], time: 80});
        pattern.push({values: [0], time: 80});
        pattern.push({values: [1], time: 80});
        pattern.push({values: [0], time: 600});;
        */

        auto pattern3 = Patterns::CreatePattern("2_lights-1");
        pattern3->AddStep({ 1, 0 }, 200);
        pattern3->AddStep({ 0, 0 }, 200);
        pattern3->AddStep({ 0, 1 }, 200);
        pattern3->AddStep({ 0, 0 }, 200);
        /*
        pattern.push({values: [1, 0], time: 200});
        pattern.push({values: [0, 0], time: 200});
        pattern.push({values: [0, 1], time: 200});
        pattern.push({values: [0, 0], time: 200});
        */

        auto pattern4 = Patterns::CreatePattern("2_lights-2");
        pattern4->AddStep({ 1, 0 }, 100);
        pattern4->AddStep({ 0, 0 }, 80);
        pattern4->AddStep({ 1, 0 }, 100);
        pattern4->AddStep({ 0, 0 }, 80);
        pattern4->AddStep({ 1, 0 }, 100);
        pattern4->AddStep({ 0, 0 }, 150);
        pattern4->AddStep({ 0, 1 }, 100);
        pattern4->AddStep({ 0, 0 }, 80);
        pattern4->AddStep({ 0, 1 }, 100);
        pattern4->AddStep({ 0, 0 }, 80);
        pattern4->AddStep({ 0, 1 }, 100);
        pattern4->AddStep({ 0, 0 }, 150);
        /*
        pattern.push({values: [1, 0], time: 100});
        pattern.push({values: [0, 0], time: 80});
        pattern.push({values: [1, 0], time: 100});
        pattern.push({values: [0, 0], time: 80});
        pattern.push({values: [1, 0], time: 100});
        pattern.push({values: [0, 0], time: 150});
        pattern.push({values: [0, 1], time: 100});
        pattern.push({values: [0, 0], time: 80});
        pattern.push({values: [0, 1], time: 100});
        pattern.push({values: [0, 0], time: 80});
        pattern.push({values: [0, 1], time: 100});
        pattern.push({values: [0, 0], time: 150});
        */

        auto pattern5 = Patterns::CreatePattern("5_lights-1");
        pattern5->AddStep({ 1, 0, 0, 0, 1 }, 100);
        pattern5->AddStep({ 0, 0, 0, 0, 0 }, 100);
        pattern5->AddStep({ 0, 1, 0, 1, 0 }, 100);
        pattern5->AddStep({ 0, 0, 0, 0, 0 }, 100);
        pattern5->AddStep({ 0, 0, 1, 0, 0 }, 100);
        /*
        pattern.push({values: [1, 0, 0, 0, 1], time: 100});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        pattern.push({values: [0, 1, 0, 1, 0], time: 100});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        pattern.push({values: [0, 0, 1, 0, 0], time: 100});
        */

        auto pattern6 = Patterns::CreatePattern("5_lights-2");
        pattern6->AddStep({ 1, 0, 0, 0, 1 }, 220);
        pattern6->AddStep({ 0, 0, 0, 0, 0 }, 100);
        pattern6->AddStep({ 0, 0, 1, 0, 0 }, 100);
        pattern6->AddStep({ 0, 0, 0, 0, 0 }, 100);
        /*
        pattern.push({values: [1, 0, 0, 0, 1], time: 220});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        pattern.push({values: [0, 0, 1, 0, 0], time: 100});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        */

        auto pattern7 = Patterns::CreatePattern("5_lights-3");
        pattern7->AddStep({ 1, 1, 0, 0, 0 }, 150);
        pattern7->AddStep({ 0, 0, 0, 1, 1 }, 150);
        pattern7->AddStep({ 0, 0, 0, 0, 0 }, 100);
        pattern7->AddStep({ 1, 1, 0, 0, 0 }, 150);
        pattern7->AddStep({ 0, 0, 0, 1, 1 }, 150);
        pattern7->AddStep({ 0, 0, 0, 0, 0 }, 100);
        /*
        pattern.push({values: [1, 1, 0, 0, 0], time: 150});
        pattern.push({values: [0, 0, 0, 1, 1], time: 150});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        pattern.push({values: [1, 1, 0, 0, 0], time: 150});
        pattern.push({values: [0, 0, 0, 1, 1], time: 150});
        pattern.push({values: [0, 0, 0, 0, 0], time: 100});
        */


        auto pattern8 = Patterns::CreatePattern("5_lights-4");
        pattern8->AddStep({ 1, 0, 0, 0, 1 }, 100);
        pattern8->AddStep({ 0, 1, 0, 1, 0 }, 100);
        pattern8->AddStep({ 0, 0, 1, 0, 0 }, 100);
        pattern8->AddStep({ 0, 1, 0, 1, 0 }, 100);
        pattern8->AddStep({ 1, 0, 0, 0, 1 }, 100);
        pattern8->AddStep({ 0, 0, 0, 0, 0 }, 200);
        /*
        pattern.push({values: [1, 0, 0, 0, 1], time: 100});
        pattern.push({values: [0, 1, 0, 1, 0], time: 100});
        pattern.push({values: [0, 0, 1, 0, 0], time: 100});
        pattern.push({values: [0, 1, 0, 1, 0], time: 100});
        pattern.push({values: [1, 0, 0, 0, 1], time: 100});
        pattern.push({values: [0, 0, 0, 0, 0], time: 200});
        */

        auto pattern_10lights_1 = Patterns::CreatePattern("10lights_1");
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 200);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, 100);
        pattern_10lights_1->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 200);

        auto pattern_10lights_2 = Patterns::CreatePattern("10lights_2");
        pattern_10lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 60);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 100);
        pattern_10lights_2->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 100);

        auto pattern_10lights_3 = Patterns::CreatePattern("10lights_3");
        pattern_10lights_3->AddStep({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 150);
        pattern_10lights_3->AddStep({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 150);

        ModConfig::Save();
    }

    //WindowMain::Create(523);

    //SAUtils
    Log::file << "Loading SAUtils..." << std::endl;
    sautils = (ISAUtils*)GetInterface("SAUtils");
    if (sautils)
    {
        //sautils->AddButton(SetType_Mods, "Giroflex VSL - Edit mode", OnEditModeButtonPressed);

        sautils->AddClickableItem(SetType_Mods, "Giroflex VSL - Edit mode", 0, 0, sizeofA(optionsGiroflexEditMode) - 1, optionsGiroflexEditMode, OnGiroflexEditModeChanged);

        //sautils->AddSliderItem(SetType_Mods, "Giroflex Menu Offset", cfgMenuOffsetX->GetInt(), -200, 200, OnMenuOffsetChanged);
        
        Log::file << "SAUtils Loaded" << std::endl;
    }
}