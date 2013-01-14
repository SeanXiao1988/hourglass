/**
 *  @file    HGSceneManagerScripter.h
 *  @brief   Scene manager lua adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/25
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

#ifndef HGSCENEMANAGERSCRIPTER_H_
#define HGSCENEMANAGERSCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{

extern void ScriptRegisterSceneManager(lua_State* L);

}

#endif // HGSCENEMANAGERSCRIPTER_H_
