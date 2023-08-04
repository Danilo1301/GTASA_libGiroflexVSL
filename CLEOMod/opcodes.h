#pragma once

#include "pch.h"

#include "Vehicles.h"
#include "Mod.h"
#include "Input.h"
#include "windows/WindowPanel.h"

#define __decl_op(__name, __int)	const char* NAME_##__name = #__name; const uint16_t OP_##__name = __int;

#define __handler_params 			void *handle, uint32_t *ip, uint16_t opcode, const char *name

// Size of array
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

#define __reg_opcode2013				cleo2013->RegisterOpcode
#define __reg_func2013					cleo2013->RegisterOpcodeFunction

#define __reg_opcode2012				cleo2012->RegisterOpcode
#define __reg_func2012					cleo2012->RegisterOpcodeFunction

#define __reg_func					cleo->RegisterOpcodeFunction

#define __op_name_match(x) 			opcode == OP_##x || strcmp(name, NAME_##x) == 0

#define __reg_op_func2012(x, h) 		__reg_opcode2012(OP_##x, h); __reg_func2012(NAME_##x, h);
#define __reg_op_func2013(x, h) 		__reg_opcode2013(OP_##x, h); __reg_func2013(NAME_##x, h);

#define __readParam(handle)         (cleo2012 ? reinterpret_cast<cleo_ifs_t::data_t*>(cleo2012->ReadParam(handle)) : cleo2013->ReadParam(handle))
#define __getPointerToScriptVar(handle)         (cleo2012 ? reinterpret_cast<cleo_ifs_t::data_t*>(cleo2012->GetPointerToScriptVar(handle)) : cleo2013->GetPointerToScriptVar(handle))


__decl_op(SEND_CAR_POSITION, 0x0EF0); // 0EF0=5,send_car_position %1d% model_id %2d% pos %3f% %4f% %5f%
__decl_op(SEND_CURRENT_VEHICLE, 0x0EF1); //0EF1=2,send_current_vehicle %1d% modelid %2d%
__decl_op(PROCESS_GIROFLEX_LIB, 0x0EF2); // 0EF2=1,process_giroflex_lib deltaMs %1d%
__decl_op(SEND_TOUCH_STATE, 0x0EF3); //0EF3=2,send_touch_state %1d% state %2d%
__decl_op(SEND_PLAYER_POSITION, 0x0EF4); // 0EF4=4,send_player_position %1d% pos %2f% %3f% %4f%
__decl_op(REGISTER_GIROFLEX_CORONA, 0x0EF5); // 0EF5=4,register_giroflex_corona %1d% at_pos %2f% %3f% %4f%
__decl_op(RUN_TEST, 0x0EF6); // 0EF6=1,run_test %1d%
__decl_op(SEND_CAR_VELOCITY, 0x0EF7); // 0EF7=4,send_car_velocity %1d% vel %2f% %3f% %4f%
__decl_op(GET_DRAW_ITEM_INFO, 0x0EF8); // 0EF8=3,%3g% = get_draw_item_info %1d% id %2d%

static void SEND_PLAYER_POSITION(__handler_params)
{
    int id = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    Mod::m_PlayerPos = CVector(x, y, z);
}

static void REGISTER_GIROFLEX_CORONA(__handler_params)
{
    int id = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "REGISTER_GIROFLEX_CORONA id=%d at_pos %.2f %.2f %.2f", id, x, y, z);
    //Log::file << szTemp << std::endl;

    if (id > Vehicles::m_CoronasToRender.size() - 1)
    {
        /*

        issue here too
        */

        //Log::file << "REGISTER_GIROFLEX_CORONA id exceeds" << std::endl;
        return;
    }

    auto renderCorona = &Vehicles::m_CoronasToRender[id];

    CVector position = CVector(x, y, z);

    if (renderCorona->car > 0 && Vehicles::HasVehicleHandle(renderCorona->car))
    {
        auto vel = Vehicles::GetVehicleByHandle(renderCorona->car)->velocity;
        auto dt = Mod::m_DeltaTime;

        auto probFps = 1000.0f / (float)dt;
        auto fixScale = probFps + 10.0f; //60fps = 70.0, 30fps = 40.0
        //if (!Mod::CoronaFixFPS == eCoronaFixFPS::FPS_AUTO) why
        switch (Mod::CoronaFixFPS)
        {
            case eCoronaFixFPS::FPS_30:
                fixScale = 40;
                break;
            case eCoronaFixFPS::FPS_45:
                fixScale = 55;
                break;
            case eCoronaFixFPS::FPS_60:
                fixScale = 70;
                break;
            default:
                break;
        }

        position.x += vel.x / fixScale;
        position.y += vel.y / fixScale;
        position.z += vel.z / fixScale;
    }

    if (renderCorona->useFlare)
    {
        float distance = DistanceBetween(Mod::m_PlayerPos, position);
        float distScale = 1.0f - (distance / renderCorona->flareDistance);

        distScale *= renderCorona->flareIntensity;

        unsigned char alpha = ucharIntensity(renderCorona->color.a, distScale);

        RegisterCorona(
            renderCorona->id + 1,
            0,
            renderCorona->color.r,
            renderCorona->color.g,
            renderCorona->color.b,
            alpha,
            { position.x, position.y, position.z },
            0.0f,
            1000.0f,
            renderCorona->coronaTexture,
            renderCorona->flareTexture,
            true,
            false,
            0,
            0.0f,
            false,
            renderCorona->nearClip,
            0,
            200.0f,
            false,
            false
        );
    }

    RegisterCorona(
        renderCorona->id,
        renderCorona->pVehicle, //test, and it worked, thank god
        renderCorona->color.r,
        renderCorona->color.g,
        renderCorona->color.b,
        renderCorona->color.a,
        { renderCorona->offset.x, renderCorona->offset.y, renderCorona->offset.z }, //{ position.x, position.y, position.z },
        renderCorona->radius,
        1000.0f,
        renderCorona->coronaTexture,
        0,
        true,
        false,
        0,
        0.0f,
        false,
        renderCorona->nearClip,
        0,
        10000.0f,
        false,
        false
    );
}

static void SEND_CURRENT_VEHICLE(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_CURRENT_VEHICLE car=%d modelId=%d", car, modelId);
    ///Log::file << szTemp << std::endl;

    if (car > 0)
    {
        Vehicles::TryCreateVehicle(car, modelId);
    }

    Mod::hPlayerVehicle = car;
}

static void SEND_TOUCH_STATE(__handler_params)
{
    int touchId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_TOUCH_STATE touchId=%d state=%d", touchId, state);
    //Log::file << szTemp << std::endl;

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

static void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    //Log::file << szTemp << std::endl;

    if (type == eDrawInfoType::MENU_OFFSET_X)
    {
        result->f = Menu::m_MenuOffset.x;
        return;
    }
    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {
        result->i = Draw::m_DrawItems.size();
        return;
    }
    if (type == eDrawInfoType::AMOUNT_OF_CORONAS)
    {
        result->i = Vehicles::m_CoronasToRender.size();
        return;
    }
    if (type == eDrawInfoType::CAR_GET_AMOUNT)
    {
        result->i = Vehicles::m_Vehicles.size();
        return;
    }

    bool carIdExceeds = (id > Vehicles::m_Vehicles.size() - 1);
    if (type == eDrawInfoType::CAR_GET_ID)
    {
        if (carIdExceeds) return;
        auto vehicle = Vehicles::GetVehicleByVecIndex(id);

        if (!vehicle)
        {
            Log::file << "Vehicle index " << id << " not found" << std::endl;
            return;
        }

        result->i = Vehicles::GetVehicleByVecIndex(id)->hVehicle;
        return;
    }
    /*
    * never gets called for some reason
    */
    if (type == eDrawInfoType::CAR_SET_TO_REMOVE)
    {
        if (carIdExceeds) return;
        auto vehicle = Vehicles::GetVehicleByHandle(id);

        if (!vehicle)
        {
            Log::file << "(CAR_SET_TO_REMOVE) Vehicle id " << id << " not found" << std::endl;
            return;
        }

        vehicle->canBeRemoved = true;
        Log::file << "(CAR_SET_TO_REMOVE) Vehicle id " << id << " set to remove" << std::endl;

        result->i = id;

        return;
    }

    //

    bool coronaIdExceeds = (id > Vehicles::m_CoronasToRender.size() - 1);

    /*
     if (coronaIdExceeds)
    {
        //Log::file << "corona id " << id << " out of range" << std::endl;
    }
    */

    if (type == eDrawInfoType::CORONA_CAR)
    {
        if (coronaIdExceeds) return;

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
        result->i = Vehicles::m_CoronasToRender[id].renderShadow ? 1 : 0;
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

        auto& renderCorona = Vehicles::m_CoronasToRender[id];

        auto dir = (renderCorona.offset.x > 0) ? 1 : -1;
        float margin = 0.15f;
        float distance = abs(renderCorona.offset.x);

        result->f = renderCorona.offset.x + (dir * renderCorona.shadowOffsetX);

        if (distance > margin)
        {
            result->f += (dir * renderCorona.shadowSize / 2);
        }

        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_OFFSET_Y)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];

        result->f = renderCorona.offset.y + renderCorona.shadowOffsetY;

        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_TEXTURE_ID)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        result->i = renderCorona.shadowTexture;

        return;
    }
    if (type == eDrawInfoType::CORONA_SHADOW_ROTATION)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        result->f = renderCorona.shadowRotation;

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

static void SEND_CAR_POSITION(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_POSITION car=%d, modelId=%d, x=%.2f, y=%.2f, z=%.2f", car, modelId, x, y, z);
    //Log::file << szTemp << std::endl;

    //Vehicles::TryCreateVehicle(car, modelId);

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::m_Vehicles[car];
    vehicle->position = CVector(x, y, z);
}


static void SEND_CAR_VELOCITY(__handler_params)
{
    int car = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_VELOCITY car=%d, x=%.2f, y=%.2f, z=%.2f", car, x, y, z);
    //Log::file << szTemp << std::endl;

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::GetVehicleByHandle(car);
    vehicle->velocity = CVector(x, y, z);
}


static void PROCESS_GIROFLEX_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;

    if (dt < 1) dt = 1;
    if (dt > 40) dt = 40;

    //Log::file << "PROCESS_GIROFLEX_LIB dt=" << dt << std::endl;

    //float lerpAmount = Mod::m_CoronaLerpNormal;
    //if (abs(dt - Mod::m_PrevDeltaTime) >= Mod::m_CoronaFixMinChange) lerpAmount = Mod::m_CoronaLerpTooMuch;

    //dt = (int)round(lerp((float)Mod::m_PrevDeltaTime, (float)dt, lerpAmount));

    //Log::file << "* fixed to dt=" << dt << std::endl;

    Mod::m_DeltaTime = dt;

    while (Draw::m_DrawItems.size() > 0) {
        auto dw = Draw::m_DrawItems[0];
        Draw::m_DrawItems.erase(std::find(Draw::m_DrawItems.begin(), Draw::m_DrawItems.end(), dw));
        delete dw;
    }
    //Draw::m_DrawItems.clear();

    Vehicles::Update(dt);

    Menu::Update(dt);

    WindowSettings::Update();
    WindowSettings::Draw();

    WindowPanel::Update();
    WindowPanel::Draw();

    Menu::Draw();

    Mod::ProcessTouch();

    Input::Update(dt);

    if (Menu::m_DrawCursor)
    {
        Draw::DrawText(1, Vehicles::m_CoronasToRender.size(), 0, CVector2D(20, 300), CRGBA(255, 255, 0));
        Draw::DrawText(1, Vehicles::m_Vehicles.size(), 0, CVector2D(20, 320), CRGBA(255, 255, 0));
        Draw::DrawText(1, Draw::m_DrawItems.size(), 0, CVector2D(20, 340), CRGBA(255, 255, 0));
        Draw::DrawText(1, dt, 0, CVector2D(20, 360), CRGBA(255, 255, 0));

        auto probFps = 1000.0f / (float)dt;
        auto fixScale = probFps + 10.0f;
        switch (Mod::CoronaFixFPS)
        {
        case eCoronaFixFPS::FPS_30:
            fixScale = 40;
            break;
        case eCoronaFixFPS::FPS_45:
            fixScale = 55;
            break;
        case eCoronaFixFPS::FPS_60:
            fixScale = 70;
            break;
        default:
            break;
        }
        Draw::DrawText(1, (int)fixScale, 0, CVector2D(20, 380), CRGBA(255, 240, 0));
    }

    if (!Mod::HasShownCredits)
    {
        Mod::HasShownCredits = true;

        /*
        Log::file << "Show credits" << std::endl;

        Menu::ShowPopup(29, 0, 0, 2500, 270.0f);
        */

        /*
        char buffer[256];
        sprintf(buffer, "Using Giroflex VSL v%s (by Danilo1301)", Mod::Version);
        if (aml->HasModOfVersion("net.rusjj.aml", "1.0.2"))
        {
            aml->ShowToast(false, buffer);
        }
        */
    }

    Mod::m_PrevDeltaTime = dt;
}

static void RUN_TEST(__handler_params)
{
    int testType = __readParam(handle)->i;

    //Log::opcodes << "RUN_TEST testType=" << testType << std::endl;

    if (testType == 2)
    {
        Draw::CFont_PrintString(200, 200, "abcdefghijklmnopqrstuvj");
    }
}
