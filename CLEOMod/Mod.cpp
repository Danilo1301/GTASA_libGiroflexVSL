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
#include "AudioStream.h"

#include "menu/Draw.h"
#include "menu/Menu.h"

#include "windows/WindowMain.h"
#include "windows/WindowSettings.h"
#include "windows/WindowSelectPanel.h"

#include "opcodes.h"

char Mod::Version[256] = "2.10.2";
int Mod::m_PrevDeltaTime = 0;
int Mod::m_DeltaTime = 0;
eCoronaFixFPS Mod::CoronaFixFPS = eCoronaFixFPS::FPS_AUTO; //remove later
uintptr_t* Mod::pVehiclePool = 0;
void* Mod::hGTASA = 0;
bool Mod::HasShownCredits = false;

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

                        WindowSelectPanel::Create();
                        //WindowPanel::Toggle(!WindowPanel::Visible);
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

void* Mod::ModGetVehicleFromRef(int h)
{
    if (!GetVehicleFromRef)
    {
        Log::file << "GetVehicleFromRef " << h << " is NULL" << std::endl;
        return NULL;
    }
    void* result = GetVehicleFromRef(h);

    //Log::file << "GetVehicleFromRef " << h << " is " << result << std::endl;

    return result;
}
int Mod::ModGetVehicleRef(int pVehicle)
{
    if (!GetVehicleRef)
    {
        Log::file << "GetVehicleRef " << pVehicle << " is NULL" << std::endl;
        return NULL;
    }

    return GetVehicleRef(pVehicle);
}
//---------------------------------------------------------------------------------------------------
//test

const char* optionsGiroflexEditMode[] = {
    "OFF",
    "ON"
};
void OnGiroflexEditModeChanged(int oldVal, int newVal, void* data)
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

    cfgMenuOffsetX = cfg->Bind("menu_offset_x", -195, "General");
    cfgTimeBetweenPatterns = cfg->Bind("time_between_patterns", Patterns::m_TimeBetweenPatterns, "General");
    cfgCoronaFpsFix = cfg->Bind("corona_fps_fix", Mod::CoronaFixFPS, "General");

    Menu::m_MenuOffset.x = (float)cfgMenuOffsetX->GetInt();
    Patterns::m_TimeBetweenPatterns = cfgTimeBetweenPatterns->GetInt();
    Mod::CoronaFixFPS = (eCoronaFixFPS)cfgCoronaFpsFix->GetInt();

    Mod::SaveCfg();

    std::string cleoVersion = CheckModVersion(
        { "net.rusjj.cleolib", "net.rusjj.cleomod" },
        { "2.0.1", "2.0.1.1", "2.0.1.2", "2.0.1.3" }
    );

    std::string sautilsVersion = CheckModVersion(
        { "net.rusjj.gtasa.utils" },
        { "1.1", "1.2", "1.2.1", "1.3.0", "1.3.1", "1.4", "1.4.1" }
    );

    std::string amlVersion = CheckModVersion(
        { "net.rusjj.aml" },
        { "1.0.0.0", "1.0.0.1", "1.0.0.2", "1.0.0.3", "1.0.0.4", "1.0.0.5", "1.0.0.6", "1.0.1", "1.0.2", "1.0.2.1", "1.0.2.2", "1.0.3", "1.0.3.1" }
    );

    Log::file << "------------------------" << std::endl;
    Log::file << "Game: " << aml->GetCurrentGame() << std::endl;
    Log::file << "Giroflex version: " << Mod::Version << std::endl;

    Log::file << "CLEO version: " << cleoVersion << "  (recommended 2.0.1.3)" << std::endl;
    Log::file << "SAUtils version: " << sautilsVersion << "  (recommended 1.3.1)" << std::endl;
    Log::file << "AML version: " << amlVersion << "  (recommended 1.0.3.1)" << std::endl;

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

    if (cleo2013)
    {
        Log::file << "Game version: " << cleo2013->GetGameVersionInternal() << std::endl;
    }
    else {
        Log::file << "Game version: " << cleo2012->GetGameVersionInternal() << std::endl;
    }

    Log::file << "------------------------" << std::endl;

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
        __reg_op_func2012(CREATE_NEW_VEHICLE, CREATE_NEW_VEHICLE);
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
        __reg_op_func2013(CREATE_NEW_VEHICLE, CREATE_NEW_VEHICLE);

    }

    //libGTASA
    Log::file << "Loading libGTASA..." << std::endl;
    void* hGTASA = Mod::hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    pGTASA = aml->GetLib("libGTASA.so");

    //

    Log::file << "Finding pointers..." << std::endl;

    Input::pScreenGetWidth = aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv");
    Input::pScreenGetHeight = aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv");
    Draw::pPrintString = aml->GetSym(hGTASA, "_ZN5CFont11PrintStringEffPt");
    Input::pTouchPos = aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE");

    Log::file << "pScreenGetWidth = " << Input::pScreenGetWidth << std::endl;
    Log::file << "pScreenGetHeight = " << Input::pScreenGetHeight << std::endl;
    Log::file << "pPrintString = " << Draw::pPrintString << std::endl;
    Log::file << "pTouchPos = " << Input::pTouchPos << std::endl;

    //SET_TO(ScreenGetInches, aml->GetSym(hGTASA, "_Z18OS_ScreenGetInchesv"));
    SET_TO(RegisterCorona, aml->GetSym(hGTASA, "_ZN8CCoronas14RegisterCoronaEjP7CEntityhhhhRK7CVectorffhhhhhfbfbfbb"));
    SET_TO(GetVehicleRef, aml->GetSym(hGTASA, "_ZN6CPools13GetVehicleRefEP8CVehicle"));
    SET_TO(GetVehicleFromRef, aml->GetSym(hGTASA, "_ZN6CPools10GetVehicleEi"));
    SET_TO(Mod::pVehiclePool, aml->GetSym(hGTASA, "_ZN6CPools15ms_pVehiclePoolE"));

    Log::file << "RegisterCorona = " << (void*)RegisterCorona << std::endl;
    Log::file << "GetVehicleRef = " << (void*)GetVehicleRef << std::endl;
    Log::file << "GetVehicleFromRef = " << (void*)GetVehicleFromRef << std::endl;
    Log::file << "pVehiclePool = " << Mod::pVehiclePool << std::endl;

    //Log::file << "ScreenGetInches() = " << ScreenGetInches() << std::endl;

    Log::file << "Load" << std::endl;

    ModConfig::ProcessVersionChanges_PreConfigLoad();

    ModConfig::Load();

    if (Patterns::m_Patterns.size() == 0)
    {
        Patterns::CreateDefaultPatterns();
    }

    ModConfig::ProcessVersionChanges_PostConfigLoad();

    ModConfig::Save();

    //WindowMain::Create(523);

    //SAUtils
    Log::file << "Loading SAUtils..." << std::endl;
    if (!(sautils = (ISAUtils*)GetInterface("SAUtils")))
    {
        Log::file << "SAUtils was not loaded" << std::endl;
    } else {
        Log::file << "SAUtils loaded" << std::endl;

        sautils->AddClickableItem(eTypeOfSettings::SetType_Mods, "Giroflex VSL - Edit mode", 0, 0, sizeofA(optionsGiroflexEditMode) - 1, optionsGiroflexEditMode, OnGiroflexEditModeChanged);
    
        if (aml->HasModOfVersion("net.rusjj.gtasa.utils", "1.4.0"))
        {
            sautils->AddOnRWInitListener([]() {
                Log::file << "RwInit" << std::endl;

                std::string imageFile = std::string(aml->GetConfigPath()) + "/giroflex/textures/1.png";

                Log::file << "Loading texture: " << imageFile << std::endl;

                //its returning 0x0 for some reason
                auto texture = sautils->LoadRwTextureFromPNG(imageFile.c_str());

                Log::file << "Texture: " << texture << std::endl;
            });
        }
        else {
            Log::file << "SAUtils 1.4 or superior not found" << std::endl;
        }
    }

    //BASS
    //https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams
    if (!(BASS = (IBASS*)GetInterface("BASS")))
    {
        Log::file << "BASS was not loaded" << std::endl;
    } else {
        Log::file << "BASS loaded: " << BASS << std::endl;

        AudioStream::_BASS = BASS; //what am I doing

      
        /*
        std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

        auto audioStream = new AudioStream(audiosPath + "/loli_dancando.mp3");
        audioStream->Loop(false);
        audioStream->Play();

        auto audioStream2 = new AudioStream(audiosPath + "/siren1.wav");
        audioStream2->Loop(true);
        audioStream2->Play();
        */
    }


    //test
    //WindowSelectPanel::Create();
}