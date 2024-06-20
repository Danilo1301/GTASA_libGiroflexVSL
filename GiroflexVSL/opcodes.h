#include <stdint.h>

#include "cleo.h"

#include <mod/logger.h>

#include "menu/Draw.h"
#include "menu/Menu.h"

#include "Input.h"
#include "Vehicles.h"
#include "Globals.h"
#include "Widgets.h"

#include "eModInfoType.h"


#define __decl_op(__name, __int)	const char* NAME_##__name = #__name; const uint16_t OP_##__name = __int;

#define __handler_params 			void *handle, uint32_t *ip, uint16_t opcode, const char *name

// Size of array
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

#define __reg_opcode				cleo->RegisterOpcode

#define __reg_func					cleo->RegisterOpcodeFunction

#define __op_name_match(x) 			opcode == OP_##x || strcmp(name, NAME_##x) == 0

#define __reg_op_func(x, h) 		__reg_opcode(OP_##x, h); __reg_func(NAME_##x, h);

#define __readParam(handle)         reinterpret_cast<cleo_ifs_t::data_t*>(cleo->ReadParam(handle))
#define __getPointerToScriptVar(handle)         reinterpret_cast<cleo_ifs_t::data_t*>(cleo->GetPointerToScriptVar(handle))

extern cleo_ifs_t* cleo;

extern void (*RegisterCorona)(unsigned int id, void* attachTo, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, CVector const& posn, float radius, float farClip, int coronaType, int flaretype, bool enableReflection, bool checkObstacles, int _param_not_used, float angle, bool longDistance, float nearClip, unsigned char fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay);

__decl_op(PROCESS_GIROFLEX_VSL_LIB, 0x0EF0); // 0EF0=1,process_giroflex_vsl_lib deltaMs %1d%
__decl_op(GET_DRAW_ITEM_INFO, 0x0EF1); // 0EF1=3,%3g% = get_draw_item_info %1d% id %2d%
__decl_op(SEND_TOUCH_STATE, 0x0EF2); //0EF2=2,send_touch_state %1d% state %2d%
__decl_op(SEND_PLAYER_POSITION, 0x0EF3); // 0EF3=4,send_player_position %1d% pos %2f% %3f% %4f%
__decl_op(SEND_CURRENT_VEHICLE, 0x0EF4); //0EF4=2,send_current_vehicle %1d% modelid %2d%
__decl_op(GET_MOD_INFO, 0x0EF5); // 0EF5=3,%3g% = get_mod_info %1d% id %2d%
__decl_op(SEND_CAR_VELOCITY, 0x0EF6); // 0EF6=4,send_car_velocity %1d% vel %2f% %3f% %4f%
__decl_op(CREATE_NEW_VEHICLE, 0x0EF7); // 0EF7=2,create_new_vehicle %1d% modelid %2d%
__decl_op(REGISTER_GIROFLEX_CORONA, 0x0EF8); // 0EF8=4,register_giroflex_corona %1d% at_pos %2f% %3f% %4f%
__decl_op(SEND_CAR_POSITION, 0x0EF9); // 0EF9=5,send_car_position %1d% model_id %2d% pos %3f% %4f% %5f%
__decl_op(ADD_LOG_MESSAGE, 0x0EFA); //0EFA=1,add_log_message %1d%
__decl_op(SEND_WIDGET_STATE, 0x0EFB); //0EFB=2,send_widget_state %1d% state %2d%

static void PROCESS_GIROFLEX_VSL_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;
    
    //Log::Level(LOG_LEVEL::LOG_BOTH) << "PROCESS_MENU_VSL_LIB dt=" << dt << std::endl;
    //logger->Info("PROCESS_MENU_VSL_LIB dt=%d", dt);

    GiroflexVSL::Update(dt);

    //Log::Level(LOG_LEVEL::LOG_BOTH) << "PROCESS_MENU_VSL_LIB end" << std::endl;
}

static void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {
        result->i = Draw::m_DrawItems.size();
        return;
    }

    if (type == eDrawInfoType::MENU_OFFSET_X)
    {
        result->f = Menu::m_MenuOffset.x;
        return;
    }

    auto item = Draw::m_DrawItems[id];

    if (type == eDrawInfoType::TYPE)
    {
        result->i = (int)item->type;
        return;
    }

    if (type == eDrawInfoType::POS_X)
    {
        result->f = item->pos.x + item->size.x / 2.0f; //add, so it draw centered
        return;
    }

    if (type == eDrawInfoType::POS_Y)
    {
        result->f = item->pos.y + item->size.y / 2.0f; //add, so it draw centered
        return;
    }

    if (type == eDrawInfoType::SIZE_X) 
    {
        result->f = item->size.x;
        return;
    }

    if (type == eDrawInfoType::SIZE_Y)
    {
        result->f = item->size.y;
        return;
    }

    if (type == eDrawInfoType::COLOR_R) 
    {
        result->i = item->color.r;
        return;
    }

    if (type == eDrawInfoType::COLOR_G)
    {
        result->i = item->color.g;
        return;
    }

    if (type == eDrawInfoType::COLOR_B)
    {
        result->i = item->color.b;
        return;
    }

    if (type == eDrawInfoType::COLOR_A)
    {
        result->i = item->color.a;
        return;
    }

    if (type == eDrawInfoType::GXT_ID)
    {
        result->i = item->gxtId;
        return;
    }

    if (type == eDrawInfoType::NUM_1)
    {
        result->i = item->num1;
        return;
    }

    if (type == eDrawInfoType::NUM_2)
    {
        result->i = item->num2;
    }

    if (type == eDrawInfoType::ALIGN)
    {
        result->i = (int)item->textAlign;
        return;
    }

    if (type == eDrawInfoType::SPRITE_ID)
    {
        result->i = item->spriteId;
        return;
    }
}

static void SEND_TOUCH_STATE(__handler_params)
{
    int touchId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_TOUCH_STATE touchId=%d state=%d", touchId, state);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    Input::SetTouchState(touchId, state == 1);
}

static void SEND_PLAYER_POSITION(__handler_params)
{
    int id = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    Globals::m_PlayerPos = CVector(x, y, z);
}

static void SEND_CURRENT_VEHICLE(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_CURRENT_VEHICLE car=%d modelId=%d", car, modelId);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (car > 0)
    {
        Vehicles::TryCreateVehicle(car, modelId);
    }

    Globals::hPlayerVehicle = car;
}


static void GET_MOD_INFO(__handler_params)
{
    eModInfoType type = (eModInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_MOD_INFO type=%d, id=%d", type, id);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (type == eModInfoType::CAR_GET_AMOUNT)
    {
        result->i = Vehicles::m_Vehicles.size();
        return;
    }

    bool carIdExceeds = (id > Vehicles::m_Vehicles.size() - 1);

    if (type == eModInfoType::CAR_GET_ID)
    {
        if (carIdExceeds) return;
        auto vehicle = Vehicles::GetVehicleByVecIndex(id);

        if (!vehicle)
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << "Vehicle index " << id << " not found" << std::endl;
            return;
        }

        result->i = Vehicles::GetVehicleByVecIndex(id)->hVehicle;
        return;
    }
    /*
    * never gets called for some reason
    */
    if (type == eModInfoType::CAR_SET_TO_REMOVE)
    {
        if (carIdExceeds) return;
        auto vehicle = Vehicles::GetVehicleByHandle(id);

        if (!vehicle)
        {
            Log::Level(LOG_LEVEL::LOG_BOTH) << "(CAR_SET_TO_REMOVE) Vehicle id " << id << " not found" << std::endl;
            return;
        }

        vehicle->canBeRemoved = true;
        Log::Level(LOG_LEVEL::LOG_BOTH) << "(CAR_SET_TO_REMOVE) Vehicle id " << id << " set to remove" << std::endl;

        result->i = id;

        return;
    }

    if (type == eModInfoType::CHECKVEHICLE_GET_AMOUNT)
    {
        result->i = Vehicles::m_NewVehiclesRef.size();
        return;
    }

    if (type == eModInfoType::CHECKVEHICLE_GET_ID)
    {
        result->i = Vehicles::m_NewVehiclesRef[id];
        return;
    }

    if (type == eModInfoType::AMOUNT_OF_CORONAS)
    {
        result->i = Vehicles::m_CoronasToRender.size();
        return;
    }

    bool coronaIdExceeds = (id > Vehicles::m_CoronasToRender.size() - 1);

    if (type == eModInfoType::CORONA_CAR)
    {
        if (coronaIdExceeds) return;

        result->i = Vehicles::m_CoronasToRender[id].car;
        return;
    }

    if (type == eModInfoType::CORONA_OFFSET_X)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.x;
        return;
    }

    if (type == eModInfoType::CORONA_OFFSET_Y)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.y;
        return;
    }

    if (type == eModInfoType::CORONA_OFFSET_Z)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].offset.z;
        return;
    }

    if (type == eModInfoType::CORONA_USE_POINT_LIGHT)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].renderPointLight ? 1 : 0;
        return;
    }

    if (type == eModInfoType::CORONA_USE_SHADOW)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].renderShadow ? 1 : 0;
        return;
    }

    if (type == eModInfoType::CORONA_R)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.r;
        return;
    }

    if (type == eModInfoType::CORONA_G)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.g;
        return;
    }

    if (type == eModInfoType::CORONA_B)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.b;
        return;
    }

    if (type == eModInfoType::CORONA_A)
    {
        if (coronaIdExceeds) return;
        result->i = Vehicles::m_CoronasToRender[id].color.a;
        return;
    }

    if (type == eModInfoType::CORONA_SHADOW_INTENSITY)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].shadowIntensity;
        return;
    }
    if (type == eModInfoType::CORONA_SHADOW_SIZE)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].shadowSize;
        return;
    }

    if (type == eModInfoType::CORONA_POINTLIGHT_DISTANCE)
    {
        if (coronaIdExceeds) return;
        result->f = Vehicles::m_CoronasToRender[id].pointLightDistance;
        return;
    }

    if (type == eModInfoType::SHADOW_R)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.r,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }
    
    if (type == eModInfoType::SHADOW_G)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.g,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }

    if (type == eModInfoType::SHADOW_B)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.b,
            Vehicles::m_CoronasToRender[id].shadowIntensity
        );
        return;
    }

    if (type == eModInfoType::POINT_LIGHT_R)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.r,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }

    if (type == eModInfoType::POINT_LIGHT_G)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.g,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }

    if (type == eModInfoType::POINT_LIGHT_B)
    {
        if (coronaIdExceeds) return;
        result->i = ucharIntensity(
            Vehicles::m_CoronasToRender[id].color.b,
            Vehicles::m_CoronasToRender[id].pointLightIntensity
        );
        return;
    }

    if (type == eModInfoType::CORONA_SHADOW_OFFSET_X)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        result->f = renderCorona.offset.x + renderCorona.shadowOffsetX;

        return;
    }

    if (type == eModInfoType::CORONA_SHADOW_OFFSET_Y)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        result->f = renderCorona.offset.y + renderCorona.shadowOffsetY;

        return;
    }

    if (type == eModInfoType::CORONA_SHADOW_TEXTURE_ID)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        result->i = renderCorona.shadowTexture;

        return;
    }

    if (type == eModInfoType::CORONA_SHADOW_ROTATION)
    {
        if (coronaIdExceeds) return;

        auto& renderCorona = Vehicles::m_CoronasToRender[id];
        
        result->f = renderCorona.shadowRotation;

        if (renderCorona.shadowFlipTextures)
        {
            float margin = 0.15f;
            float distance = abs(renderCorona.offset.x);

            if (distance > margin)
            {
                float addRotation = renderCorona.offset.x > 0 ? 3.1415f : 0.0f;

                result->f += renderCorona.shadowRotation + addRotation;
            }
        }

        return;
    }
}

static void SEND_CAR_VELOCITY(__handler_params)
{
    int car = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_VELOCITY car=%d, x=%.2f, y=%.2f, z=%.2f", car, x, y, z);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::GetVehicleByHandle(car);
    vehicle->velocity = CVector(x, y, z);
}

static void CREATE_NEW_VEHICLE(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;

    //char szTemp[256];
    //sprintf(szTemp, "CREATE_NEW_VEHICLE car=%d modelId=%d", car, modelId);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (car > 0)
    {
        Vehicles::TryCreateVehicle(car, modelId);
    }
}

static void REGISTER_GIROFLEX_CORONA(__handler_params)
{
    int id = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "REGISTER_GIROFLEX_CORONA id=%d at_pos %.2f %.2f %.2f", id, x, y, z);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    if (id > Vehicles::m_CoronasToRender.size() - 1)
    {
        /*

        issue here too
        */

        //Log::Level(LOG_LEVEL::LOG_BOTH) << "REGISTER_GIROFLEX_CORONA id exceeds" << std::endl;
        return;
    }

    auto renderCorona = &Vehicles::m_CoronasToRender[id];

    CVector position = CVector(x, y, z);

    if (renderCorona->car > 0 && Vehicles::HasVehicleHandle(renderCorona->car))
    {
        /*
        auto vel = Vehicles::GetVehicleByHandle(renderCorona->car)->velocity;
        auto dt = Globals::m_DeltaTime;
        
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
        */
    }

    if (renderCorona->useFlare)
    {
        float distance = DistanceBetween(Globals::m_PlayerPos, position);
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

static void SEND_CAR_POSITION(__handler_params)
{
    int car = __readParam(handle)->i;
    int modelId = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float z = __readParam(handle)->f;

    char szTemp[256];
    sprintf(szTemp, "SEND_CAR_POSITION car=%d, modelId=%d, x=%.2f, y=%.2f, z=%.2f", car, modelId, x, y, z);
    //Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;

    //Vehicles::TryCreateVehicle(car, modelId);

    if (!Vehicles::HasVehicleHandle(car)) return;

    auto vehicle = Vehicles::m_Vehicles[car];
    vehicle->position = CVector(x, y, z);
}

static void ADD_LOG_MESSAGE(__handler_params)
{
    int num = __readParam(handle)->i;
    
    Log::Level(LOG_LEVEL::LOG_UPDATE) << "Log: CLEO " << num << std::endl;
}

static void SEND_WIDGET_STATE(__handler_params)
{
    int widgetId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    /*/
    char szTemp[256];
    sprintf(szTemp, "SEND_WIDGET_STATE widgetId=%d state=%d", widgetId, state);
    Log::Level(LOG_LEVEL::LOG_BOTH) << szTemp << std::endl;
    */

    Widgets::SetWidgetState(widgetId, state == 1);
}