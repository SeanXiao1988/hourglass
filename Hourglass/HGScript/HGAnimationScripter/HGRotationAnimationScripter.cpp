/**
 *  @file    HGRotationAnimationScripter.cpp
 *  @brief   Rotation animation script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/14
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

#include "HGRotationAnimationScripter.h"
#include "HGIAnimationScripter.h"
#include "HGRotationAnimation.h"

#define ROTATIONANIMATION_METATABLE "RotationAnimationMetatable"
#define ROTATIONANIMATION_LUA_NAME  "RotationAnimation"

HGNAMESPACE_START
    
RotationAnimation* rotationanimation_check(lua_State* L, int idx)
{
    RotationAnimation* anim = NULL;

    if (lua_isuserdata(L, idx))
        anim = *static_cast<RotationAnimation **>(luaL_checkudata(L, idx, ROTATIONANIMATION_METATABLE));
    
    return anim;
}
    
// push a RotationAnimation to lua
int rotationanimation_push(lua_State* L, RotationAnimation* anim)
{
    int ret = 0;
    
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    RotationAnimation** udata = static_cast<RotationAnimation **>(lua_newuserdata(L, sizeof(RotationAnimation *)));
    *udata = anim;
    luaL_getmetatable(L, ROTATIONANIMATION_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
// RotationAnimation methods
static int rotationanimation_set_rotation(lua_State* L)
{
    BREAK_START;
    
    RotationAnimation* anim = rotationanimation_check(L, 1);
    if (anim == NULL)
        break;
    
    float rot = (float)luaL_checknumber(L, 2);
    anim->setRotation(rot);
    
    BREAK_END;
    
    return 0;
}
    
static int rotationanimation_set_duration(lua_State* L)
{
    BREAK_START;
    
    RotationAnimation* anim = rotationanimation_check(L, 1);
    if (anim == NULL)
        break;
    
    float duration = (float)luaL_checknumber(L, 2);
    anim->setDuration(duration);
    
    BREAK_END;
    
    return 0;
}
    
static int rotationanimation_new(lua_State* L)
{
    int ret = 0;
    
    float rot = (float)luaL_checknumber(L, 1);
    float dur = (float)luaL_checknumber(L, 2);
    
    RotationAnimation* anim = new RotationAnimation(rot, dur);
    ret = rotationanimation_push(L, anim);
    
    return ret;
}
 
static int rotationanimation_delete(lua_State* L)
{
    RotationAnimation* anim = rotationanimation_check(L, 1);
    delete anim;
    
    return 0;
}
    
luaL_Reg sRotationAnimationRegs[] =
{
    { "setRotation",    rotationanimation_set_rotation },
    { "setDuration",    rotationanimation_set_duration },
    { NULL, NULL }
};
    
void ScriptRegisterRotationAnimation(lua_State* L)
{
    luaL_newmetatable(L, ROTATIONANIMATION_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIAnimationRegs, 0);
    luaL_setfuncs(L, sRotationAnimationRegs, 0);
    
    lua_pop(L, 1);
    
    // register RotationAnimation class to lua
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", rotationanimation_new);
    luaH_setfunc2table(L, "delete", rotationanimation_delete);
    
    lua_setglobal(L, ROTATIONANIMATION_LUA_NAME);
}

HGNAMESPACE_END