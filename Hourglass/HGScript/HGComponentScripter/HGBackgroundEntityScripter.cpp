/**
 *  @file    HGBackgroundEntityScripter.cpp
 *  @brief   Background entity script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/11
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

#include "HGBackgroundEntityScripter.h"
#include "HGBackgroundEntity.h"
#include "HGGraphicScripter.h"
#include "HGISceneEntityScripter.h"

#include "HGCompositeScripter.h"

#define BACKGROUNDENTITY_METATABLE  "BackgroundEntityMetatable"
#define BACKGROUNDENTITY_LUA_NAME   "BackgroundEntity"

HGNAMESPACE_START

BackgroundEntity* backgroundentity_check(lua_State* L, int idx)
{
    BackgroundEntity* entity = NULL;
    
    if (lua_isuserdata(L, idx))
        entity = *static_cast<BackgroundEntity **>(luaL_checkudata(L, idx, BACKGROUNDENTITY_METATABLE));
    
    return entity;
}

// push a BackgroundEntity to Lua
int backgroundentity_push(lua_State* L, BackgroundEntity* entity)
{
    int ret = 0;
    
    if (entity != NULL)
    {
        BackgroundEntity** udata = static_cast<BackgroundEntity **>(lua_newuserdata(L, sizeof(BackgroundEntity*)));
        *udata = entity;
        luaL_getmetatable(L, BACKGROUNDENTITY_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}

// member methods
static int backgroundentity_set_type(lua_State* L)
{
    BackgroundType type = BACKGROUND_DEFAULT;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        type = (BackgroundType)luaL_checkinteger(L, 2);
        entity->setType(type);
    }
    
    return 0;
}

static int backgroundentity_set_width(lua_State* L)
{
    float w = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        w = (float)luaL_checknumber(L, 2);
        entity->setWidth(w);
    }
    
    return 0;
}

static int backgroundentity_set_height(lua_State* L)
{
    float h = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        h = (float)luaL_checknumber(L, 2);
        entity->setHeight(h);
    }
    
    return 0;
}

static int backgroundentity_set_texture(lua_State* L)
{
    GLuint tex = 0;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        tex = (GLuint)luaL_checkunsigned(L, 2);
        entity->setTexture(tex);
    }
    
    return 0;
}

static int backgroundentity_set_texture_rect(lua_State* L)
{
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        float w = (float)luaL_checknumber(L, 4);
        float h = (float)luaL_checknumber(L, 5);
        entity->setTextureRect(x, y, w, h);
    }
    
    return 0;
}

static int backgroundentity_set_vertex_alpha(lua_State* L)
{
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        int i = -1;
        uint8_t alpha = (uint8_t)luaL_checkunsigned(L, 2);
        if (lua_isnumber(L, 3))
            i = (int)luaL_checkinteger(L, 3);
        
        entity->setVertexAlpha(alpha, i);
    }
    
    return 0;
}

static int backgroundentity_set_scroll_speed_x(lua_State* L)
{
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        float sx = (float)luaL_checknumber(L, 2);
        entity->setScrollSpeedX(sx);
    }
    
    return 0;
}

static int backgroundentity_set_scroll_speed_y(lua_State* L)
{
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
    {
        float sy = (float)luaL_checknumber(L, 2);
        entity->setScrollSpeedY(sy);
    }
    
    return 0;
}

static int backgroundentity_get_width(lua_State* L)
{
    float width = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
        width = entity->getWidth();
    
    lua_pushnumber(L, width);
    
    return 1;
}

static int backgroundentity_get_height(lua_State* L)
{
    float height = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
        height = entity->getHeight();
    
    lua_pushnumber(L, height);
    
    return 1;
}

static int backgroundentity_get_scroll_speed_x(lua_State* L)
{
    float speedX = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
        speedX = entity->getScrollSpeedX();
    
    lua_pushnumber(L, speedX);
    
    return 1;
}

static int backgroundentity_get_scroll_speed_y(lua_State* L)
{
    float speedY = 0.0f;
    
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    if (entity != NULL)
        speedY = entity->getScrollSpeedY();
    
    lua_pushnumber(L, speedY);
    
    return 1;
}

// class methods
static int backgroundentity_new(lua_State* L)
{
    int ret = 0;
    
    BackgroundEntity* entity = new BackgroundEntity();
    if (entity != NULL)
        ret = backgroundentity_push(L, entity);
    
    return ret;
}

static int backgroundentity_delete(lua_State* L)
{
    BackgroundEntity* entity = backgroundentity_check(L, 1);
    delete entity;
    
    return 0;
}

//
luaL_Reg sBackgroundEntityRegs[] =
{
    { "setType",            backgroundentity_set_type },
    { "setWidth",           backgroundentity_set_width },
    { "setHeight",          backgroundentity_set_height },
    { "setTexture",         backgroundentity_set_texture },
    { "setTextureRect",     backgroundentity_set_texture_rect },
    { "setVertexAlpha",     backgroundentity_set_vertex_alpha },
    { "setScrollSpeedX",    backgroundentity_set_scroll_speed_x },
    { "setScrollSpeedY",    backgroundentity_set_scroll_speed_y },
    { "getWidth",           backgroundentity_get_width },
    { "getHeight",          backgroundentity_get_height },
    { "getScrollSpeedX",    backgroundentity_get_scroll_speed_x },
    { "getScrollSpeedY",    backgroundentity_get_scroll_speed_y },
    { NULL, NULL }
};

void ScriptRegisterBackgroundEntity(lua_State* L)
{
    luaL_newmetatable(L, BACKGROUNDENTITY_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sISceneEntityRegs, 0);
    luaL_setfuncs(L, sBackgroundEntityRegs, 0);
    
    lua_pop(L, 1);
    
    // register BackgroundEntity
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", backgroundentity_new);
    luaH_setfunc2table(L, "delete", backgroundentity_delete);
    
    lua_setglobal(L, BACKGROUNDENTITY_LUA_NAME);
    
    lua_pushinteger(L, BACKGROUND_DEFAULT);
    lua_setglobal(L, "BACKGROUND_DEFAULT");
    
    lua_pushinteger(L, BACKGROUND_STATIC);
    lua_setglobal(L, "BACKGROUND_STATIC");
    
    lua_pushinteger(L, BACKGROUND_REPEAT);
    lua_setglobal(L, "BACKGROUND_REPEAT");
    
    lua_pushinteger(L, BACKGROUND_SCROLL);
    lua_setglobal(L, "BACKGROUND_SCROLL");
}

HGNAMESPACE_END
