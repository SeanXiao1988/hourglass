/**
 *  @file    HGBackgroundEntityScripter.h
 *  @brief   Background entity script adapter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/11
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

#ifndef HGBACKGROUNDENTITYSCRIPTER_H_
#define HGBACKGROUNDENTITYSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class BackgroundEntity;

extern luaL_Reg sBackgroundEntityRegs[];

extern BackgroundEntity* backgroundentity_check(lua_State* L, int idx);
extern int backgroundentity_push(lua_State* L, BackgroundEntity* entity);
extern void ScriptRegisterBackgroundEntity(lua_State* L);

HGNAMESPACE_END

#endif // HGBACKGROUNDENTITYSCRIPTER_H_
