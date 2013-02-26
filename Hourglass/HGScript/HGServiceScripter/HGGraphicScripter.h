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
#include "HGGeometry.hpp"

HGNAMESPACE_START
    
extern int vertex_push2lua(lua_State* L, const Vertex* v);
extern int quad_push2lua(lua_State* L, const Quad* q);

extern color4f_t color4_check(lua_State* L, int idx);
extern int color4f_push(lua_State* L, color4f_t* color);

extern Point2f point2f_check(lua_State* L, int idx);
extern int point2f_push(lua_State* L, Point2f* point);

extern Point2i point2i_check(lua_State* L, int idx);
extern int point2i_push(lua_State* L, Point2i* point);

extern Recti recti_check(lua_State* L, int idx);
extern int recti_push(lua_State* L, Recti* rect);
    
extern void ScriptRegisterGraphic(lua_State* L);
    
HGNAMESPACE_END

#endif // HGGRAPHICSCRIPTER_H_
