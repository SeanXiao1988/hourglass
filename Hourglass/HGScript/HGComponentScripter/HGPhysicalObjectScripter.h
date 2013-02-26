/**
 *  @file    HGPhysicalObjectScripter.h
 *  @brief   PhysicalObject script adapter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/21
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

#ifndef HGPHYSICALOBJECTSCRIPTER_H_
#define HGPHYSICALOBJECTSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class PhysicalObject;

extern luaL_Reg sPhysicalObjectRegs[];

extern PhysicalObject* physicalobject_check(lua_State* L, int idx);
extern int physicalobject_push(lua_State* L, PhysicalObject* object);
extern void ScriptRegisterPhysicalObject(lua_State* L);

HGNAMESPACE_END

#endif // HGPHYSICALOBJECTSCRIPTER_H_