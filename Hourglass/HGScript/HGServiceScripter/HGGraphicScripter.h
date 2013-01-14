/**
 *  @file    HGGraphicScripter.h
 *  @brief   Graphic script adapter
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/01
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

#ifndef HGGRAPHICSCRIPTER_H_
#define HGGRAPHICSCRIPTER_H_

#include "HGScriptDef.h"
#include "HGGraphicsDef.h"

namespace HG
{
    
extern int vertex_push2lua(lua_State* L, const Vertex* v);
extern int quad_push2lua(lua_State* L, const Quad* q);
extern int triple_push2lua(lua_State* L, const Triple* t);
extern int rtarget_push2lua(lua_State* L, const rtarget_t* rt);
    
extern void ScriptRegisterGraphic(lua_State* L);
    
}

#endif // HGGRAPHICSCRIPTER_H_
