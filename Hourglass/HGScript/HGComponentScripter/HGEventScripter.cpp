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

#include "HGEventScripter.h"
#include "HGEventDef.h"

namespace HG
{

void ScriptRegisterEvent(lua_State* L)
{
    // event type
    lua_pushinteger(L, EVENT_WILDCARD);
    lua_setglobal(L, "EVENT_WILDCARD");
    
    lua_pushinteger(L, EVENT_UPDATE);
    lua_setglobal(L, "EVENT_UPDATE");
    
    lua_pushinteger(L, EVENT_INPUT);
    lua_setglobal(L, "EVENT_INPUT");
    
    lua_pushinteger(L, EVENT_PLAYER_ATTACK);
    lua_setglobal(L, "EVENT_PLAYER_ATTACK");
    
    lua_pushinteger(L, EVENT_COUNT);
    lua_setglobal(L, "EVENT_COUNT");
    
    // event process results
    lua_pushinteger(L, EVENT_RESULT_IGNORED);
    lua_setglobal(L, "EVENT_RESULT_IGNORED");
    
    lua_pushinteger(L, EVENT_RESULT_PROCESSED);
    lua_setglobal(L, "EVENT_RESULT_PROCESSED");
    
    lua_pushinteger(L, EVENT_RESULT_INTERCEPT);
    lua_setglobal(L, "EVENT_RESULT_INTERCEPT");
}
    
}
