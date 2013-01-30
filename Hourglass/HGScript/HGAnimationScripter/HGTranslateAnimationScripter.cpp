/**
 *  @file    HGTranslateAnimationScripter.cpp
 *  @brief   Translate animation script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/13
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

#include "HGTranslateAnimationScripter.h"
#include "HGIAnimationScripter.h"
#include "HGTranslateAnimation.h"

#define TRANSLATEANIMATION_METATABLE    "TranslateAnimationMetatable"
#define TRANSLATEANIMATION_LUA_NAME     "TranslateAnimation"

HGNAMESPACE_START
    
TranslateAnimation* translateanimation_check(lua_State* L, int idx)
{
    TranslateAnimation* anim = NULL;
    
    if (lua_isuserdata(L, idx))
        anim = *static_cast<TranslateAnimation **>(luaL_checkudata(L, idx, TRANSLATEANIMATION_METATABLE));
    
    return anim;
}
    
// push a TranslateAnimation to Lua
int translateanimation_push(lua_State* L, TranslateAnimation* anim)
{
    int ret = 0;
    
    if (anim != NULL)
    {
        TranslateAnimation** udata = static_cast<TranslateAnimation **>(lua_newuserdata(L, sizeof(TranslateAnimation *)));
        *udata = anim;
        luaL_getmetatable(L, TRANSLATEANIMATION_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}
    
// TranslateAnimation methods
static int translateanimation_set_translate(lua_State* L)
{
    TranslateAnimation* anim = translateanimation_check(L, 1);
    if (anim != NULL)
    {
        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        anim->setTranslate(x, y);
    }
    
    return 0;
}
    
static int translateanimation_set_duration(lua_State* L)
{
    TranslateAnimation* anim = translateanimation_check(L, 1);
    if (anim != NULL)
    {
        float duration = (float)luaL_checknumber(L, 2);
        anim->setDuration(duration);
    }
    
    return 0;
}
    
static int translateanimation_new(lua_State* L)
{
    int ret = 0;
    
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float d = (float)luaL_checknumber(L, 3);
    
    TranslateAnimation* anim = new TranslateAnimation(x, y, d);
    ret = translateanimation_push(L, anim);
    
    return ret;
}
    
static int translateanimation_delete(lua_State* L)
{
    TranslateAnimation* anim = translateanimation_check(L, 1);
    delete anim;
    
    return 0;
}
    
luaL_Reg sTranslateAnimationRegs[] =
{
    { "setTranslate",   translateanimation_set_translate },
    { "setDuration",    translateanimation_set_duration },
    { NULL, NULL }
};

void ScriptRegisterTranslateAnimation(lua_State* L)
{
    luaL_newmetatable(L, TRANSLATEANIMATION_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIAnimationRegs, 0);
    luaL_setfuncs(L, sTranslateAnimationRegs, 0);
    
    lua_pop(L, 1);
    
    // register TranslateAnimation class to lua
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", translateanimation_new);
    luaH_setfunc2table(L, "delete", translateanimation_delete);
    
    lua_setglobal(L, TRANSLATEANIMATION_LUA_NAME);
}
    
HGNAMESPACE_END