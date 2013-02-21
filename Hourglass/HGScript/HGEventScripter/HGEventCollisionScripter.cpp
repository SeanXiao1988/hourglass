/**
 *  @file    HGEventCollisionScripter.cpp
 *  @brief   EventCollision script adapter implementation
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

#include "HGEventCollisionScripter.h"
#include "HGEvent.h"
#include "HGEventScripter.h"

#define EVENTCOLLISION_METATABLE    "EventCollisionMetatable"
#define EVENTCOLLISION_LUA_NAME     "EventCollision"

HGNAMESPACE_START

EventCollision* eventcollision_check(lua_State* L, int idx)
{
    EventCollision* event = NULL;
    
    if (lua_isuserdata(L, idx))
        event = *static_cast<EventCollision **>(luaL_checkudata(L, idx, EVENTCOLLISION_METATABLE));
    
    return event;
}

int eventcollision_push(lua_State* L, EventCollision* event)
{
    int ret = 0;
    
    BREAK_START;
    
    if (event == NULL)
        break;
    
    EventCollision** udata = static_cast<EventCollision **>(lua_newuserdata(L, sizeof(EventCollision *)));
    *udata = event;
    luaL_getmetatable(L, EVENTCOLLISION_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

// EventCollision methods
static int eventcollision_collision_type(lua_State* L)
{
    EventCollision* event = NULL;
    int collisionType = 0;
    
    event = eventcollision_check(L, 1);
    if (event != NULL)
        collisionType = event->collisionType;
    
    lua_pushinteger(L, collisionType);
    
    return 1;
}

static int eventcollision_physical_obj(lua_State* L)
{
    EventCollision* event = eventcollision_check(L, 1);
    return 0;
}

HGNAMESPACE_END
