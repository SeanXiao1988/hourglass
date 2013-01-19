/**
 *  @file    HGMapDataScripter.h
 *  @brief   Map data script adapter
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

#ifndef HGMAPDATASCRIPTER_H_
#define HGMAPDATASCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class MapData;

extern luaL_Reg sMapDataRegs[];

extern MapData* mapdata_check(lua_State* L, int idx);
extern int mapdata_push(lua_State* L, MapData* data);
extern void ScriptRegisterMapData(lua_State* L);

HGNAMESPACE_END

#endif // HGMAPDATASCRIPTER_H_