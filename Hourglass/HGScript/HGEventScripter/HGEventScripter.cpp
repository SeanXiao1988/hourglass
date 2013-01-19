/**
 *  @file    HGEventScripter.cpp
 *  @brief   Event script adapter impletementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/08
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

#include "HGEvent.h"
#include "HGEventScripter.h"
#include "HGEventKeyboardScripter.h"
#include "HGEventMouseScripter.h"

#define EVENT_LUA_NAME  "Event"

HGNAMESPACE_START
    
Event* event_check(lua_State* L, int idx)
{
    Event* event = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    event = *static_cast<Event **>(lua_touserdata(L, idx));
    
    BREAK_END;
    
    return event;
}
    
int event_push(lua_State* L, const Event* event)
{
    int ret = 0;
    
    BREAK_START;
    
    if (event == NULL)
        break;
    
    switch (event->eventID)
    {
        case EVENT_KEYBOARD:
            ret = eventkeyboard_push(L, (EventKeyboard *)event);
            break;
        
        case EVENT_MOUSE:
            ret = eventmouse_push(L, (EventMouse *)event);
            break;
            
        default:
            break;
    }
    
    BREAK_END;
    
    return ret;
}
    
static int event_event_id(lua_State* L)
{
    EventID eid = EVENT_WILDCARD;
    
    BREAK_START;
    
    Event *event = event_check(L, 1);
    if (event == NULL)
        break;
    
    eid = event->eventID;
    
    BREAK_END;
    
    lua_pushinteger(L, (int)eid);
    
    return 1;
}
    
luaL_Reg sEventRegs[] =
{
    { "eventID", event_event_id },
    { NULL, NULL }
};

void ScriptRegisterEvent(lua_State* L)
{
    luaL_newmetatable(L, EVENT_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sEventRegs, 0);
    
    lua_pop(L, 1);
    
    // event type
    lua_pushinteger(L, EVENT_WILDCARD);
    lua_setglobal(L, "EVENT_WILDCARD");
    
    lua_pushinteger(L, EVENT_UPDATE);
    lua_setglobal(L, "EVENT_UPDATE");
    
    lua_pushinteger(L, EVENT_MOUSE);
    lua_setglobal(L, "EVENT_MOUSE");
    
    lua_pushinteger(L, EVENT_KEYBOARD);
    lua_setglobal(L, "EVENT_KEYBOARD");
    
    lua_pushinteger(L, EVENT_COUNT);
    lua_setglobal(L, "EVENT_COUNT");
    
    // event process results
    lua_pushinteger(L, EVENT_RESULT_IGNORED);
    lua_setglobal(L, "EVENT_RESULT_IGNORED");
    
    lua_pushinteger(L, EVENT_RESULT_PROCESSED);
    lua_setglobal(L, "EVENT_RESULT_PROCESSED");
    
    lua_pushinteger(L, EVENT_RESULT_INTERCEPT);
    lua_setglobal(L, "EVENT_RESULT_INTERCEPT");
    
    ScriptRegisterEventKeyboard(L);
    ScriptRegisterEventMouse(L);
}
    
HGNAMESPACE_END