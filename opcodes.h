#include <stdint.h>

#include "cleo.h"

#include <mod/logger.h>

#include "menu/Draw.h"
#include "menu/Menu.h"
#include "Input.h"

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

__decl_op(PROCESS_GIROFLEX_VSL_LIB, 0x0EF0); // 0EF0=1,process_giroflex_vsl_lib deltaMs %1d%
__decl_op(GET_DRAW_ITEM_INFO, 0x0EF1); // 0EF1=3,%3g% = get_draw_item_info %1d% id %2d%
__decl_op(SEND_TOUCH_STATE, 0x0EF2); //0EF2=2,send_touch_state %1d% state %2d%

static void PROCESS_GIROFLEX_VSL_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;
    
    //Log::file << "PROCESS_MENU_VSL_LIB dt=" << dt << std::endl;
    //logger->Info("PROCESS_MENU_VSL_LIB dt=%d", dt);

    GiroflexVSL::Update(dt);
}

static void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    //Log::file << szTemp << std::endl;

    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {
        result->i = Draw::m_DrawItems.size();
        return;
    }

    if (type == eDrawInfoType::MENU_OFFSET_X)
    {
        Log::file << "MENU_OFFSET_X " << Menu::m_MenuOffset.x << std::endl;
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
    Log::file << szTemp << std::endl;

    Input::SetTouchState(touchId, state == 1);
}