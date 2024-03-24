#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include "Log.h"
#include "GiroflexVSL.h"

#include "opcodes.h"

#include "dlfcn.h"

// ---------------------------------------

MYMODCFG(net.danilo1301.giroflexVSL, GiroflexVSL, GiroflexVSL::m_Version, Danilo1301)

// ---------------------------------------

// CLEO 2.0.1.3
#include "cleo.h"
cleo_ifs_t* cleo = NULL;

// ---------------------------------------

CVector2D *m_vecCachedPos;

int (*ScreenGetWidth)();
int (*ScreenGetHeight)();

// ---------------------------------------

ConfigEntry* cfgMenuOffsetX = NULL;

//

extern "C" void OnModPreLoad()
{
    logger->SetTag("GiroflexVSL");

    logger->Info("Preloading");

    char logPath[512];
	sprintf(logPath, "%s/giroflexVSL/giroflexVSL.log", aml->GetConfigPath());
    Log::Open(logPath);
    Log::file << "Preload()" << std::endl;
}

extern "C" void OnModLoad()
{
    Log::file << "Load()" << std::endl;

    cfgMenuOffsetX = cfg->Bind("menu_offset_x", -195, "General");
    //cfgTimeBetweenPatterns = cfg->Bind("time_between_patterns", Patterns::m_TimeBetweenPatterns, "General");
    //cfgCoronaFpsFix = cfg->Bind("corona_fps_fix", Mod::CoronaFixFPS, "General");

    Menu::m_MenuOffset.x = (float)cfgMenuOffsetX->GetInt();
    //Patterns::m_TimeBetweenPatterns = cfgTimeBetweenPatterns->GetInt();
    //Mod::CoronaFixFPS = (eCoronaFixFPS)cfgCoronaFpsFix->GetInt();

    cfg->Bind("Author", "", "About")->SetString("Danilo1301"); cfg->ClearLast();
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/mkCDRf4zJA"); cfg->ClearLast();
    cfg->Bind("GitHub", "", "About")->SetString("https://github.com/Danilo1301/GTASA_libGiroflexVSL"); cfg->ClearLast();
    cfg->Save();

    //CLEO
    Log::file << "Loading CLEO..." << std::endl;
    cleo = (cleo_ifs_t*)GetInterface("CLEO");
    if (cleo)
    {
        Log::file << "CLEO loaded" << std::endl;
    }

    Log::file << "----------------------------" << std::endl;
    Log::file << "GiroflexVSL: v" << GiroflexVSL::m_Version << std::endl;
    Log::file << "----------------------------" << std::endl;

    //
    
    //void* hGTASA = aml->GetLibHandle("libGTASA.so"); crashes the game
    void* hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    Log::file << "hGTASA: " << hGTASA << std::endl;

    Log::file << "Getting Syms..." << std::endl;

    SET_TO(m_vecCachedPos, aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE"));

    SET_TO(ScreenGetWidth, aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv"));
    SET_TO(ScreenGetHeight, aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv"));

    //
    
    Log::file << "vecCachedPos: x " << m_vecCachedPos->x << ", y " << m_vecCachedPos->y << std::endl;
    Log::file << "ScreenGetWidth: " << ScreenGetWidth() << std::endl;
    Log::file << "ScreenGetHeight: " << ScreenGetHeight() << std::endl;

    //
    
    Log::file << "Registering opcodes..." << std::endl;

    __reg_op_func(PROCESS_GIROFLEX_VSL_LIB, PROCESS_GIROFLEX_VSL_LIB);
    __reg_op_func(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
    __reg_op_func(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
}
