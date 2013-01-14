/**
 *  @file    HGScriptManager.cpp
 *  @brief   Script manager implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/25
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

#include "HGScriptManager.h"

#include "HGApplicationScripter.h"
#include "HGGraphicScripter.h"
#include "HGHashScripter.h"
#include "HGRenderScripter.h"
#include "HGSceneManagerScripter.h"
#include "HGSpriteAnimationCompilerScripter.h"

#include "HGCompositeScripter.h"
#include "HGQuadEntityScripter.h"
#include "HGSceneNodeScripter.h"
#include "HGIAnimationScripter.h"
#include "HGTranslateAnimationScripter.h"

namespace HG
{

ScriptManager::ScriptManager()
    : mState(NULL)
{
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::initialize()
{
    BREAK_START;

    mState = luaL_newstate();
    if (mState == NULL)
        break;

    static const luaL_Reg lualibs[] = 
    {
        { "base", luaopen_base },
        { "io", luaopen_io },
        { NULL, NULL }
    };

    const luaL_Reg* lib = lualibs;
    for (; lib->func != NULL; lib++)
    {
        luaL_requiref(mState, lib->name, lib->func, 1);
        lua_settop(mState, 0);
    }
    
    lua_atpanic(mState, &ScriptManager::scriptAtPanic);
    
    _registerEngine();

    BREAK_END;
}

void ScriptManager::deInitialize()
{
    lua_close(mState);
    mState = NULL;
}
    
void ScriptManager::execScript(const char* filename)
{
    BREAK_START;
    
    if (mState == NULL || filename == NULL)
        break;
    
    int error = luaL_loadfile(mState, filename);
    if(error) // if non-0, then an error
    {
        // the top of the stack should be the error string
        if (!lua_isstring(mState, lua_gettop(mState)))
            break;
        
        // get the top of the stack as the error and pop it off
        const char* str = lua_tostring(mState, lua_gettop(mState));
        lua_pop(mState, 1);
        
        HGLog("Lua Error: %s\n", str);
    }
    else
    {
        // if not an error, then the top of the stack will be the function to call to run the file
        int status = lua_pcall(mState, 0, LUA_MULTRET, 0); // once again, returns non-0 on error, you should probably add a little check
        if (status)
        {
            // the top of the stack should be the error string
            if (!lua_isstring(mState, lua_gettop(mState)))
                break;
            
            // get the top of the stack as the error and pop it off
            const char* str = lua_tostring(mState, lua_gettop(mState));
            lua_pop(mState, 1);
            
            HGLog("Lua Error: %s\n", str);
        }
    }
    
    
    BREAK_END;
}
    
void ScriptManager::engineInvocation(const char* func)
{
    BREAK_START;
    
    if (mState == NULL || func == NULL)
        break;
    
    lua_getglobal(mState, func);
    if (!lua_pcall(mState, 0, 0, 0)) {
        if (!lua_isstring(mState, lua_gettop(mState)))
            break;
        
        const char* str = lua_tostring(mState, lua_gettop(mState));
        lua_pop(mState, 1);
        
        HGLog("Lua Error: %s\n", str);
    }
    
    BREAK_END;
}
    
void ScriptManager::_registerEngine()
{
    // singletons push to lua as simple table
    ScriptRegisterApplication(mState);
    ScriptRegisterGraphic(mState);
    ScriptRegisterHash(mState);
    ScriptRegisterRender(mState);
    ScriptRegisterSceneManager(mState);
    ScriptRegisterSpriteAnimationCompiler(mState);
    
    // register metatable for other classes
    ScriptRegisterComposite(mState);
    ScriptRegisterSceneNode(mState);
    ScriptRegisterQuadEntity(mState);
    ScriptRegisterIAnimation(mState);
    ScriptRegisterTranslateAnimation(mState);
}
    
int ScriptManager::scriptAtPanic(lua_State* L)
{
    BREAK_START;
    
    if (!lua_isstring(L, lua_gettop(L)))
        break;
    
    const char* str = lua_tostring(L, lua_gettop(L));
    lua_pop(L, 1);
    
    HGLog("Lua Error: %s\n", str);
    
    BREAK_END;
    
    return 0;
}
    
}
