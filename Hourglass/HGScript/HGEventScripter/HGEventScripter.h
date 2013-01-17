/**
 *  @file    HGEventScripter.h
 *  @brief   Event script adapter
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

#ifndef HGEVENTSCRIPTER_H_
#define HGEVENTSCRIPTER_H_

#include "HGScriptDef.h"

#define EVENT_METATABLE "EventMetatable"

namespace HG
{
    
class Event;

extern luaL_Reg sEventRegs[];

extern Event* event_check(lua_State* L, int idx);
extern int event_push(lua_State* L, Event* event);
    
extern void ScriptRegisterEvent(lua_State* L);
    
}

#endif // HGEVENTSCRIPTER_H_

