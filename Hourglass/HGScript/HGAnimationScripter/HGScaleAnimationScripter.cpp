/**
 *  @file    HGScaleAnimationScripter.cpp
 *  @brief   Scale animation script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/15
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

#include "HGScaleAnimationScripter.h"
#include "HGIAnimationScripter.h"
#include "HGScaleAnimation.h"

#define SCALEANIMATION_METATABLE    "ScaleAnimationMetatable"
#define SCALEANIMATION_LUA_NAME     "ScaleAnimation"

namespace HG
{
    
ScaleAnimation* scaleanimation_check(lua_State* L, int idx)
{
    ScaleAnimation* anim = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    anim = *static_cast<ScaleAnimation **>(luaL_checkudata(L, idx, SCALEANIMATION_METATABLE));
    
    BREAK_END;
    
    return anim;
}
    
// push a ScaleAnimation to lua
int scaleanimation_push(lua_State* L, ScaleAnimation* anim)
{
    int ret = 0;
    
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    ScaleAnimation** udata = static_cast<ScaleAnimation **>(lua_newuserdata(L, sizeof(ScaleAnimation *)));
    *udata = anim;
    luaL_getmetatable(L, SCALEANIMATION_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
// ScaleAnimation methods
static int scaleanimation_set_scale(lua_State* L)
{
    BREAK_START;
    
    ScaleAnimation* anim = scaleanimation_check(L, 1);
    if (anim == NULL)
        break;
    
    float sx = (float)luaL_checknumber(L, 2);
    float sy = (float)luaL_checknumber(L, 3);
    anim->setScale(sx, sy);
    
    BREAK_END;
    
    return 0;
}
    
static int scaleanimation_set_duration(lua_State* L)
{
    BREAK_START;
    
    ScaleAnimation* anim = scaleanimation_check(L, 1);
    if (anim == NULL)
        break;
    
    float duration = (float)luaL_checknumber(L, 2);
    anim->setDuration(duration);
    
    BREAK_END;
    
    return 0;
}
    
static int scaleanimation_set_type(lua_State* L)
{
    BREAK_START;
    
    ScaleAnimation* anim = scaleanimation_check(L, 1);
    if (anim == NULL)
        break;
    
    ANIMATION_TYPE type = (ANIMATION_TYPE)luaL_checkinteger(L, 2);
    anim->setType(type);
    
    BREAK_END;
    
    return 0;
}
    
static int scaleanimation_new(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    float sx = (float)luaL_checknumber(L, 1);
    float sy = (float)luaL_checknumber(L, 2);
    float d = (float)luaL_checknumber(L, 3);
    
    ScaleAnimation* anim = new ScaleAnimation(sx, sy, d);
    ret = scaleanimation_push(L, anim);
    
    BREAK_END;
    
    return ret;
}
    
static int scaleanimation_delete(lua_State* L)
{
    BREAK_START;
    
    ScaleAnimation* anim = scaleanimation_check(L, 1);
    delete anim;
    
    BREAK_END;
    
    return 0;
}
    
luaL_Reg sScaleAnimationRegs[] =
{
    { "setScale",       scaleanimation_set_scale },
    { "setDuration",    scaleanimation_set_duration },
    { "setType",        scaleanimation_set_type },
    { NULL, NULL }
};
    
void ScriptRegisterScaleAnimation(lua_State* L)
{
    luaL_newmetatable(L, SCALEANIMATION_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIAnimationRegs, 0);
    luaL_setfuncs(L, sScaleAnimationRegs, 0);
    
    lua_pop(L, 1);
    
    // register ScaleAnimation class to lua
    lua_newtable(L);
    
    lua_pushstring(L, "new");
    lua_pushcfunction(L, scaleanimation_new);
    lua_settable(L, -3);
    
    lua_pushstring(L, "delete");
    lua_pushcfunction(L, scaleanimation_delete);
    lua_settable(L, -3);
    
    lua_setglobal(L, SCALEANIMATION_LUA_NAME);
}
    
}
