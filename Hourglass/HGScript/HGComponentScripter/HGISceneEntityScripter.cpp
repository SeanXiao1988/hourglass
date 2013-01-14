/**
 *  @file    HGISceneEntityScripter.h
 *  @brief   ISceneEntity script implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/06
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

#include "HGISceneEntityScripter.h"
#include "HGQuadEntityScripter.h"
#include "HGISceneEntity.h"
#include "HGIAnimationScripter.h"
#include "HGHash.h"

#define ISCENEENTITY_METATABLE "ISceneEntityMetatable"

namespace HG
{
    
ISceneEntity* isceneentity_check(lua_State* L, int idx)
{
    ISceneEntity* entity = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    entity = *static_cast<ISceneEntity **>(lua_touserdata(L, idx));
    
    BREAK_END;
    
    return entity;
}
    
static int isceneentity_add_animation(lua_State* L)
{
    BREAK_START;
    
    ISceneEntity* entity = NULL;
    entity = isceneentity_check(L, 1);
    if (entity == NULL)
        break;
    
    IAnimation* anim = ianimation_check(L, 2);
    if (anim == NULL)
        break;
    
    uint32_t name = 0;
    if (lua_isstring(L, 3))
    {
        const char* animName = luaL_checkstring(L, 3);
        name = Hash(animName);
    }
    else if(lua_isnumber(L, 3))
    {
        name = luaL_checkunsigned(L, 3);
    }
    
    entity->addAnimation(anim, name);
    
    BREAK_END;
    
    return 0;
}
    
static int isceneentity_get_animation(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    ISceneEntity* entity = NULL;
    entity = isceneentity_check(L, 1);
    if (entity == NULL)
        break;
    
    uint32_t name = 0;
    if (lua_isstring(L, 2))
    {
        const char* animName = luaL_checkstring(L, 2);
        name = Hash(animName);
    }
    else if (lua_isnumber(L, 2))
    {
        name = luaL_checkunsigned(L, 2);
    }
    
    IAnimation* anim = entity->getAnimation(name);
    if (anim == NULL)
        break;
    
    ret = ianimation_push(L, anim);
    
    BREAK_END;
    
    return ret;
}
    
static int isceneentity_remove_animation(lua_State* L)
{
    BREAK_START;
    
    ISceneEntity* entity = NULL;
    entity = isceneentity_check(L, 1);
    if (entity == NULL)
        break;
    
    uint32_t name = 0;
    if (lua_isstring(L, 2))
    {
        const char* animName = luaL_checkstring(L, 2);
        name = Hash(animName);
    }
    else if (lua_isnumber(L, 2))
    {
        name = luaL_checkunsigned(L, 2);
    }
    
    entity->removeAnimation(name);
    
    BREAK_END;
    
    return 0;
}
    
static int isceneentity_remove_all_animations(lua_State* L)
{
    BREAK_START;
    
    ISceneEntity* entity = NULL;
    entity = isceneentity_check(L, 1);
    if (entity == NULL)
        break;
    
    entity->removeAllAnimations();
    
    BREAK_END;
    
    return 0;
}
    
luaL_Reg sISceneEntityRegs[] =
{
    { "addAnimation",           isceneentity_add_animation },
    { "getAnimation",           isceneentity_get_animation },
    { "removeAnimation",        isceneentity_remove_animation },
    { "removeAllAnimations",    isceneentity_remove_all_animations },
    { NULL, NULL }
};
    
void ScriptRegisterISceneEntity(lua_State* L)
{
    luaL_newmetatable(L, ISCENEENTITY_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sISceneEntityRegs, 0);
    
    lua_pop(L, 1);
}
    
}