/**
 *  @file    HGAlphaAnimationScripter.cpp
 *  @brief   Alpha animation script adapter implementation
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

#include "HGAlphaAnimationScripter.h"
#include "HGIAnimationScripter.h"
#include "HGAlphaAnimation.h"

#define ALPHAANIMATION_METATABLE    "AlphaAnimationMetatable"
#define ALPHAANIMATION_LUA_NAME     "AlphaAnimation"

HGNAMESPACE_START
    
AlphaAnimation* alphaanimation_check(lua_State* L, int idx)
{
    AlphaAnimation* anim = NULL;
    
    if (lua_isuserdata(L, idx))
        anim = *static_cast<AlphaAnimation **>(luaL_checkudata(L, idx, ALPHAANIMATION_METATABLE));
    
    return anim;
}
    
// push a AlphaAnimation to lua
int alphaanimation_push(lua_State* L, AlphaAnimation* anim)
{
    int ret = 0;
    
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    AlphaAnimation** udata = static_cast<AlphaAnimation **>(lua_newuserdata(L, sizeof(AlphaAnimation *)));
    *udata = anim;
    luaL_getmetatable(L, ALPHAANIMATION_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
// AlphaAnimation methods
static int alphaanimation_set_alpha(lua_State* L)
{
    AlphaAnimation* anim = alphaanimation_check(L, 1);
    if (anim != NULL)
    {
        float alpha = (float)luaL_checknumber(L, 2);
        anim->setAlpha(alpha);
    }
    
    return 0;
}
    
static int alphaanimation_set_duration(lua_State* L)
{
    AlphaAnimation* anim = alphaanimation_check(L, 1);
    if (anim != NULL)
    {
        float duration = (float)luaL_checknumber(L, 2);
        anim->setDuration(duration);
    }
    
    return 0;
}
    
static int alphaanimation_new(lua_State* L)
{
    int ret = 0;
    
    float alpha = (float)luaL_checknumber(L, 1);
    float duration = (float)luaL_checknumber(L, 2);
    
    AlphaAnimation* anim = new AlphaAnimation(alpha, duration);
    ret = alphaanimation_push(L, anim);
    
    return ret;
}
    
static int alphaanimation_delete(lua_State* L)
{
    AlphaAnimation* anim = alphaanimation_check(L, 1);
    delete anim;
    
    return 0;
}
    
luaL_Reg sAlphaAnimationRegs[] =
{
    { "setAlpha",       alphaanimation_set_alpha },
    { "setDuration",    alphaanimation_set_duration },
    { NULL, NULL }
};
    
void ScriptRegisterAlphaAnimation(lua_State* L)
{
    luaL_newmetatable(L, ALPHAANIMATION_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIAnimationRegs, 0);
    luaL_setfuncs(L, sAlphaAnimationRegs, 0);
    
    lua_pop(L, 1);
    
    // register AlphaAnimation class to lua
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", alphaanimation_new);
    luaH_setfunc2table(L, "delete", alphaanimation_delete);
    
    lua_setglobal(L, ALPHAANIMATION_LUA_NAME);
}
    
HGNAMESPACE_END
