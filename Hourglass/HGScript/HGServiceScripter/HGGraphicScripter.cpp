/**
 *  @file    HGGraphicScripter.cpp
 *  @brief   Graphic lua adapter implementation
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

#include "HGGraphicScripter.h"

HGNAMESPACE_START
    
int vertex_push2lua(lua_State* L, const Vertex* v)
{
    int ret = 0;
    
    BREAK_START;
    
    if (v == NULL)
        break;
    
    lua_newtable(L);
    
    lua_pushstring(L, "x");
    lua_pushnumber(L, v->x);
    lua_settable(L, -3);
    
    lua_pushstring(L, "y");
    lua_pushnumber(L, v->y);
    lua_settable(L, -3);
    
    lua_pushstring(L, "z");
    lua_pushnumber(L, v->z);
    lua_settable(L, -3);
    
    lua_pushstring(L, "u");
    lua_pushnumber(L, v->u);
    lua_settable(L, -3);
    
    lua_pushstring(L, "v");
    lua_pushnumber(L, v->v);
    lua_settable(L, -3);
    
    lua_pushstring(L, "color");
    lua_newtable(L);
    for (int i = 0; i < 4; i++)
    {
        lua_pushunsigned(L, (uint8_t)v->color[i]);
        lua_rawseti(L, -2, i);
    }
    lua_settable(L, -3);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

int quad_push2lua(lua_State* L, const Quad* q)
{
    int ret = 0;
    
    BREAK_START;
    
    if (q == NULL)
        break;
    
    lua_newtable(L);
    
    lua_pushstring(L, "blend");
    lua_pushunsigned(L, q->blend);
    lua_settable(L, -3);
    
    lua_pushstring(L, "tex");
    lua_pushunsigned(L, q->tex);
    lua_settable(L, -3);
    
    lua_pushstring(L, "v");
    lua_newtable(L);
    for (int i = 0; i < 4; i++)
    {
        vertex_push2lua(L, &q->v[i]);
        lua_rawseti(L, -2, i);
    }
    lua_settable(L, -3);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

int triple_push2lua(lua_State* L, const Triple* t)
{
    int ret = 0;
    
    BREAK_START;
    
    BREAK_END;
    
    return ret;
}

int rtarget_push2lua(lua_State* L, const rtarget_t* rt)
{
    int ret = 0;
    
    BREAK_START;
    
    BREAK_END;
    
    return ret;
}

void ScriptRegisterGraphic(lua_State* L)
{
    lua_pushinteger(L, BLEND_COLORADD);
    lua_setglobal(L, "BLEND_COLORADD");
    
    lua_pushinteger(L, BLEND_COLORMUL);
    lua_setglobal(L, "BLEND_COLORMUL");
    
    lua_pushinteger(L, BLEND_ALPHABLEND);
    lua_setglobal(L, "BLEND_ALPHABLEND");
    
    lua_pushinteger(L, BLEND_ALPHAADD);
    lua_setglobal(L, "BLEND_ALPHAADD");
    
    lua_pushinteger(L, BLEND_ZWRITE);
    lua_setglobal(L, "BLEND_ZWRITE");
    
    lua_pushinteger(L, BLEND_NOZWRITE);
    lua_setglobal(L, "BLEND_NOZWRITE");
    
    lua_pushinteger(L, BLEND_DEFAULT);
    lua_setglobal(L, "BLEND_DEFAULT");
    
    lua_pushinteger(L, BLEND_DEFAULT_Z);
    lua_setglobal(L, "BLEND_DEFAULT_Z");
}
    
HGNAMESPACE_END