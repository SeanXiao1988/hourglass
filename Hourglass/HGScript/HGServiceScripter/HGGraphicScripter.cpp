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

// color4f_t
int color4f_push(lua_State* L, color4f_t* color)
{
    if (color == NULL)
        return 0;
    
    lua_newtable(L);
    
    lua_pushstring(L, "r");
    lua_pushnumber(L, color->r);
    lua_settable(L, -3);
    
    lua_pushstring(L, "g");
    lua_pushnumber(L, color->g);
    lua_settable(L, -3);
    
    lua_pushstring(L, "b");
    lua_pushnumber(L, color->b);
    lua_settable(L, -3);
    
    lua_pushstring(L, "a");
    lua_pushnumber(L, color->a);
    lua_settable(L, -3);
    
    return 1;
}

color4f_t color4_check(lua_State* L, int idx)
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    
    if (lua_istable(L, idx))
    {
        lua_pushstring(L, "r");
        lua_gettable(L, idx);
        r = luaL_checknumber(L, -1);

        lua_pushstring(L, "g");
        lua_gettable(L, idx);
        g = luaL_checknumber(L, -1);
        
        lua_pushstring(L, "b");
        lua_gettable(L, idx);
        b = luaL_checknumber(L, -1);
        
        lua_pushstring(L, "a");
        lua_gettable(L, idx);
        a = luaL_checknumber(L, -1);
    }
    return color4f(r, g, b, a);
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