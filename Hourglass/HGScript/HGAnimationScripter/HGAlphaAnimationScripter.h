/**
 *  @file    HGAlphaAnimationScripter.h
 *  @brief   Alpha animation script adapter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/15
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

#ifndef HGALPHAANIMATIONSCRIPTER_H_
#define HGALPHAANIMATIONSCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{
    
class AlphaAnimation;

extern luaL_Reg sAlphaAnimationRegs[];

extern AlphaAnimation* alphaanimation_check(lua_State* L, int idx);
extern int alphaanimation_push(lua_State* L, AlphaAnimation* anim);
extern void ScriptRegisterAlphaAnimation(lua_State* L);

}

#endif // HGALPHAANIMATIONSCRIPTER_H_
