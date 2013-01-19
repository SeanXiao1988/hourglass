/**
 *  @file    HGHashScripter.cpp
 *  @brief   Hash scripter adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/27
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2012 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#include "HGHash.h"
#include "HGHashScripter.h"

HGNAMESPACE_START
    
static int hash_make(lua_State* L)
{
    uint32_t value = 0;
    
    const char* str = luaL_checkstring(L, 1);
    if (str != NULL)
        value = Hash(str);
    
    lua_pushunsigned(L, value);
    
    return 1;
}
    
static int hash_string_for_hash(lua_State* L)
{
    uint32_t value = 0;
    
    value = luaL_checkunsigned(L, 1);
    std::string str = Hash::StringForHash(value);
    lua_pushstring(L, str.c_str());
    
    return 1;
}
    
void ScriptRegisterHash(lua_State* L)
{
    lua_newtable(L);
    
    lua_pushstring(L, "make");
    lua_pushcfunction(L, hash_make);
    lua_settable(L, -3);
    
    lua_pushstring(L, "stringForHash");
    lua_pushcfunction(L, hash_string_for_hash);
    lua_settable(L, -3);
    
    lua_setglobal(L, "HASH");
}
    
HGNAMESPACE_END