/**
 *  @file    HGEventKeyboardScripter.cpp
 *  @brief   EventKeyboard script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/17
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

#include "HGEventKeyboardScripter.h"
#include "HGEvent.h"
#include "HGEventScripter.h"

#define EVENTKEYBOARD_METATABLE "EventKeyboardMetatable"
#define EVENTKEYBOARD_LUA_NAME  "EventKeyboard"

HGNAMESPACE_START
    
EventKeyboard* eventkeyboard_check(lua_State* L, int idx)
{
    EventKeyboard* event = NULL;
    
    if (lua_isuserdata(L, idx))
        event = *static_cast<EventKeyboard **>(luaL_checkudata(L, idx, EVENTKEYBOARD_METATABLE));
    
    return event;
}
    
int eventkeyboard_push(lua_State* L, EventKeyboard* event)
{
    int ret = 0;
    
    BREAK_START;
    
    if (event == NULL)
        break;
    
    EventKeyboard** udata = static_cast<EventKeyboard **>(lua_newuserdata(L, sizeof(EventKeyboard *)));
    *udata = event;
    luaL_getmetatable(L, EVENTKEYBOARD_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
// EventKeyboard methods
static int eventkeyboard_key(lua_State* L)
{
    EventKeyboard* event = NULL;
    int key = 0;
    
    event = eventkeyboard_check(L, 1);
    if (event != NULL)
        key = event->key;
    
    lua_pushinteger(L, key);
    
    return 1;
}

static int eventkeyboard_action(lua_State* L)
{
    EventKeyboard* event = NULL;
    int action = 0;
    
    event = eventkeyboard_check(L, 1);
    if (event != NULL)
        action = event->action;
    
    lua_pushinteger(L, action);
    
    return 1;
}
    
luaL_Reg sEventKeyboardRegs[] =
{
    { "key",    eventkeyboard_key },
    { "action", eventkeyboard_action },
    { NULL, NULL }
};
    
void ScriptRegisterEventKeyboard(lua_State* L)
{
    luaL_newmetatable(L, EVENTKEYBOARD_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sEventRegs, 0);
    luaL_setfuncs(L, sEventKeyboardRegs, 0);
    
    lua_pop(L, 1);
}
    
HGNAMESPACE_END