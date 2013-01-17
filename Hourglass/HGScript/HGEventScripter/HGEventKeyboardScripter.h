/**
 *  @file    HGEventKeyboardScripter.h
 *  @brief   EventKeyboard script adapter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/17
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

#ifndef HGEVENTKEYBOARDSCRIPTER_H_
#define HGEVENTKEYBOARDSCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{
    
class EventKeyboard;

extern luaL_Reg sEventKeyboardRegs[];

extern EventKeyboard* eventkeyboard_check(lua_State* L, int idx);
extern int eventkeyboard_push(lua_State* L, EventKeyboard* event);

extern void ScriptRegisterEventKeyboard(lua_State* L);
    
}


#endif // HGEVENTKEYBOARDSCRIPTER_H_
