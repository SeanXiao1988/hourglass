/**
 *  @file    HGRotationAnimationScripter.h
 *  @brief   Rotation animation script adapter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/14
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

#ifndef HGROTATIONANIMATIONSCRIPTER_H_
#define HGROTATIONANIMATIONSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START
    
class RotationAnimation;

extern luaL_Reg sRotationAnimationRegs[];

extern RotationAnimation* rotationanimation_check(lua_State* L, int idx);
extern int rotationanimation_push(lua_State* L, RotationAnimation* anim);
extern void ScriptRegisterRotationAnimation(lua_State* L);
    
HGNAMESPACE_END

#endif // HGROTATIONANIMATIONSCRIPTER_H_
