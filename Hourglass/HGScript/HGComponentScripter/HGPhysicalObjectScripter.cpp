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
#include "HGGraphicScripter.h"

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

int physicalobject_get_map_collider(lua_State* L)
{
    int ret = 0;
    
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
    {
        lua_newtable(L);
        
        tile_collider_t collider = object->getMapCollider();
        
        lua_pushstring(L, "upper_climber");
        point2i_push(L, &collider.upper_climber);
        lua_settable(L, -3);
        
        lua_pushstring(L, "lower_climber");
        point2i_push(L, &collider.lower_climber);
        lua_settable(L, -3);
        
        lua_pushstring(L, "middle_detector");
        point2i_push(L, &collider.middle_detector);
        lua_settable(L, -3);
        
        lua_pushstring(L, "status");
        lua_pushinteger(L, (int)collider.status);
        lua_settable(L, -3);
        
        ret = 1;
    }
    
    return ret;
}

int physicalobject_set_physical_mode(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);

    if (object != NULL)
    {
        PhysicalMode mode = (PhysicalMode)luaL_checkinteger(L, 2);
        object->setPhyscialMode(mode);
    }
    
    return 0;
}

int physicalobject_get_physical_mode(lua_State* L)
{
    PhysicalMode mode = PHYSICAL_MODE_DEFAULT;
    
    PhysicalObject* object = physicalobject_check(L, 1);
    if (object != NULL)
        mode = object->getPhsyicalMode();
    
    lua_pushinteger(L, (int)mode);
    
    return 1;
}

int physicalobject_add_collision_rect(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
    {
        Recti rect = recti_check(L, 2);
        object->addCollisionRect(rect);
    }

    return 0;
}

int physicalobject_remove_collision_rects(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object != NULL)
        object->removeCollisionRects();
    
    return 0;
}

int physicalobject_get_collision_rects(lua_State* L)
{
    int i = 0;
    PhysicalObject* object = physicalobject_check(L, 1);
    
    if (object == NULL)
        return 0;
    
    CollisionRects rects = object->getCollisionRects();
    CollisionRects::iterator iter = rects.begin();
    
    lua_newtable(L);
    for (; iter != rects.end(); ++iter)
    {
        Recti rect = *iter;
        recti_push(L, &rect);
        lua_rawseti(L, -2, i);
        i++;
    }
    lua_settable(L, -3);
    
    return 1;
}

// new, delete
static int physicalobject_new(lua_State* L)
{
    int ret = 0;
    PhysicalObject* object = new PhysicalObject();
    if (object != NULL)
        ret = physicalobject_push(L, object);
    
    return ret;
}

static int physicalobject_delete(lua_State* L)
{
    PhysicalObject* object = physicalobject_check(L, 1);
    delete object;
    
    return 0;
}

luaL_Reg sPhysicalObjectRegs[] =
{
    { "setMap",             	physicalobject_set_map },
    { "applyDisplacement",      physicalobject_apply_displacement },
    { "performDuckJump",        physicalobject_perform_duckjump },
    { "getMapCollider",         physicalobject_get_map_collider },
    { "setPhysicalMode",        physicalobject_set_physical_mode },
    { "getPhysicalMode",        physicalobject_get_physical_mode },
    { "addCollisionRect",       physicalobject_add_collision_rect },
    { "removeCollisionRects",   physicalobject_remove_collision_rects },
    { "getCollisionRects",      physicalobject_get_collision_rects },
    { NULL, NULL }
};

void ScriptRegisterPhysicalObject(lua_State* L)
{
    luaL_newmetatable(L, PHYSICALOBJECT_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sPhysicalObjectRegs, 0);
    
    lua_pop(L, 1);
    
    // register PhysicalObject
    lua_newtable(L);
    luaH_setfunc2table(L, "new", physicalobject_new);
    luaH_setfunc2table(L, "delete", physicalobject_delete);
    
    lua_setglobal(L, PHYSICALOBJECT_LUA_NAME);
    
    // constants
    lua_pushinteger(L, MC_STATUS_IN_AIR);
    lua_setglobal(L, "MC_STATUS_IN_AIR");
    
    lua_pushinteger(L, MC_STATUS_ON_GROUND);
    lua_setglobal(L, "MC_STATUS_ON_GROUND");
    
    lua_pushinteger(L, MC_STATUS_ON_SLOPE);
    lua_setglobal(L, "MC_STATUS_ON_SLOPE");
    
    lua_pushinteger(L, PHYSICAL_MODE_DEFAULT);
    lua_setglobal(L, "PHYSICAL_MODE_DEFAULT");
    
    lua_pushinteger(L, PHYSICAL_MODE_FLOAT);
    lua_setglobal(L, "PHYSICAL_MODE_FLOAT");
}

HGNAMESPACE_END
