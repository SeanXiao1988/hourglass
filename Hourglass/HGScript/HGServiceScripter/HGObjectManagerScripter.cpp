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

HGNAMESPACE_START

static int objectmanager_is_object_exists(lua_State* L)
{
    int ret = 1;
    uint32_t objectNameHash = 0;
    bool exist = false;
    
    BREAK_START;
    
    if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    else if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
    }
    
    exist = OBJECTMANAGER.isObjectExists(objectNameHash);

    BREAK_END;
    
    lua_pushboolean(L, exist?1:0);

    return ret;
}
    
static int objectmanager_delete_object(lua_State* L)
{
    uint32_t objectNameHash = 0;
    
    if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    else if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
    }
    
    OBJECTMANAGER.deleteObject(objectNameHash);
    
    return 0;
}
    
static int objectmanager_delete_objects(lua_State* L)
{
    OBJECTMANAGER.deleteObjects();
    
    return 0;
}
    
static int objectmanager_get_object_components(lua_State* L)
{
    int ret = 0;
    uint32_t objectNameHash = 0;
    
    BREAK_START;
    
    if (lua_isnumber(L, 1))
    {
        objectNameHash = luaL_checkunsigned(L, 1);
    }
    else if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        objectNameHash = Hash(objectName);
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
    
static int objectmanager_query_component(lua_State* L)
{
    int ret = 0;
    uint32_t name = 0;
    ComponentTypeID tid = COMP_NONE;
    
    BREAK_START;

    if (lua_isnumber(L, 1))
    {
        name = luaL_checkunsigned(L, 1);
    }
    else if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        name = Hash(objectName);
    }
    
    tid = (ComponentTypeID)luaL_checkinteger(L, 2);
    
    IComponent* comp = OBJECTMANAGER.queryComponent(name, tid);
    
    ret = icomponent_push(L, comp);
    
    BREAK_END;
    
    return ret;
}
    
static int objectmanager_add_component_to_object(lua_State* L)
{
    bool added = false;
    uint32_t name = 0;
    IComponent* comp = NULL;
    
    BREAK_START;
    
    if (lua_isnumber(L, 1))
    {
        name = luaL_checkunsigned(L, 1);
    }
    else if (lua_isstring(L, 1))
    {
        const char* objectName = luaL_checkstring(L, 1);
        name = Hash(objectName);
    }
    
    comp = icomponent_check(L, 2);
    if (comp == NULL)
        break;
    
    added = OBJECTMANAGER.addComponentToObject(name, comp);
    
    BREAK_END;
    
    lua_pushboolean(L, added?1:0);
    
    return 1;
}

void ScriptRegisterObjectManager(lua_State* L)
{
    BREAK_START;

    if (L == NULL)
        break;

    lua_newtable(L);
    
    luaH_setfunc2table(L, "isObjectExists", objectmanager_is_object_exists);
    luaH_setfunc2table(L, "deleteObject", objectmanager_delete_object);
    luaH_setfunc2table(L, "deleteObjects", objectmanager_delete_objects);
    luaH_setfunc2table(L, "getObjectComponents", objectmanager_get_object_components);
    luaH_setfunc2table(L, "queryComponent", objectmanager_query_component);
    luaH_setfunc2table(L, "addComponentToObject", objectmanager_add_component_to_object);

    lua_setglobal(L, "OBJECTMANAGER");

    BREAK_END;
}
    
HGNAMESPACE_END
