/**
 *  @file    HGIAnimationScripter.cpp
 *  @brief   IAnimation script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/12
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2013 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#include "HGIAnimationScripter.h"
#include "HGIAnimation.h"

#include "HGTranslateAnimationScripter.h"

#define IANIMATION_METATABLE "IAnimationMetatable"

namespace HG
{
    
IAnimation* ianimation_check(lua_State* L, int idx)
{
    IAnimation* anim = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    anim = *static_cast<IAnimation **>(lua_touserdata(L, idx));
    
    BREAK_END;
    
    return anim;
}
    
int ianimation_push(lua_State* L, IAnimation* anim)
{
    int ret = 0;
    
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    AnimationTypeID tid = anim->typeID;
    switch (tid)
    {
        case ANIMATION_ID_TRANSLATE:
            translateanimation_push(L, (TranslateAnimation *)anim);
            break;
            
        default:
            break;
    }
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
static int ianimation_is_finished(lua_State* L)
{
    int ret = 1;
    bool isFinished = false;
    
    BREAK_START;
    
    IAnimation* anim = NULL;
    anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    isFinished = anim->isFinished;
    
    BREAK_END;
    
    lua_pushboolean(L, isFinished?1:0);
    
    return ret;
}
    
static int ianimation_is_animating(lua_State* L)
{
    int ret = 1;
    bool isAnimating = true;
    
    BREAK_START;
    
    IAnimation* anim = NULL;
    anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    isAnimating = anim->isAnimating;
    
    BREAK_END;
    
    lua_pushboolean(L, isAnimating?1:0);
    
    return ret;
}
    
static int ianimation_animation_stage(lua_State* L)
{
    int ret = 1;
    ANIMATION_STAGE stage = ANIMATION_STAGE_FORWARD;
    
    BREAK_START;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    stage = anim->animationStage;
    
    BREAK_END;
    
    lua_pushinteger(L, stage);
    
    return ret;
}
    
static int ianimation_name(lua_State* L)
{
    int ret = 1;
    uint32_t name = 0;
    
    BREAK_START;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    name = anim->name;
    
    BREAK_END;
    
    lua_pushunsigned(L, name);
    
    return ret;
}
    
static int ianimation_type_id(lua_State* L)
{
    int ret = 1;
    AnimationTypeID tid = ANIMATION_ID_INVALID;
    
    BREAK_START;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    tid = anim->typeID;
    
    BREAK_END;
    
    lua_pushinteger(L, tid);
    
    return ret;
}
    
static int ianimation_type(lua_State* L)
{
    int ret = 1;
    ANIMATION_TYPE type = ANIMATION_TYPE_RESET;
    
    BREAK_START;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim == NULL)
        break;
    
    type = anim->type;
    
    BREAK_END;
    
    lua_pushinteger(L, type);
    
    return ret;
}
    
luaL_Reg sIAnimationRegs[] =
{
    { "isFinished",     ianimation_is_finished },
    { "isAnimating",    ianimation_is_animating },
    { "animationStage", ianimation_animation_stage },
    { "name",           ianimation_name },
    { "typeID",         ianimation_type_id },
    { "type",           ianimation_type },
    { NULL, NULL}
};
    
void ScriptRegisterIAnimation(lua_State* L)
{
    luaL_newmetatable(L, IANIMATION_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIAnimationRegs, 0);
    
    lua_pop(L, 1);
}

}
