/**
 *  @file    HGApplicationScripter.h
 *  @brief   Application script adapter
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

#ifndef HGAPPLICATIONSCRIPTER_H_
#define HGAPPLICATIONSCRIPTER_H_

#include "HGScriptDef.h"

namespace HG
{

extern void ScriptRegisterApplication(lua_State* L);
    
}

#endif // HGAPPLICATIONSCRIPTER_H_
