/**
 *  @file    HGScaleAnimationScripter.h
 *  @brief   Scale animation script adapter header
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

#ifndef HGSCALEANIMATIONSCRIPTER_H_
#define HGSCALEANIMATIONSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START
    
class ScaleAnimation;

extern luaL_Reg sScaleAnimationRegs[];

extern ScaleAnimation* scaleanimation_check(lua_State* L, int idx);
extern int scaleanimation_push(lua_State* L, ScaleAnimation* anim);
extern void ScriptRegisterScaleAnimation(lua_State* L);

HGNAMESPACE_END

#endif // HGSCALEANIMATIONSCRIPTER_H_
