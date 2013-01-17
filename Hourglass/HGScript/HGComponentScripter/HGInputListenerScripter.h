/**
 *  @file    HGInputListenerScripter.h
 *  @brief   InputListener script adapter header
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

#ifndef HGINPUTLISTENERSCRIPTER_H_
#define HGINPUTLISTENERSCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{
    
class InputListener;

extern luaL_Reg sInputListenerRegs[];

extern InputListener* inputlistener_check(lua_State* L, int idx);
extern int inputlistener_push(lua_State* L, InputListener* listener);
extern void ScriptRegisterInputListener(lua_State* L);
    
}

#endif // HGINPUTLISTENERSCRIPTER_H_
