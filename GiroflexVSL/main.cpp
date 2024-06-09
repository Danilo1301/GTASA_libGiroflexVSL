#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include "Log.h"
#include "GiroflexVSL.h"
#include "ModelInfos.h"
#include "Patterns.h"
#include "ModConfig.h"
#include "SoundSystem.h"

#include "windows/WindowSettings.h"

#include "opcodes.h"

// ---------------------------------------

MYMODCFG(net.danilo1301.giroflexVSL, GiroflexVSL, GiroflexVSL::m_Version, Danilo1301)

// ---------------------------------------

// CLEO 2.0.1.3
#include "cleo.h"
cleo_ifs_t* cleo = NULL;

// BASS
IBASS* BASS = NULL;

// SAUtils 1.6 (but it says 1.5.2)
#include "isautils.h"
ISAUtils* sautils = NULL;

// ---------------------------------------

CVector2D *m_vecCachedPos;
uintptr_t* pVehiclePool = 0;

int (*ScreenGetWidth)();
int (*ScreenGetHeight)();
int (*GetVehicleRef)(int);
void* (*GetVehicleFromRef)(int);
void (*RegisterCorona)(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, CVector const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay);

CCamera* camera;
bool* userPaused;
bool* codePaused;
// ---------------------------------------

ConfigEntry* cfgMenuOffsetX = NULL;
ConfigEntry* cfgTimeBetweenPatterns = NULL;

void SaveCfg()
{
    cfgMenuOffsetX->SetInt((int)Menu::m_MenuOffset.x);
    cfgTimeBetweenPatterns->SetInt(Patterns::m_TimeBetweenPatterns);

    cfg->Save();
}

//

DECL_HOOK(void*, UpdateGameLogic, uintptr_t a1)
{
    if (BASS) {
        SoundSystem::Update();
    }

    return UpdateGameLogic(a1);
}

//

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

        SaveCfg();
    }
}

//---------------------------------------------------------------------------------------------------

extern "C" void OnModPreLoad()
{
    ModConfig::MakePaths();

    char logPath[512];
	sprintf(logPath, "%s/giroflexVSL/giroflexVSL.log", aml->GetConfigPath());
    Log::Open(logPath);
    Log::file << "Preload()" << std::endl;
    Log::file << "AML headers: 1.1" << std::endl;

    logger->SetTag("GiroflexVSL");

    logger->Info("Preloading");
}

extern "C" void OnModLoad()
{
    Log::file << "Load()" << std::endl;

    cfgMenuOffsetX = cfg->Bind("menu_offset_x", -195, "General");
    cfgTimeBetweenPatterns = cfg->Bind("time_between_patterns", Patterns::m_TimeBetweenPatterns, "General");

    Menu::m_MenuOffset.x = (float)cfgMenuOffsetX->GetInt();
    Patterns::m_TimeBetweenPatterns = cfgTimeBetweenPatterns->GetInt();

    cfg->Bind("Author", "", "About")->SetString("Danilo1301"); cfg->ClearLast();
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/mkCDRf4zJA"); cfg->ClearLast();
    cfg->Bind("GitHub", "", "About")->SetString("https://github.com/Danilo1301/GTASA_libGiroflexVSL"); cfg->ClearLast();
    //cfg->Save();

    SaveCfg();

    //CLEO
    Log::file << "Loading CLEO..." << std::endl;
    cleo = (cleo_ifs_t*)GetInterface("CLEO");
    if (cleo)
    {
        Log::file << "CLEO loaded" << std::endl;
    }

    //BASS
    //https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams
    Log::file << "Loading BASS..." << std::endl;
    if (!(BASS = (IBASS*)GetInterface("BASS")))
    {
        Log::file << "BASS was not loaded" << std::endl;
    }
    else {
        Log::file << "BASS loaded: " << BASS << std::endl;

        SoundSystem::Init();

        std::string audiosPath = ModConfig::GetConfigFolder() + "/audios/";

        /*
        auto audioStream = new AudioStream(audiosPath + "/funk_raca_negra.mp3");
        audioStream->Loop(true);
        audioStream->Play();
        */

        /*
        auto audioStream = SoundSystem::LoadStream(audiosPath + "/siren1.wav", false);
        audioStream->SetVolume(0.5f);
        audioStream->Loop(false);
        audioStream->Play();
        */

        /*
        auto audioStream = new AudioStream(audiosPath + "/loli_dancando.mp3");
        audioStream->Loop(false);
        audioStream->Play();
        */
    }

    //SAUtils
    Log::file << "Loading SAUtils..." << std::endl;
    if (!(sautils = (ISAUtils*)GetInterface("SAUtils")))
    {
        Log::file << "SAUtils was not loaded" << std::endl;
    }
    else {
        Log::file << "SAUtils loaded" << std::endl;

        sautils->AddClickableItem(eTypeOfSettings::SetType_Mods, "Giroflex VSL - Edit mode", 0, 0, sizeofA(optionsGiroflexEditMode) - 1, optionsGiroflexEditMode, OnGiroflexEditModeChanged);

        if (aml->HasModOfVersion("net.rusjj.gtasa.utils", "1.4.0"))
        {
            sautils->AddOnRWInitListener([]() {
                Log::file << "RWInit" << std::endl;

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

    //

    std::string cleoVersion = CheckModVersion(
        { "net.rusjj.cleolib", "net.rusjj.cleomod" },
        { "2.0.1", "2.0.1.1", "2.0.1.2", "2.0.1.3", "2.0.1.4", "2.0.1.5" }
    );

    std::string sautilsVersion = CheckModVersion(
        { "net.rusjj.gtasa.utils" },
        { "1.1", "1.2", "1.2.1", "1.3.0", "1.3.1", "1.4", "1.4.1", "1.5.1", "1.6"}
    );
    
    std::string amlVersion = CheckModVersion(
        { "net.rusjj.aml" },
        { "1.0.0.0", "1.0.0.1", "1.0.0.2", "1.0.0.3", "1.0.0.4", "1.0.0.5", "1.0.0.6", "1.0.1", "1.0.2", "1.0.2.1", "1.0.2.2", "1.0.3", "1.0.3.1", "1.1", "1.2", "1.2.1", "1.2.2"}
    );

    Log::file << "----------------------------" << std::endl;
    Log::file << "Game: " << aml->GetCurrentGame() << std::endl;
    Log::file << "GiroflexVSL: " << GiroflexVSL::m_Version << std::endl;
    Log::file << "CLEO version: " << cleoVersion << " (recommended 2.0.1.3)" << std::endl;
    Log::file << "SAUtils version: " << sautilsVersion << " (recommended 1.3.1)" << std::endl;
    Log::file << "AML version: " << amlVersion << " (recommended 1.2.2)" << std::endl;
    Log::file << "----------------------------" << std::endl;
    Log::file << "Config: " << aml->GetConfigPath() << std::endl;

    //
    
    //void* hGTASA = aml->GetLibHandle("libGTASA.so"); crashes the game
    void* hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    uintptr_t gameAddr = (uintptr_t)(cleo->GetMainLibraryLoadAddress());
   
    Log::file << "hGTASA: " << hGTASA << std::endl;

    Log::file << "Getting Syms..." << std::endl;

    SET_TO(m_vecCachedPos, aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE"));
    SET_TO(pVehiclePool, aml->GetSym(hGTASA, "_ZN6CPools15ms_pVehiclePoolE"));

    SET_TO(ScreenGetWidth, aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv"));
    SET_TO(ScreenGetHeight, aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv"));
    SET_TO(GetVehicleRef, aml->GetSym(hGTASA, "_ZN6CPools13GetVehicleRefEP8CVehicle"));
    SET_TO(GetVehicleFromRef, aml->GetSym(hGTASA, "_ZN6CPools10GetVehicleEi"));
    SET_TO(RegisterCorona, aml->GetSym(hGTASA, "_ZN8CCoronas14RegisterCoronaEjP7CEntityhhhhRK7CVectorffhhhhhfbfbfbb"));

    SET_TO(camera, cleo->GetMainLibrarySymbol("TheCamera"));
    SET_TO(userPaused, cleo->GetMainLibrarySymbol("_ZN6CTimer11m_UserPauseE"));
    SET_TO(codePaused, cleo->GetMainLibrarySymbol("_ZN6CTimer11m_CodePauseE"));
    
    HOOKPLT(UpdateGameLogic, gameAddr + 0x66FE58);
    //
    
    Log::file << "vecCachedPos: x " << m_vecCachedPos->x << ", y " << m_vecCachedPos->y << std::endl;
    Log::file << "pVehiclePool: " << pVehiclePool << std::endl;

    Log::file << "ScreenGetWidth: " << ScreenGetWidth() << std::endl;
    Log::file << "ScreenGetHeight: " << ScreenGetHeight() << std::endl;
    Log::file << "RegisterCorona: " << (void*)RegisterCorona << std::endl;

    //
    
    Log::file << "Registering opcodes..." << std::endl;

    __reg_op_func(PROCESS_GIROFLEX_VSL_LIB, PROCESS_GIROFLEX_VSL_LIB);
    __reg_op_func(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
    __reg_op_func(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
    __reg_op_func(SEND_PLAYER_POSITION, SEND_PLAYER_POSITION);
    __reg_op_func(SEND_CURRENT_VEHICLE, SEND_CURRENT_VEHICLE);
    __reg_op_func(GET_MOD_INFO, GET_MOD_INFO);
    __reg_op_func(SEND_CAR_VELOCITY, SEND_CAR_VELOCITY);
    __reg_op_func(CREATE_NEW_VEHICLE, CREATE_NEW_VEHICLE);
    __reg_op_func(REGISTER_GIROFLEX_CORONA, REGISTER_GIROFLEX_CORONA);
    __reg_op_func(SEND_CAR_POSITION, SEND_CAR_POSITION);

    //

    ModConfig::ProcessVersionChanges_PreConfigLoad();

    ModConfig::Load();

    if (Patterns::m_Patterns.size() == 0)
    {
        Patterns::CreateDefaultPatterns();
    }

    ModConfig::ProcessVersionChanges_PostConfigLoad();

    /*
    if(!ModelInfos::HasModelInfo(522))
    {
        auto modelInfo = ModelInfos::CreateModelInfo(522);

        auto lightGroup = new LightGroup();
        lightGroup->offset = CVector(0, 0, 2);
        lightGroup->AddPoint();
        lightGroup->AddPoint();
        lightGroup->AddPoint();
        lightGroup->AddPoint();

        modelInfo->AddLightGroup(lightGroup);
    }
    */

    ModConfig::Save();

    //

    
}

