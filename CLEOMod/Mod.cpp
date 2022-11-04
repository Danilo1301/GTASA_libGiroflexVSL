#include "Mod.h"

#include "Log.h"
#include "Vehicles.h"
#include "Draw.h"
#include "Input.h"
#include "Menu.h"
#include "Patterns.h"
#include "ModelInfos.h"
#include "LightGroupDatas.h"

#include "WindowMain.h"

#include <iostream>
#include <string>
#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST

// CLEO 2.0.1.2
#include "icleo.h"
ICLEO* cleo2012 = NULL;

// CLEO 2.0.1.3
#include "cleo.h"
cleo_ifs_t* cleo2013 = NULL;

// SAUtils
#include "isautils.h"
ISAUtils* sautils = NULL;

#define __decl_op(__name, __int)	const char* NAME_##__name = #__name; const uint16_t OP_##__name = __int;
//#define __print_to_log(__str)		cleo->PrintToCleoLog(__str); logger->Info(__str)

#define __reg_opcode2012				cleo2012->RegisterOpcode
#define __reg_func2012					cleo2012->RegisterOpcodeFunction

//#define __reg_opcode				cleo->RegisterOpcode
//#define __reg_func					cleo->RegisterOpcodeFunction

#define __reg_opcode2013				cleo2013->RegisterOpcode
#define __reg_func2013					cleo2013->RegisterOpcodeFunction

#define __handler_params 			void *handle, uint32_t *ip, uint16_t opcode, const char *name
#define __op_name_match(x) 			opcode == OP_##x || strcmp(name, NAME_##x) == 0
#define __reg_op_func2012(x, h) 		__reg_opcode2012(OP_##x, h); __reg_func2012(NAME_##x, h);
#define __reg_op_func2013(x, h) 		__reg_opcode2013(OP_##x, h); __reg_func2013(NAME_##x, h);


#define __readParam(handle)         (cleo2012 ? reinterpret_cast<cleo_ifs_t::data_t*>(cleo2012->ReadParam(handle)) : cleo2013->ReadParam(handle))
#define __getPointerToScriptVar(handle)         (cleo2012 ? reinterpret_cast<cleo_ifs_t::data_t*>(cleo2012->GetPointerToScriptVar(handle)) : cleo2013->GetPointerToScriptVar(handle))

// Size of array
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))


void* hGTASA = NULL;
uintptr_t pGTASA = 0;
uintptr_t pRegisterCorona = 0;

unsigned int uniqueLightId = 65487;

char Mod::Version[256] = "1.0.0";

//---------------------------------------------------------------------------------------------------

__decl_op(SEND_CAR_POSITION, 0x0EF0); // 0EF0=5,send_car_position %1d% model_id %2d% pos %3f% %4f% %5f%
__decl_op(SEND_CURRENT_VEHICLE, 0x0EF1); //0EF1=1,send_current_vehicle %1d%
__decl_op(PROCESS_GIROFLEX_LIB, 0x0EF2); // 0EF2=1,process_giroflex_lib deltaMs %1d%
__decl_op(SEND_TOUCH_STATE, 0x0EF3); //0EF3=2,send_touch_state %1d% state %2d%
__decl_op(RUN_TEST, 0x0EF6); // 0EF6=1,run_test %1d%
__decl_op(GET_DRAW_ITEM_INFO, 0x0EF8); // 0EF8=3,%3g% = get_draw_item_info %1d% id %2d%
__decl_op(TOGGLE_GIROFLEX_MENU, 0x0EF9); // 0EF9=1,toggle_giroflex_menu car %1d%
__decl_op(TOGGLE_GIROFLEX, 0x0EF4); // 0EF4=1,toggle_giroflex car %1d%

void SEND_CURRENT_VEHICLE(__handler_params)
{


    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_CURRENT_VEHICLE car=%d", car);
    Log::opcodes << szTemp << std::endl;
}

void SEND_TOUCH_STATE(__handler_params)
{
    int touchId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_TOUCH_STATE touchId=%d state=%d", touchId, state);
    Log::opcodes << szTemp << std::endl;

    Input::SetTouchState(touchId, state == 1);
}

void TOGGLE_GIROFLEX_MENU(__handler_params)
{
    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "TOGGLE_GIROFLEX_MENU car=%d", car);
    Log::opcodes << szTemp << std::endl;

    //WindowMain::Create();
}

void TOGGLE_GIROFLEX(__handler_params)
{
    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "TOGGLE_GIROFLEX car=%d", car);
    Log::opcodes << szTemp << std::endl;
}

void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    Log::opcodes << szTemp << std::endl;

    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {
        result->i = Draw::m_DrawItems.size();
        return;
    }

    /*
    if (type == eDrawInfoType::UPDATE_TOUCH_STATE)
    {
        Input::ChangePressedState(id == 1);
        result->i = id;
        return;
    }
    */

    if (type == eDrawInfoType::TOUCH_X)
    {
        result->i = Input::GetTouchPos().x;
        return;
    }

    if (type == eDrawInfoType::TOUCH_Y) {
        result->i = Input::GetTouchPos().y;
        return;
    }

    if (id > Draw::m_DrawItems.size() - 1)
    {
        Log::opcodes << "id " << id << " out of range" << std::endl;
        return;
    }

    auto item = Draw::m_DrawItems[id];

    if (type == eDrawInfoType::TYPE) result->i = (int)item->type;
    if (type == eDrawInfoType::POS_X) result->f = item->pos.x + item->size.x / 2.0f; //add, so it draw centered
    if (type == eDrawInfoType::POS_Y) result->f = item->pos.y + item->size.y / 2.0f; //add, so it draw centered
    if (type == eDrawInfoType::SIZE_X) result->f = item->size.x;
    if (type == eDrawInfoType::SIZE_Y) result->f = item->size.y;
    if (type == eDrawInfoType::R) result->i = item->color.r;
    if (type == eDrawInfoType::G) result->i = item->color.g;
    if (type == eDrawInfoType::B) result->i = item->color.b;
    if (type == eDrawInfoType::A) result->i = item->color.a;
    if (type == eDrawInfoType::GXT_ID) result->i = item->gxtId;
    if (type == eDrawInfoType::NUM_1) result->i = item->num1;
    if (type == eDrawInfoType::NUM_2) result->i = item->num2;
}

void SEND_CAR_POSITION(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;


    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_POSITION car=%d, modelId=%d, x=%.2f, y=%.2f, z=%.2f", car, modelId, x, y, z);
    Log::opcodes << szTemp << std::endl;

    Vehicles::TryCreateVehicle(car, modelId);

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::m_Vehicles[car];
    vehicle->position = CVector(x, y, z);

    //

    if (!ModelInfos::HasModelInfo(vehicle->modelId)) return;

    auto modelInfo = ModelInfos::GetModelInfo(vehicle->modelId);

    auto lightId = uniqueLightId + vehicle->hVehicle + 30000;

    for (auto lightGroup : modelInfo->lightGroups)
    {
        if (!LightGroupDatas::HasLightGroupData(lightGroup)) return;

        LightGroupData* lightGroupData = LightGroupDatas::m_LightGroupDatas[lightGroup];

        for (int i = 0; i < (int)lightGroup->points.size(); i++)
        {
            auto point = lightGroup->points[i];

            posStruct pos = {
                x + point->offset.x,
                y + point->offset.y,
                z + point->offset.z
            };

            int index = i;
            //if (lightGroup->usePointPositionInsteadOfIndex) index = (int)point->pointPosition;

            bool enabled = lightGroupData->GetPointIsEnabled(point, index);

            float radius = enabled ? 2.0f : 0.0f;
           
            

            Mod::RegisterCorona(lightId++, 0, point->color.r, point->color.g, point->color.b, point->color.a, { pos.x, pos.y, pos.z }, radius, 300.0f, WindowMain::type, 0, false, false, 0, 0.0f, false, 0.1f, 0, 20.0f, false, false);

        }
    }



    /*
    
    this works

    auto lightId = uniqueLightId + vehicle->hVehicle + 30000;

    posStruct pos = { x, y, z };

    pos.z += 1.0f;


    for (int i = 0; i < WindowMain::numCoronas; i++)
    {
        Mod::RegisterCorona(lightId + i, 0, 0, 0, 255, 255, { pos.x + (i*1.0f), pos.y, pos.z }, 2.0f, 300.0f, WindowMain::type, 0, false, false, 0, 0.0f, false, 0.1f, 0, 5.0f, false, false);
    }
    */

    
}

void PROCESS_GIROFLEX_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;

    Log::opcodes << "PROCESS_GIROFLEX_LIB dt=" << dt << std::endl;

    while (Draw::m_DrawItems.size() > 0) {
        auto dw = Draw::m_DrawItems[0];
        Draw::m_DrawItems.erase(std::find(Draw::m_DrawItems.begin(), Draw::m_DrawItems.end(), dw));
        delete dw;
    }
    //Draw::m_DrawItems.clear();

    Log::opcodes << "CheckStreamedOutVehicles" << std::endl;
    //Vehicles::CheckStreamedOutVehicles();

    Log::opcodes << "Menu Update" << std::endl;

    Vehicles::UpdateVehiclesPattern(dt);

    Menu::Update(dt);

    Menu::Draw();

   
    if (Input::GetTouchIdState(6) && Input::GetTouchIdState(5) && Input::GetTouchIdPressTime(6) > 500)
    {
        WindowMain::Create();
    }
    
    Input::Update(dt);
}

void RUN_TEST(__handler_params)
{
    int testType = __readParam(handle)->i;

    Log::opcodes << "RUN_TEST testType=" << testType << std::endl;

    if (testType == 0)
    {
        /*
        void (*func)(float, float, char*) = (void (*)(float, float, char*)) FindPointer("_ZN5CFont11PrintStringEffPt");

        char szTemp[256];
        sprintf(szTemp, "helooooooooooooooo");
        func(200.0f, 200.0f, szTemp);
        */
    }

    if (testType == 1)
    {
        for (auto pair : Vehicles::m_Vehicles)
        {
            auto vehicle = pair.second;
            //auto lightId = uniqueLightId + vehicle->hVehicle + 30000;


            posStruct pos = { vehicle->position.x, vehicle->position.y, vehicle->position.z };
            pos.z += 1.0f;


            //RegisterCorona(612352, NULL, 0, 255, 0, 255, pos, 2.0f, 300.0f, 6, 1, false, false, 0, 0.0f, false, 0.1f, 0, 2.0f, false, false);
            //Mod::RegisterCorona(lightId + 2, NULL, 0, 255, 0, 255, pos, 2.0f, 300.0f, 6, 1, false, false, 0, 0.0f, false, 0.1f, 0, 2.0f, false, false);
            //Mod::RegisterCorona(lightId + 3, NULL, 0, 255, 0, 255, pos, 2.0f, 300.0f, 0, 1, false, false, 0, 0.0f, false, 0.1f, 0, 2.0f, false, false);
        }
    }

    if (testType == 2)
    {
        Draw::CFont_PrintString(200, 200, "abcdefghijklmnopqrstuvj");
    }
}

//---------------------------------------------------------------------------------------------------
//test
const char* pLocations[] = {
    "opt1",
    "opt2",
};
void OnLocationChanged(int oldVal, int newVal)
{
    logger->Info("OnLocationChanged");
}
//---------------------------------------------------------------------------------------------------

bool isDirExist(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

void Mod::OnModPreLoad()
{
    char logPath[0xFF];
    snprintf(logPath, sizeof(logPath), "%s/giroflex", aml->GetConfigPath());
    
    if (!isDirExist(logPath))
    {
        mkdir(logPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    snprintf(logPath, sizeof(logPath), "%s/giroflex/giroflex.log", aml->GetConfigPath());
    Log::file.open(logPath, std::fstream::out | std::fstream::trunc);

    snprintf(logPath, sizeof(logPath), "/storage/emulated/0/cleo/giroflex_opcodes.log");
    Log::opcodes.open(logPath, std::fstream::out | std::fstream::trunc);

    Log::file << "OnModPreLoad..." << std::endl;
    Log::opcodes << "OnModPreLoad..." << std::endl;

    logger->SetTag("Giroflex");
}

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

void Mod::OnModLoad()
{
    Log::file << "OnModLoad..." << std::endl;

    std::string cleoVersion = CheckModVersion(
        {"net.rusjj.cleolib", "net.rusjj.cleomod"},
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

  
    void* cleoInterface = GetInterface("CLEO");

    if (!cleoInterface)
    {
        Log::file << "CLEO interface not found!" << std::endl;
        return;
    }

    
    cfg->Bind("pos.x", 45, "523");
    cfg->Save();


    Log::file << "------------------------" << std::endl;
    Log::file << "Giroflex version: " << Version << std::endl;
    Log::file << "SAUtils version: " << sautilsVersion << "  (expected 1.3.1)" << std::endl;
    Log::file << "AML version: " << amlVersion << "  (expected 1.0.0.6)" << std::endl;
    Log::file << "CLEO version: " << cleoVersion << "  (expected 2.0.1.3)" << std::endl;

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
    

    //CLEO
    

    //opcodes
    Log::file << "Registering opcodes..." << std::endl;

    if (cleo2012)
    {
        __reg_op_func2012(SEND_CAR_POSITION, SEND_CAR_POSITION);
        __reg_op_func2012(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
        __reg_op_func2012(PROCESS_GIROFLEX_LIB, PROCESS_GIROFLEX_LIB);
        __reg_op_func2012(RUN_TEST, RUN_TEST);
        __reg_op_func2012(TOGGLE_GIROFLEX_MENU, TOGGLE_GIROFLEX_MENU);
        __reg_op_func2012(TOGGLE_GIROFLEX, TOGGLE_GIROFLEX);
        __reg_op_func2012(SEND_CURRENT_VEHICLE, SEND_CURRENT_VEHICLE);
        __reg_op_func2012(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
    }
    else {
        __reg_op_func2013(SEND_CAR_POSITION, SEND_CAR_POSITION);
        __reg_op_func2013(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
        __reg_op_func2013(PROCESS_GIROFLEX_LIB, PROCESS_GIROFLEX_LIB);
        __reg_op_func2013(RUN_TEST, RUN_TEST);
        __reg_op_func2013(TOGGLE_GIROFLEX_MENU, TOGGLE_GIROFLEX_MENU);
        __reg_op_func2013(TOGGLE_GIROFLEX, TOGGLE_GIROFLEX);
        __reg_op_func2013(SEND_CURRENT_VEHICLE, SEND_CURRENT_VEHICLE);
        __reg_op_func2013(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
    }
   

    //libGTASA
    Log::file << "Loading libGTASA..." << std::endl;
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    pGTASA = aml->GetLib("libGTASA.so");

    Log::file << "Finding pointers..." << std::endl;
    pRegisterCorona = aml->GetSym(hGTASA, "_ZN8CCoronas14RegisterCoronaEjP7CEntityhhhhRK7CVectorffhhhhhfbfbfbb");
    Draw::pPrintString = aml->GetSym(hGTASA, "_ZN5CFont11PrintStringEffPt");
    Input::pTouchPos = aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE");
    Input::pScreenGetWidth = aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv");
    Input::pScreenGetHeight = aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv");
    Input::pScreenResX = aml->GetSym(hGTASA, "ScreenResolutionX");
    Input::pScreenResY = aml->GetSym(hGTASA, "ScreenResolutionY");

    Log::file << "pRegisterCorona = " << pRegisterCorona << std::endl;
    Log::file << "pPrintString = " << Draw::pPrintString << std::endl;
    Log::file << "pTouchPos = " << Input::pTouchPos << std::endl;
    Log::file << "pScreenGetWidth = " << Input::pScreenGetWidth << std::endl;
    Log::file << "pScreenGetHeight = " << Input::pScreenGetHeight << std::endl;
    Log::file << "pScreenResX = " << Input::pScreenResX << std::endl;
    Log::file << "pScreenResY = " << Input::pScreenResY << std::endl;

    Log::file << "Loaded" << std::endl;

    auto pattern1 = Patterns::CreatePattern();
    pattern1->AddStep({ 1 }, 100);
    pattern1->AddStep({ 0 }, 100);
    pattern1->AddStep({ 1 }, 100);
    pattern1->AddStep({ 0 }, 400);

    auto pattern2 = Patterns::CreatePattern();
    pattern2->AddStep({ 1, 1, }, 100);
    pattern2->AddStep({ 0, 0 }, 100);
    pattern2->AddStep({ 1, 1 }, 100);
    pattern2->AddStep({ 0, 0 }, 400);

    auto pattern3 = Patterns::CreatePattern();
    pattern3->AddStep({ 1, 0, 0, 0, 0 }, 200);
    pattern3->AddStep({ 0, 1, 0, 0, 0 }, 80);
    pattern3->AddStep({ 0, 0, 1, 0, 0 }, 80);
    pattern3->AddStep({ 0, 0, 0, 1, 0 }, 80);
    pattern3->AddStep({ 0, 0, 0, 0, 1 }, 200);
    pattern3->AddStep({ 0, 0, 0, 1, 0 }, 80);
    pattern3->AddStep({ 0, 0, 1, 0, 0 }, 80);
    pattern3->AddStep({ 0, 1, 0, 0, 0 }, 80);

    auto modelInfo523 = ModelInfos::CreateModelInfo(523);

    auto lightGroup3 = new LightGroup();
    lightGroup3->type = eLightGroupType::FIVE_LIGHTS;
    //lightGroup3->usePointPositionInsteadOfIndex = true;
    lightGroup3->offset.z = 0.9f;
    lightGroup3->MakeLightGroup();
    modelInfo523->AddLightGroup(lightGroup3);

    WindowMain::Create();

    /*
    logger->Info("Loading...");

    //CLEO
    if (!(cleo = (ICLEO*)GetInterface("CLEO")))
    {
        logger->Error("CLEO interface not found!");
        return;
    }

    Log::OpenAtFolder("/storage/emulated/0/cleo");
    //Log::OpenAtFolder(cleo->GetCleoStorageDir() + std::string("/giroflex"));
    Log::file << "Loading..." << std::endl;
    Log::opcodes << "Loading..." << std::endl;

    

    

    //pointerse
    pRegisterCorona = aml->GetSym(hGTASA, "_ZN8CCoronas14RegisterCoronaEjP7CEntityhhhhRK7CVectorffhhhhhfbfbfbb");
    Draw::pPrintString = aml->GetSym(hGTASA, "_ZN5CFont11PrintStringEffPt");
    Input::pTouchPos = aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE");
    Input::pScreenGetWidth = aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv");
    Input::pScreenGetHeight = aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv");
    Input::pScreenResX = aml->GetSym(hGTASA, "ScreenResolutionX");
    Input::pScreenResY = aml->GetSym(hGTASA, "ScreenResolutionY");

    Log::file << "pRegisterCorona = " << pRegisterCorona << std::endl;
    Log::file << "pPrintString = " << Draw::pPrintString << std::endl;
    Log::file << "pTouchPos = " << Input::pTouchPos << std::endl;
    Log::file << "pScreenGetWidth = " << Input::pScreenGetWidth << std::endl;
    Log::file << "pScreenGetHeight = " << Input::pScreenGetHeight << std::endl;
    Log::file << "pScreenResX = " << Input::pScreenResX << std::endl;
    Log::file << "pScreenResY = " << Input::pScreenResY << std::endl;

    //SAUtils
    Log::file << "Loading SAUtils..." << std::endl;
    sautils = (ISAUtils*)GetInterface("SAUtils");
    if (sautils)
    {
        sautils->AddClickableItem(SetType_Game, "TestMod Item", 0, 0, sizeofA(pLocations) - 1, pLocations, OnLocationChanged);
        logger->Info("SAUtils loaded");
    }

    Log::file << "Loaded" << std::endl;

    auto mainWindow = Menu::m_MainWindow;
    */
}

void Mod::RegisterCorona(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, posStruct const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay)
{
    void (*func)(unsigned int, void*, unsigned char, unsigned char, unsigned char, unsigned char, posStruct const&, float, float, int, int, bool, bool, int, float, bool, float, unsigned char, float, bool, bool) = (void (*)(unsigned int, void*, unsigned char, unsigned char, unsigned char, unsigned char, posStruct const&, float, float, int, int, bool, bool, int, float, bool, float, unsigned char, float, bool, bool)) pRegisterCorona;

    func(id, attachTo, red, green, blue, alpha, posn, radius, farClip, coronaType, flaretype, enableReflection, checkObstacles, _param_not_used, angle, longDistance, nearClip, fadeState, fadeSpeed, onlyFromBelow, reflectionDelay);
}

/*
0EF1=4,send_car_speed %1d% x %2f% y %3f% z %4f%
0EF3=2,%2d% = is_giroflex_enabled for_car %1d%
0EF4=2,set_giroflex_enabled for_car %1d% enabled %2d%
0EF5=9,register_giroflex_corona id %1d% r %2d% g %3d% b %4d% a %5d% x %6f% y %7f% z %8f% flare %9d%
*/