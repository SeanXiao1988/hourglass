/**
 *  @file    HGCompositeScripter.h
 *  @brief   Composite script adapter
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

#ifndef HGCOMPOSITESCRIPTER_H_
#define HGCOMPOSITESCRIPTER_H_

#include "HGScriptDef.h"

#define ICOMPONENT_METATABLE "IComponentMetatable"

HGNAMESPACE_START
    
class IComponent;
    
extern luaL_Reg sIComponentRegs[];

extern IComponent* icomponent_check(lua_State* L, int idx);
extern int icomponent_push(lua_State* L, IComponent* comp);
    
extern void ScriptRegisterComposite(lua_State* L);

HGNAMESPACE_END

#endif // HGCOMPOSITESCRIPTER_H_
