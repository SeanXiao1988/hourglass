/**
 *  @file    HGQuadEntityScripter.h
 *  @brief   Quad entity lua adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/29
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2012 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGQUADENTITYSCRIPTER_H_
#define HGQUADENTITYSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START
    
class QuadEntity;

extern luaL_Reg sQuadEntityRegs[];
    
extern QuadEntity* quadentity_check(lua_State* L, int idx);
extern int quadentity_push(lua_State* L, QuadEntity* entity);
extern void ScriptRegisterQuadEntity(lua_State* L);
    
HGNAMESPACE_END

#endif // HGQUADENTITYSCRIPTER_H_
