/**
 *  @file    HGTranslateAnimationScripter.h
 *  @brief   Translate Animation Script adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/13
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

#ifndef HGTRANSLATEANIMATIONSCRIPTER_H_
#define HGTRANSLATEANIMATIONSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class TranslateAnimation;

extern luaL_Reg sTranslateAnimationRegs[];

extern TranslateAnimation* translateanimation_check(lua_State* L, int idx);
extern int translateanimation_push(lua_State* L, TranslateAnimation* anim);
extern void ScriptRegisterTranslateAnimation(lua_State* L);

HGNAMESPACE_END

#endif // HGTRANSLATEANIMATIONSCRIPTER_H_