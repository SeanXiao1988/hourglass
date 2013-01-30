/**
 *  @file    HGInputListenerScripter.cpp
 *  @brief   InputListener script adapter implementation
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

#include "HGInputListenerScripter.h"
#include "HGInputListener.h"
#include "HGInputDef.h"

#include "HGCompositeScripter.h"

#define INPUTLISTENER_METATABLE "InputListenerMetatable"
#define INPUTLISTENER_LUA_NAME  "InputListener"

HGNAMESPACE_START
    
InputListener* inputlistener_check(lua_State* L, int idx)
{
    InputListener* listener = NULL;
    
    if (lua_isuserdata(L, idx))
        listener = *static_cast<InputListener **>(luaL_checkudata(L, idx, INPUTLISTENER_METATABLE));
    
    return listener;
}
    
// push a InputListener to Lua
int inputlistener_push(lua_State* L, InputListener* listener)
{
    int ret = 0;
    
    BREAK_START;
    
    if (listener == NULL)
        break;
    
    InputListener** udata = static_cast<InputListener **>(lua_newuserdata(L, sizeof(InputListener *)));
    *udata = listener;
    luaL_getmetatable(L, INPUTLISTENER_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
static int inputlistener_set_script_callback(lua_State* L)
{
    InputListener* listener = inputlistener_check(L, 1);
    if (listener != NULL)
    {
        int callbackRef = luaH_tofunction(L, 2);
        listener->setScriptCallback(callbackRef);
    }
    
    return 0;
}
    
static int inputlistener_get_script_callback(lua_State* L)
{
    int ret = 0;
    
    InputListener* listener = inputlistener_check(L, 1);
    if (listener != NULL)
    {
        int function = listener->getScriptCallback();
        lua_pushinteger(L, function);
        ret = 1;
    }
    
    return ret;
}
    
static int inputlistener_new(lua_State* L)
{
    int ret = 0;
    
    InputListener* listener = new InputListener;
    if (listener != NULL)
        ret = inputlistener_push(L, listener);
    
    return ret;
}
    
static int inputlistener_delete(lua_State* L)
{
    InputListener* listener = inputlistener_check(L, 1);
    delete listener;
    
    return 0;
}
    
luaL_Reg sInputListenerRegs[] =
{
    { "setScriptCallback", inputlistener_set_script_callback },
    { "getScriptCallback", inputlistener_get_script_callback },
    { NULL, NULL }
};
    
void ScriptRegisterInputListener(lua_State* L)
{
    luaL_newmetatable(L, INPUTLISTENER_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sInputListenerRegs, 0);
    
    lua_pop(L, 1);
    
    // register InputListener class to Lua
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", inputlistener_new);
    luaH_setfunc2table(L, "delete", inputlistener_delete);
    
    lua_setglobal(L, INPUTLISTENER_LUA_NAME);
    
    // constants
    lua_pushinteger(L, INPUT_NONE);
    lua_setglobal(L, "INPUT_NONE");

    lua_pushinteger(L, INPUT_PRESS);
    lua_setglobal(L, "INPUT_PRESS");

    lua_pushinteger(L, INPUT_RELEASE);
    lua_setglobal(L, "INPUT_RELEASE");

    lua_pushinteger(L, INPUT_TRIGGER);
    lua_setglobal(L, "INPUT_TRIGGER");

    lua_pushinteger(L, INPUT_MOUSE_MOVE);
    lua_setglobal(L, "INPUT_MOUSE_MOVE");

    lua_pushinteger(L, INPUT_MOUSE_LEFT_PRESS);
    lua_setglobal(L, "INPUT_MOUSE_LEFT_PRESS");
    
    lua_pushinteger(L, INPUT_MOUSE_LEFT_RELEASE);
    lua_setglobal(L, "INPUT_MOUSE_LEFT_RELEASE");

    lua_pushinteger(L, INPUT_MOUSE_RIGHT_PRESS);
    lua_setglobal(L, "INPUT_MOUSE_RIGHT_PRESS");
    
    lua_pushinteger(L, INPUT_MOUSE_RIGHT_RELEASE);
    lua_setglobal(L, "INPUT_MOUSE_RIGHT_RELEASE");
}

HGNAMESPACE_END