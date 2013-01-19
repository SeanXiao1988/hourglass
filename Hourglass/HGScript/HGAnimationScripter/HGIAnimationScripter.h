/**
 *  @file    HGIAnimationScripter.h
 *  @brief   IAnimation script adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/12
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

#ifndef HGIANIMATIONSCRIPTER_H_
#define HGIANIMATIONSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START
    
class IAnimation;
    
extern luaL_Reg sIAnimationRegs[];

extern IAnimation* ianimation_check(lua_State* L, int idx);
    
extern int ianimation_push(lua_State* L, IAnimation* anim);
    
extern void ScriptRegisterIAnimation(lua_State* L);
    
HGNAMESPACE_END

#endif // HGIANIMATIONSCRIPTER_H_
