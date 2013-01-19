/**
 *  @file    HGEventMouseScripter.h
 *  @brief   Mouse event script adapter header
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

#ifndef HGEVENTMOUSESCRIPTER_H_
#define HGEVENTMOUSESCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class EventMouse;
extern luaL_Reg sEventMouseRegs[];

extern EventMouse* eventmouse_check(lua_State* L, int idx);
extern int eventmouse_push(lua_State* L, EventMouse* event);

extern void ScriptRegisterEventMouse(lua_State* L);

HGNAMESPACE_END

#endif // HGEVENTMOUSESCRIPTER_H_
