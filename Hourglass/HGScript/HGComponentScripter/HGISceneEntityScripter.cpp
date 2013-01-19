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

HGNAMESPACE_START
    
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
    
luaL_Reg sISceneEntityRegs[] =
{
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
    
HGNAMESPACE_END