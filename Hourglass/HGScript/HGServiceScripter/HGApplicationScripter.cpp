/**
 *  @file    HGApplicationScripter.cpp
 *  @brief   Application script adapter implementation
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

#include "HGApplicationScripter.h"
#include "HGApplication.h"

#define APPLICATION_LUA_NAME    "APPLICATION"

namespace HG
{
    
static int application_set_titile(lua_State* L)
{
    const char* title = luaL_checkstring(L, 1);
    APPLICATION.setAppTitle(title);
    
    return 0;
}
    
void ScriptRegisterApplication(lua_State* L)
{
    lua_newtable(L);
    
    lua_pushstring(L, "setTitle");
    lua_pushcfunction(L, application_set_titile);
    lua_settable(L, -3);
    
    lua_setglobal(L, APPLICATION_LUA_NAME);
}
    
}
