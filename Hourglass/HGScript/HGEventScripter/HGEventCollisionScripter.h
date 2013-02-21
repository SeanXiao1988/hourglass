/**
 *  @file    HGEventCollisionScripter.h
 *  @brief   EventCollision script adapter header
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

#ifndef HGEVENTCOLLISIONSCRIPTER_H_
#define HGEVENTCOLLISIONSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class EventCollision;

extern luaL_Reg sEventCollisionRegs[];

extern EventCollision* eventcollision_check(lua_State* L, int idx);
extern int eventcollision_push(lua_State* L, EventCollision* event);

extern void ScriptRegisterEventCollision(lua_State* L);

HGNAMESPACE_END

#endif // HGEVENTCOLLISIONSCRIPTER_H_
