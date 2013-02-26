/**
 *  @file    HGPhysicalObjectScripter.cpp
 *  @brief   PhysicalObject script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/21
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

#include "HGPhysicalObjectScripter.h"
#include "HGPhysicalObject.h"
#include "HGCompositeScripter.h"
#include "HGMapData.h"
#include "HGMapDataScripter.h"

#define PHYSICALOBJECT_METATABLE    "PhysicalObjectMetatable"
#define PHYSICALOBJECT_LUA_NAME     "PhysicalObject"

HGNAMESPACE_START

PhysicalObject* physicalobject_check(lua_State* L, int idx)
{
    return *static_cast<PhysicalObject **>(luaL_checkudata(L, idx, PHYSICALOBJECT_METATABLE));
}

// push a PhysicalObject to Lua
int physicalobject_push(lua_State* L, PhysicalObject* object)
{
    int ret = 0;
    
    if (object != NULL)
    {
        PhysicalObject** udata = static_cast<PhysicalObject **>(lua_newuserdata(L, sizeof(PhysicalObject *)));
        *udata = object;
        luaL_getmetatable(L, PHYSICALOBJECT_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}

// methods
int physicalobject_set_map(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
    {
        MapData* data = mapdata_check(L, 2);
        object->setMap(data);
    }
    
    return 0;
}

int physicalobject_apply_displacement(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
    {
        int x = (int)luaL_checkinteger(L, 2);
        int y = (int)luaL_checkinteger(L, 3);
        object->applyDisplacement(Vector2Di(x, y));
    }
    
    return 0;
}

int physicalobject_perform_duckjump(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
        object->performDuckJump();

    return 0;
}

HGNAMESPACE_END