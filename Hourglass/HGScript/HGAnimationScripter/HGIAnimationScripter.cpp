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

#include "HGAlphaAnimationScripter.h"
#include "HGRotationAnimationScripter.h"
#include "HGScaleAnimationScripter.h"
#include "HGTranslateAnimationScripter.h"

#define IANIMATION_METATABLE "IAnimationMetatable"

HGNAMESPACE_START
    
IAnimation* ianimation_check(lua_State* L, int idx)
{
    IAnimation* anim = NULL;
    
    if (lua_isuserdata(L, idx))
        anim = *static_cast<IAnimation **>(lua_touserdata(L, idx));
    
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
        case ANIMATION_ID_ALPHA:
            alphaanimation_push(L, (AlphaAnimation *)anim);
            break;
            
        case ANIMATION_ID_ROTATE:
            rotationanimation_push(L, (RotationAnimation *)anim);
            break;
        
        case ANIMATION_ID_SCALE:
            scaleanimation_push(L, (ScaleAnimation *)anim);
            break;
            
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
    
    IAnimation* anim = NULL;
    anim = ianimation_check(L, 1);
    if (anim != NULL)
        isFinished = anim->isFinished;
    
    luaH_pushboolean(L, isFinished);
    
    return ret;
}
    
static int ianimation_is_animating(lua_State* L)
{
    int ret = 1;
    bool isAnimating = true;
    
    IAnimation* anim = NULL;
    anim = ianimation_check(L, 1);
    if (anim != NULL)
        isAnimating = anim->isAnimating;
    
    luaH_pushboolean(L, isAnimating);
    
    return ret;
}
    
static int ianimation_animation_stage(lua_State* L)
{
    int ret = 1;
    ANIMATION_STAGE stage = ANIMATION_STAGE_FORWARD;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim != NULL)
        stage = anim->animationStage;
    
    lua_pushinteger(L, stage);
    
    return ret;
}
    
static int ianimation_name(lua_State* L)
{
    int ret = 1;
    uint32_t name = 0;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim != NULL)
        name = anim->name;
    
    lua_pushunsigned(L, name);
    
    return ret;
}
    
static int ianimation_type_id(lua_State* L)
{
    int ret = 1;
    AnimationTypeID tid = ANIMATION_ID_INVALID;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim != NULL)
        tid = anim->typeID;
    
    lua_pushinteger(L, tid);
    
    return ret;
}
    
static int ianimation_type(lua_State* L)
{
    int ret = 1;
    ANIMATION_TYPE type = ANIMATION_TYPE_RESET;
    
    IAnimation* anim = ianimation_check(L, 1);
    if (anim != NULL)
        type = anim->type;
    
    lua_pushinteger(L, type);
    
    return ret;
}
    
static int ianimation_set_type(lua_State* L)
{
    IAnimation* anim = ianimation_check(L, 1);
    if (anim != NULL)
    {
        ANIMATION_TYPE type = (ANIMATION_TYPE)luaL_checkinteger(L, 2);
        anim->type = type;
    }
    
    return 0;
}
    
luaL_Reg sIAnimationRegs[] =
{
    { "isFinished",     ianimation_is_finished },
    { "isAnimating",    ianimation_is_animating },
    { "animationStage", ianimation_animation_stage },
    { "name",           ianimation_name },
    { "typeID",         ianimation_type_id },
    { "type",           ianimation_type },
    { "setType",        ianimation_set_type },
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
    
    lua_pushinteger(L, ANIMATION_TYPE_DEFAULT);
    lua_setglobal(L, "ANIMATION_TYPE_DEFAULT");
    
    lua_pushinteger(L, ANIMATION_TYPE_PINGPONG);
    lua_setglobal(L, "ANIMATION_TYPE_PINGPONG");
    
    lua_pushinteger(L, ANIMATION_TYPE_RESET);
    lua_setglobal(L, "ANIMATION_TYPE_RESET");
    
    // register animations
    ScriptRegisterAlphaAnimation(L);
    ScriptRegisterRotationAnimation(L);
    ScriptRegisterScaleAnimation(L);
    ScriptRegisterTranslateAnimation(L);
}

HGNAMESPACE_END