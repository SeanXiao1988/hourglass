/**
 *  @file    HGISceneEntityScripter.h
 *  @brief   ISceneEntity script adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/06
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

#ifndef HGISCENEENTITYSCRIPTER_H_
#define HGISCENEENTITYSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START
    
class ISceneEntity;
    
extern luaL_Reg sISceneEntityRegs[];

extern ISceneEntity* isceneentity_check(lua_State* L, int idx);
    
extern void ScriptRegisterISceneEntity(lua_State* L);

HGNAMESPACE_END

#endif // HGISCENEENTITYSCRIPTER_H_