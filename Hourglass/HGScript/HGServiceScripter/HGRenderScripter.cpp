/**
 *  @file    HGRenderScripter.cpp
 *  @brief   Render lua adapter implementation
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

#include "HGRenderScripter.h"
#include "HGGraphicScripter.h"
#include "HGRender.h"
#include "HGHash.h"

#define RENDER_LUA_NAME     "RENDER"

HGNAMESPACE_START
    
static int render_set_clear_color(lua_State* L)
{
    uint32_t color = 0x000000FF;
    
    if (lua_isnumber(L, 1))
    {
        color = luaL_checkunsigned(L, 1);
        RENDER.setClearColor(color);
    }

    return 0;
}

static int render_texture_create(lua_State* L)
{
    uint32_t texID = 0;

    if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
    {
        int w = luaL_checkunsigned(L, 1);
        int h = luaL_checkunsigned(L, 2);
        
        texID = RENDER.textureCreate(w, h);
    }
    
    lua_pushunsigned(L, texID);

    return 1;
}

static int render_texture_free(lua_State* L)
{
    if (lua_isnumber(L, 1))
    {
        uint32_t texID = luaL_checkunsigned(L, 1);
        RENDER.textureFree(texID);
    }

    return 0;
}

static int render_texture_get_width(lua_State* L)
{
    int w = 0;
    
    if (lua_isnumber(L, 1))
    {
        uint32_t texID = luaL_checkunsigned(L, 1);
        w = RENDER.textureGetWidth(texID);
    }
    
    lua_pushinteger(L, w);

    return 1;
}

static int render_texture_get_height(lua_State* L)
{
    int h = 0;
    
    if (lua_isnumber(L, 1))
    {
        uint32_t texID = luaL_checkunsigned(L, 1);
        h = RENDER.textureGetHeight(texID);
    }
    
    lua_pushinteger(L, h);
    
    return 1;
}

static int render_texture_lock(lua_State* L)
{
    if (lua_isnumber(L, 1))
    {
        uint32_t texID = luaL_checkunsigned(L, 1);
        RENDER.textureLock(texID);
    }
    
    return 0;
}

static int render_texture_unlock(lua_State* L)
{
    if (lua_isnumber(L, 1))
    {
        uint32_t texID = luaL_checkunsigned(L, 1);
        RENDER.textureUnlock(texID);
    }
    
    return 0;
}

static int render_texture_load(lua_State* L)
{
    uint32_t texID = 0;
    
    BREAK_START;
    
    if (lua_isnumber(L, 1))
    {
        uint32_t nameHash = luaL_checkunsigned(L, 1);
        texID = RENDER.textureLoad(nameHash);
        break;
    }
    else if (lua_isstring(L, 1))
    {
        const char* filename = luaL_checkstring(L, 1);
        texID = RENDER.textureLoad(filename);
    }

    BREAK_END;

    lua_pushunsigned(L, texID);
    
    return 1;
}

static int render_texture_free_all(lua_State* L)
{
    RENDER.textureFreeAll();

    return 0;
}


static int render_rtarget_create(lua_State* L)
{
    uint32_t tarID = 0;
    
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
    {
        int w = luaL_checkunsigned(L, 1);
        int h = luaL_checkunsigned(L, 2);
        
        tarID = RENDER.rtargetCreate(w, h);
    }
    
    lua_pushunsigned(L, tarID);
    
    return 1;
}

static int render_rtarget_free(lua_State* L)
{
    if (lua_isnumber(L, 1))
    {
        uint32_t tarID = luaL_checkunsigned(L, 1);
        RENDER.rtargetFree(tarID);
    }
    
    return 0;
}

static int render_rtarget_get_texture(lua_State* L)
{
    uint32_t texID = 0;
    
    if (lua_isnumber(L, 1))
    {
        uint32_t tarID = luaL_checkunsigned(L, 1);
        texID = RENDER.rtargetGetTexture(tarID);
    }
    
    lua_pushunsigned(L, texID);
    
    return 1;
}

static int render_rtarget_find(lua_State* L)
{
    int ret = 0;

    BREAK_START;

	if (!lua_isnumber(L, 1))
		break;
	
	uint32_t tarID = luaL_checkunsigned(L, 1);
	rtarget_t* rtar = RENDER.rtargetFind(tarID);
	if (rtar == NULL)
		break;

	rtarget_push2lua(L, rtar);
	ret = 1;

    BREAK_END;

    return ret;
}

static int render_rtarget_free_all(lua_State* L)
{
	RENDER.rtargetFreeAll();

    return 0;
}

static int render_shader_load(lua_State* L)
{
	uint32_t shaderID = 0;
	bool active = false;

	if (lua_isstring(L, 1))
    {
        if (lua_isboolean(L, 2))
            active = lua_toboolean(L, 2) == 0 ? false : true;

        const char* filename = luaL_checkstring(L, 1);
        shaderID = RENDER.shaderLoad(filename, active);
    }

	lua_pushunsigned(L, shaderID);

    return 1;
}

static int render_shader_free(lua_State* L)
{
	if (lua_isnumber(L, 1))
    {
        uint32_t shaderID = luaL_checkunsigned(L, 1);
        RENDER.shaderFree(shaderID);
    }

    return 0;
}

static int render_shader_active(lua_State* L)
{
	if (lua_isnumber(L, 1))
    {
        uint32_t shaderID = luaL_checkunsigned(L, 1);
        RENDER.shaderActive(shaderID);
    }
    
    return 0;
}

static int render_shader_deactive(lua_State* L)
{
	RENDER.shaderDeActive();
    
    return 0;
}

static int render_shader_push(lua_State* L)
{
	RENDER.shaderPush();
    
    return 0;
}

static int render_shader_pop(lua_State* L)
{
	RENDER.shaderPop();
    
    return 0;
}

static int render_shader_get_current(lua_State* L)
{
	uint32_t shaderID = 0;

	shaderID = RENDER.shaderGetCurrent();

	lua_pushunsigned(L, shaderID);

    return 0;
}

static int render_draw_line(lua_State* L)
{
    int ret = 0;

    BREAK_START;

	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6))
		break;
	
	float x1 = (float)luaL_checknumber(L, 1);
	float y1 = (float)luaL_checknumber(L, 2);
	float x2 = (float)luaL_checknumber(L, 3);
	float y2 = (float)luaL_checknumber(L, 4);
	uint32_t color = luaL_checkunsigned(L, 5);
	float z = (float)luaL_checknumber(L, 6);

	RENDER.renderLine(x1, y1, x2, y2, color, z);

    BREAK_END;

    return ret;
}

static int render_draw_rect(lua_State* L)
{
	BREAK_START;

	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6))
		break;

	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);
	uint32_t color = luaL_checkunsigned(L, 5);
	float z = (float)luaL_checknumber(L, 6);

	RENDER.renderRect(x, y, w, h, color, z);

	BREAK_END;

	return 0;
}

static int render_draw_box(lua_State* L)
{
    BREAK_START;

	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6))
		break;

	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);
	uint32_t color = luaL_checkunsigned(L, 5);
	float z = (float)luaL_checknumber(L, 6);

	RENDER.renderBox(x, y, w, h, color, z);

    BREAK_END;

    return 0;
}

static int render_draw_quad(lua_State* L)
{
    return 0;
}

static int render_draw_triple(lua_State* L)
{
    return 0;
}

static int render_draw_vertex_list(lua_State* L)
{
    return 0;
}

static int render_draw_text(lua_State* L)
{
    return 0;
}

static int render_set_default_font_size(lua_State* L)
{
	if (lua_isnumber(L, 1))
    {
        uint32_t size = luaL_checkunsigned(L, 1);
        RENDER.setDefaultFontSize(size);
    }

    return 0;
}

static int render_get_fps(lua_State* L)
{
	double fps = 0.0;

	fps = RENDER.getFPS();

	lua_pushnumber(L, fps);

    return 1;
}

static int render_get_width(lua_State* L)
{
	int width = 0;
    
	width = RENDER.getWidth();

	lua_pushinteger(L, width);

    return 1;
}

static int render_get_height(lua_State* L)
{
	int height = 0;

	height = RENDER.getHeight();

	lua_pushinteger(L, height);

    return 0;
}

static int render_is_fullscreen(lua_State* L)
{
	bool isFullScreen = false;

	isFullScreen = RENDER.isFullScreen();

	lua_pushboolean(L, isFullScreen?1:0);

	return 1;
}

void ScriptRegisterRender(lua_State* L)
{
    lua_newtable(L);
    
    luaH_setfunc2table(L, "setClearColor",      render_set_clear_color);
    luaH_setfunc2table(L, "textureCreate",      render_texture_create);
    luaH_setfunc2table(L, "textureFree",        render_texture_free);
    luaH_setfunc2table(L, "textureGetWidth",    render_texture_get_width);
    luaH_setfunc2table(L, "textureGetHeight",   render_texture_get_height);
    luaH_setfunc2table(L, "textureLock",        render_texture_lock);
    luaH_setfunc2table(L, "textureUnlock",      render_texture_unlock);
    luaH_setfunc2table(L, "textureLoad",        render_texture_load);
    luaH_setfunc2table(L, "textureFreeAll",     render_texture_free_all);
    luaH_setfunc2table(L, "rtargetCreate",      render_rtarget_create);
    luaH_setfunc2table(L, "rtargetFree",        render_rtarget_free);
    luaH_setfunc2table(L, "rtargetGetTexture",  render_rtarget_get_texture);
    luaH_setfunc2table(L, "rtargetFind",        render_rtarget_find);
    luaH_setfunc2table(L, "rtargetFreeAll",     render_rtarget_free_all);
    luaH_setfunc2table(L, "shaderLoad",         render_shader_load);
    luaH_setfunc2table(L, "shaderFree",         render_shader_free);
    luaH_setfunc2table(L, "shaderActive",       render_shader_active);
    luaH_setfunc2table(L, "shaderDeactive",     render_shader_deactive);
    luaH_setfunc2table(L, "shaderPush",         render_shader_push);
    luaH_setfunc2table(L, "shaderPop",          render_shader_pop);
    luaH_setfunc2table(L, "shaderGetCurrent",   render_shader_get_current);
    luaH_setfunc2table(L, "drawLine",           render_draw_line);
    luaH_setfunc2table(L, "drawRect",           render_draw_rect);
    luaH_setfunc2table(L, "drawBox",            render_draw_box);
    luaH_setfunc2table(L, "drawQuad",           render_draw_quad);
    luaH_setfunc2table(L, "drawTriple",         render_draw_triple);
    luaH_setfunc2table(L, "drawVertexList",     render_draw_vertex_list);
    luaH_setfunc2table(L, "drawText",           render_draw_text);
    luaH_setfunc2table(L, "setDefaultFontSize", render_set_default_font_size);
    luaH_setfunc2table(L, "getFPS",             render_get_fps);
    luaH_setfunc2table(L, "getWidth",           render_get_width);
    luaH_setfunc2table(L, "getHeight",          render_get_height);
    
    lua_setglobal(L, RENDER_LUA_NAME);
}
    
HGNAMESPACE_END
