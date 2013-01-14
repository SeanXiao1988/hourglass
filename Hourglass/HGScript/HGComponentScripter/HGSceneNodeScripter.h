/**
 *  @file    HGSceneNodeScripter.h
 *  @brief   Scene node lua adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/26
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

#ifndef HGSCENENODESCRIPTER_H_
#define HGSCENENODESCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{
    
class SceneNode;
    
extern luaL_Reg sSceneNodeRegs[];

extern SceneNode* scenenode_check(lua_State* L, int idx);
extern int scenenode_push(lua_State* L, SceneNode* node);
extern void ScriptRegisterSceneNode(lua_State* L);
    
}

#endif // HGSCENENODESCRIPTER_H_
