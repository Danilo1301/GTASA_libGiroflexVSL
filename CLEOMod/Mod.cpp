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
#include "INIConfig.h"


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

char Mod::Version[256] = "2.0.0";
int Mod::m_DeltaTime = 0;
int Mod::m_FixLightsScale = 40;

bool canTurnSirenOn = true;

unsigned char ucharIntensity(unsigned char uc, float intensity) {
    return (unsigned char)std::clamp((int)round(((float)uc) * intensity), 0, 255);
}

//---------------------------------------------------------------------------------------------------

__decl_op(SEND_CAR_POSITION, 0x0EF0); // 0EF0=5,send_car_position %1d% model_id %2d% pos %3f% %4f% %5f%
__decl_op(SEND_CURRENT_VEHICLE, 0x0EF1); //0EF1=1,send_current_vehicle %1d%
__decl_op(PROCESS_GIROFLEX_LIB, 0x0EF2); // 0EF2=1,process_giroflex_lib deltaMs %1d%
__decl_op(SEND_TOUCH_STATE, 0x0EF3); //0EF3=2,send_touch_state %1d% state %2d%
//__decl_op(TOGGLE_GIROFLEX, 0x0EF4); // 0EF4=1,toggle_giroflex car %1d%
__decl_op(REGISTER_GIROFLEX_CORONA, 0x0EF5); // 0EF5=4,register_giroflex_corona %1d% at_pos %2f% %3f% %4f%
__decl_op(RUN_TEST, 0x0EF6); // 0EF6=1,run_test %1d%
__decl_op(SEND_CAR_VELOCITY, 0x0EF7); // 0EF7=4,send_car_velocity %1d% vel %2f% %3f% %4f%
__decl_op(GET_DRAW_ITEM_INFO, 0x0EF8); // 0EF8=3,%3g% = get_draw_item_info %1d% id %2d%
//__decl_op(TOGGLE_GIROFLEX_MENU, 0x0EF9); // 0EF9=1,toggle_giroflex_menu car %1d%


void REGISTER_GIROFLEX_CORONA(__handler_params)
{
    int id = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "REGISTER_GIROFLEX_CORONA id=%d at_pos %.2f %.2f %.2f", id, x, y, z);
    Log::file << szTemp << std::endl;

    if (id > Vehicles::m_CoronasToRender.size() - 1)
    {
        /*
        
        issue here too

        */

        Log::file << "REGISTER_GIROFLEX_CORONA id exceeds" << std::endl;
        return;
    }

    auto renderCorona = &Vehicles::m_CoronasToRender[id];

    CVector position = CVector(x, y, z);

    if (renderCorona->car > 0 && Vehicles::HasVehicleHandle(renderCorona->car))
    {
        auto vel = Vehicles::GetVehicleByHandle(renderCorona->car)->velocity;

        auto probFps = 1000 / Mod::m_DeltaTime;
        float fixScale = probFps + 10.0f; //60fps = 70.0, 30fps = 40.0
        

        //float fixScale = 1.05 * (Mod::m_DeltaTime) + 15;

        //Log::opcodes << "fix scale " << fixScale << ", dt " << Mod::m_DeltaTime << " : " << vel.x << ", " << vel.y << ", " << vel.z << std::endl;

        position.x += vel.x / fixScale;
        position.y += vel.y / fixScale;
        position.z += vel.z / fixScale;
    }

    Mod::RegisterCorona(renderCorona->id, 0, renderCorona->color.r, renderCorona->color.g, renderCorona->color.b, renderCorona->color.a, { position.x, position.y, position.z }, renderCorona->radius, 1000.0f, 0, 0, true, false, 0, 0.0f, false, renderCorona->nearClip, 0, 200.0f, false, false);

}

void SEND_CURRENT_VEHICLE(__handler_params)
{
    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_CURRENT_VEHICLE car=%d", car);
    //Log::opcodes << szTemp << std::endl;

    Vehicles::hPlayerVehicle = car;
}

void SEND_TOUCH_STATE(__handler_params)
{
    int touchId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_TOUCH_STATE touchId=%d state=%d", touchId, state);
    Log::file << szTemp << std::endl;

    Input::SetTouchState(touchId, state == 1);
}

/*

void TOGGLE_GIROFLEX_MENU(__handler_params)
{
    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "TOGGLE_GIROFLEX_MENU car=%d", car);
    //Log::opcodes << szTemp << std::endl;

    //WindowMain::Create();
}
*/

/*

void TOGGLE_GIROFLEX(__handler_params)
{
    int car = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "TOGGLE_GIROFLEX car=%d", car);
    //Log::opcodes << szTemp << std::endl;
}
*/

void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    Log::file << szTemp << std::endl;

    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {

        Log::file << "AMOUNT_OF_DRAWITEMS = " << Draw::m_DrawItems.size() << std::endl;
        result->i = Draw::m_DrawItems.size();
        return;
    }
    if (type == eDrawInfoType::AMOUNT_OF_CORONAS)
    {
        Log::file << "AMOUNT_OF_CORONAS = " << Vehicles::m_CoronasToRender.size() << std::endl;
        result->i = Vehicles::m_CoronasToRender.size();
        return;
    }

    //

    bool coronaIdExceeds = (id > Vehicles::m_CoronasToRender.size() - 1);

    if (coronaIdExceeds)
    {
        Log::file << "coronaIdExceeds exceeds" << std::endl;
    }
    if (type == eDrawInfoType::CORONA_CAR)
    {
        if (coronaIdExceeds) return;

        Log::file << "returns " << Vehicles::m_CoronasToRender[id].car << std::endl;

        result->i = Vehicles::m_CoronasToRender[id].car;
        return;
    }
    if (type == eDrawInfoType::CORONA_OFFSET_X)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.x;
        return;
    }
    if (type == eDrawInfoType::CORONA_OFFSET_Y)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.y;
        return;
    }
    if (type == eDrawInfoType::CORONA_OFFSET_Z)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.z;
        return;
    }
    if (type == eDrawInfoType::CORONA_USE_POINT_LIGHT)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].renderPointLight ? 1 : 0;
        return;
    }
    if (type == eDrawInfoType::CORONA_USE_SHADOW)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].renderShadow ? 1: 0;
        return;
    }
    if (type == eDrawInfoType::CORONA_R)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.r;
        return;
    }
    if (type == eDrawInfoType::CORONA_G)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.g;
        return;
    }
    if (type == eDrawInfoType::CORONA_B)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.b;
        return;
    }
    if (type == eDrawInfoType::CORONA_A)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.a;
        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_INTENSITY)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].shadowIntensity;
        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_SIZE)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].shadowSize;
        return;
    }
    if (type == eDrawInfoType::CORONA_POINTLIGHT_DISTANCE)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].pointLightDistance;
        return;
    }
    if (type == eDrawInfoType::SHADOW_R)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.r,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }
    if (type == eDrawInfoType::SHADOW_G)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.g,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }
    if (type == eDrawInfoType::SHADOW_B)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.b,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }
    if (type == eDrawInfoType::POINT_LIGHT_R)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.r,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }
    if (type == eDrawInfoType::POINT_LIGHT_G)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.g,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }
    if (type == eDrawInfoType::POINT_LIGHT_B)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.b,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_OFFSET_X)
    {
        if (coronaIdExceeds) return;

        auto offset = Vehicles::m_CoronasToRender[id].offset;

        float margin = 0.15f;
        float distance = abs(offset.x);

        if (distance < margin)
        {
            result->f = offset.x;
        }
        else {
            auto dir = (offset.x > 0) ? 1 : -1;
            result->f = offset.x + dir * Vehicles::m_CoronasToRender[id].shadowSize/2;
        }

        
        return;
    }

    //

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
        /*
        
        theres an issue here

        */

        Log::file << "draw item id " << id << " out of range" << std::endl;
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
    if (type == eDrawInfoType::ALIGN) result->i = (int)item->textAlign;
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
    Log::file << szTemp << std::endl;

    Vehicles::TryCreateVehicle(car, modelId);

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::m_Vehicles[car];
    vehicle->position = CVector(x, y, z);



    /*
    Vehicles::TryCreateVehicle(car, modelId);

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::m_Vehicles[car];
    vehicle->position = CVector(x, y, z);

    auto lightId = uniqueLightId + vehicle->hVehicle + 30000;

    posStruct pos = {
        vehicle->position.x,
        vehicle->position.y,
        vehicle->position.z
    };

    CRGBA color = CRGBA(255, 120, 0);

    Mod::RegisterCorona(lightId++, 0, color.r, color.g, color.b, color.a, { pos.x, pos.y, pos.z }, 2.0f, 300.0f, WindowMain::type, 0, false, false, 0, 0.0f, false, 0.1f, 0, 20.0f, false, false);

    pos.x += 1.0f;

    Mod::RegisterCorona(lightId++, 0, color.r, color.g, color.b, color.a, { pos.x, pos.y, pos.z }, 1.0f, 300.0f, WindowMain::type, 0, false, false, 0, 0.0f, false, 0.1f, 0, 20.0f, false, false);

    */
}


void SEND_CAR_VELOCITY(__handler_params)
{
    int car = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_VELOCITY car=%d, x=%.2f, y=%.2f, z=%.2f", car, x, y, z);
    Log::file << szTemp << std::endl;

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::GetVehicleByHandle(car);

    vehicle->velocity = CVector(x, y, z);

    //Log::opcodes << "velocity set" << std::endl;
}

void ProcessTouch()
{
    Log::file << "ProcessTouch" << std::endl;

    if (Input::GetTouchIdState(6) && Input::GetTouchIdState(5))
    {
        if (Input::GetTouchIdPressTime(6) > 500)
        {
            if (!Vehicles::IsPlayerInAnyVehicle())
            {
                Menu::ShowPopup(16, 0, 0, 1000);
                return;
            }

            auto vehicle = Vehicles::GetPlayerVehicle();

            if (canTurnSirenOn)
            {
                canTurnSirenOn = false;

                Log::file << "Toggling siren by 0.5s touch" << std::endl;

                vehicle->SetGiroflexEnabled(!vehicle->lightsOn);
            }

            if (Input::GetTouchIdPressTime(6) > 1000)
            {
                Log::file << "more than 1.0s" << std::endl;

                if (!WindowMain::m_Window)
                {
                    Log::file << "window not found, enabling giroflex" << std::endl;

                    vehicle->SetGiroflexEnabled(true);

                    Log::file << "creating set" << std::endl;

                    WindowMain::Create(vehicle->modelId);

                }
            }
        }
    }
    else {
        canTurnSirenOn = true;
    }

    Log::file << "ProcessTouch end" << std::endl;
}

void PROCESS_GIROFLEX_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;

    Mod::m_DeltaTime = dt;

    //Log::opcodes << "PROCESS_GIROFLEX_LIB dt=" << dt << std::endl;
    Log::file << "PROCESS_GIROFLEX_LIB dt=" << dt << std::endl;


    while (Draw::m_DrawItems.size() > 0) {
        auto dw = Draw::m_DrawItems[0];
        Draw::m_DrawItems.erase(std::find(Draw::m_DrawItems.begin(), Draw::m_DrawItems.end(), dw));
        delete dw;
    }
    //Draw::m_DrawItems.clear();

    Log::file << "Vehicles::Update" << std::endl;

    Vehicles::Update(dt);

    Log::file << "Menu::Update" << std::endl;

    Menu::Update(dt);

    Log::file << "Menu::Draw" << std::endl;

    Menu::Draw();

    ProcessTouch();
    
    Log::file << "Input::Update" << std::endl;

    Input::Update(dt);

    Log::file << "Input::Update end" << std::endl;
}

void RUN_TEST(__handler_params)
{
    int testType = __readParam(handle)->i;

    //Log::opcodes << "RUN_TEST testType=" << testType << std::endl;

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

void Mod::OnModPreLoad()
{
    std::string configPath = aml->GetConfigPath();


    bool insideCleo = true;
    if (configPath.find("rockstargames") != std::string::npos) insideCleo = false;

    if (insideCleo)
    {
        Log::file.open("/storage/emulated/0/cleo/giroflex.log", std::fstream::out | std::fstream::trunc);
        Log::opcodes.open("/storage/emulated/0/cleo/giroflex_opcodes.log", std::fstream::out | std::fstream::trunc);
    } else {
        Log::file.open(configPath + "/giroflex/giroflex.log", std::fstream::out | std::fstream::trunc);
        Log::opcodes.open(configPath + "/giroflex/giroflex_opcodes.log", std::fstream::out | std::fstream::trunc);
    }

    Log::file << "Preload..." << std::endl;
    Log::opcodes << "Preload..." << std::endl;

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
    Log::file << "Load..." << std::endl;

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

    //cfg->Bind("pos.x", 45, "523");
    //cfg->Save();



    Log::file << "------------------------" << std::endl;
    Log::file << "Giroflex version: " << Version << std::endl;
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

    INIConfig::Load();

    if (Patterns::m_Patterns.size() == 0)
    {
        auto pattern1 = Patterns::CreatePattern();
        pattern1->AddStep({ 0 }, 300);
        pattern1->AddStep({ 1 }, 300);
        /*
        pattern.push({values: [0], time: 300});
        pattern.push({values: [1], time: 300});
        */

        auto pattern2 = Patterns::CreatePattern();
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

        auto pattern3 = Patterns::CreatePattern();
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

        auto pattern4 = Patterns::CreatePattern();
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

        auto pattern5 = Patterns::CreatePattern();
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

        auto pattern6 = Patterns::CreatePattern();
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

        auto pattern7 = Patterns::CreatePattern();
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


        auto pattern8 = Patterns::CreatePattern();
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





    }

    //WindowMain::Create(523);

    //SAUtils
    Log::file << "Loading SAUtils..." << std::endl;
    sautils = (ISAUtils*)GetInterface("SAUtils");
    if (sautils)
    {
        sautils->AddClickableItem(SetType_Game, "Vehicle Siren Lights", 0, 0, sizeofA(pLocations) - 1, pLocations, OnLocationChanged);
        Log::file << "SAUtils Loaded" << std::endl;
    }
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
*/