/**
 *  @file    HGEventMouseScripter.cpp
 *  @brief   Mouse event script adapter scripter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/19
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

#include "HGEventMouseScripter.h"
#include "HGEvent.h"
#include "HGEventScripter.h"

#define EVENTMOUSE_METATABLE    "EventMouseMetatable"
#define EVENTMOUSE_LUA_NAME     "EventMouse"

HGNAMESPACE_START

EventMouse* eventmouse_check(lua_State* L, int idx)
{
    EventMouse* event = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    event = *static_cast<EventMouse **>(luaL_checkudata(L, idx, EVENTMOUSE_METATABLE));
    
    BREAK_END;
    
    return event;
}

int eventmouse_push(lua_State* L, EventMouse* event)
{
    int ret = 0;
    
    BREAK_START;
    
    if (event == NULL)
        break;
    
    EventMouse** udata = static_cast<EventMouse **>(lua_newuserdata(L, sizeof(EventMouse *)));
    *udata = event;
    luaL_getmetatable(L, EVENTMOUSE_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

// EventMouse methods
static int eventmouse_x(lua_State* L)
{
    EventMouse* event = NULL;
    int x = 0;
    
    BREAK_START;
    
    event = eventmouse_check(L, 1);
    if (event == NULL)
        break;
    
    x = event->x;
    
    BREAK_END;
    
    lua_pushinteger(L, x);
    
    return 1;
}

static int eventmouse_y(lua_State* L)
{
    EventMouse* event = NULL;
    int y = 0;
    
    BREAK_START;
    
    event = eventmouse_check(L, 1);
    if (event == NULL)
        break;
    
    y = event->y;
    
    BREAK_END;
    
    lua_pushinteger(L, y);
    
    return 1;
}

static int eventmouse_action(lua_State* L)
{
    EventMouse* event = NULL;
    int action = 0;
    
    BREAK_START;
    
    event = eventmouse_check(L, 1);
    if (event == NULL)
        break;
    
    action = event->action;
    
    BREAK_END;
    
    lua_pushinteger(L, action);
    
    return 1;
}

luaL_Reg sEventMouseRegs[] =
{
    { "x",  	eventmouse_x },
    { "y",      eventmouse_y },
    { "action", eventmouse_action },
    { NULL, NULL }
};

void ScriptRegisterEventMouse(lua_State* L)
{
    luaL_newmetatable(L, EVENTMOUSE_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sEventRegs, 0);
    luaL_setfuncs(L, sEventMouseRegs, 0);
    
    lua_pop(L, 1);
}

HGNAMESPACE_END