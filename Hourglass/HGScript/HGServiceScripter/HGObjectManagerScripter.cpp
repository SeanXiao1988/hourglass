/**
 *  @file    HGObjectManagerScripter.cpp
 *  @brief   ObjectManager script adapter
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

#include "HGObjectManagerScripter.h"
#include "HGObjectManager.h"
#include "HGCompositeScripter.h"
#include "HGHash.h"

namespace HG
{

static int objectmanager_is_object_exists(lua_State* L)
{
    int ret = 1;
    uint32_t objectNameHash = 0;
    bool exist = false;
    
    BREAK_START;
    
    if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
    }
    else if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    
    exist = OBJECTMANAGER.isObjectExists(objectNameHash);

    BREAK_END;
    
    lua_pushboolean(L, exist?1:0);

    return ret;
}
    
static int objectmanager_delete_object(lua_State* L)
{
    int ret = 0;
    uint32_t objectNameHash = 0;
    
    BREAK_START;
    
    if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
    }
    else if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    
    OBJECTMANAGER.deleteObject(objectNameHash);
    
    BREAK_END;
    
    return ret;
}
    
static int objectmanager_delete_objects(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    OBJECTMANAGER.deleteObjects();

    BREAK_END;
    
    return ret;
}
    
static int objectmanager_get_object_components(lua_State* L)
{
    int ret = 0;
    uint32_t objectNameHash = 0;
    
    BREAK_START;
    
    if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
    }
    else if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    
    ComponentsMap* map = OBJECTMANAGER.getObjectComponents(objectNameHash);
    
    if (map == NULL)
        break;
    
    lua_newtable(L);
    
    ComponentsMap::iterator iter = map->begin();
    for (; iter != map->end(); ++iter)
    {
        // push component type as key
        lua_pushinteger(L, (int)iter->first);
        // push component as value
        icomponent_push(L, iter->second);
        lua_settable(L, -3);
    }
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

void ScriptRegisterObjectManager(lua_State* L)
{
    BREAK_START;

    if (L == NULL)
        break;

    lua_newtable(L);
    
    lua_pushstring(L, "isObjectExists");
    lua_pushcfunction(L, objectmanager_is_object_exists);
    lua_settable(L, -3);
    
    lua_pushstring(L, "deleteObject");
    lua_pushcfunction(L, objectmanager_delete_object);
    lua_settable(L, -3);
    
    lua_pushstring(L, "deleteObjects");
    lua_pushcfunction(L, objectmanager_delete_objects);
    lua_settable(L, -3);
    
    lua_pushstring(L, "getObjectComponents");
    lua_pushcfunction(L, objectmanager_get_object_components);
    lua_settable(L, -3);

    lua_setglobal(L, "OBJECTMANAGER");

    BREAK_END;
}

}
