/**
 *  @file    HGQuadEntityScripter.cpp
 *  @brief   Quad entity lua adapter implementation
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

#include "HGQuadEntityScripter.h"
#include "HGQuadEntity.h"
#include "HGGraphicScripter.h"
#include "HGISceneEntityScripter.h"

#include "HGCompositeScripter.h"

#define QUADENTITY_METATABLE    "QuadEntityMetatable"
#define QUADENTITY_LUA_NAME     "QuadEntity"

HGNAMESPACE_START
    
QuadEntity* quadentity_check(lua_State* L, int idx)
{
    QuadEntity* entity = NULL;
    
    if (lua_isuserdata(L, idx))
        entity = *static_cast<QuadEntity **>(luaL_checkudata(L, idx, QUADENTITY_METATABLE));
    
    return entity;
}

// push a QuadEntity to Lua
int quadentity_push(lua_State* L, QuadEntity* entity)
{
    int ret = 0;
    
    if (entity != NULL)
    {
        QuadEntity** udata = static_cast<QuadEntity **>(lua_newuserdata(L, sizeof(QuadEntity*)));
        *udata = entity;
        luaL_getmetatable(L, QUADENTITY_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}

static int quadentity_set_texture(lua_State* L)
{
    QuadEntity* entity = NULL;
    GLuint texID = 0;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    texID = luaL_checkunsigned(L, 2);
    
    entity->setTexture(texID);

    BREAK_END;

    return 0;
}

static int quadentity_set_texture_rect(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5))
        break;
    
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    float w = (float)luaL_checknumber(L, 4);
    float h = (float)luaL_checknumber(L, 5);
    entity->setTextureRect(x, y, w, h);
    
    BREAK_END;
    
    return 0;
}

static int quadentity_set_color(lua_State* L)
{
    int i = -1;
    QuadEntity* entity = NULL;

    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    if (lua_isnumber(L, 3))
        i = (int)luaL_checkinteger(L, 3);
    
    uint32_t color = luaL_checkunsigned(L, 2);
    entity->setColor(color, i);

    BREAK_END;

    return 0;
}

static int quadentity_set_vertex_alpha(lua_State* L)
{
    int i = -1;
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    if (lua_isnumber(L, 3))
        i = (int)luaL_checkinteger(L, 3);
    
    uint8_t alpha = (uint8_t)luaL_checkunsigned(L, 2);
    entity->setVertexAlpha(alpha, i);

    BREAK_END;

    return 0;
}

static int quadentity_set_blend_mode(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    int blend = (int)luaL_checkinteger(L, 2);
    entity->setBlendMode(blend);
    
    BREAK_END;
    
    return 0;
}

static int quadentity_set_center_point(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        break;
    
    float cx = (float)luaL_checknumber(L, 2);
    float cy = (float)luaL_checknumber(L, 3);
    entity->setCenterPoint(cx, cy);
    
    BREAK_END;
    
    return 0;
}

static int quadentity_set_width(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    float w = (float)luaL_checknumber(L, 2);
    entity->setWidth(w);
    
    BREAK_END;
    
    return 0;
}

static int quadentity_set_height(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    float h = (float)luaL_checknumber(L, 2);
    entity->setHeight(h);
    
    BREAK_END;
    
    return 0;
}

static int quadentity_get_quad(lua_State* L)
{
    int ret = 0;
    QuadEntity* entity = NULL;

    entity = quadentity_check(L, 1);
    if (entity != NULL)
    {
        Quad* q = entity->getQuad();
        quad_push2lua(L, q);
        ret = 1;
    }
    
    return ret;
}
    
static int quadentity_get_texture(lua_State* L)
{
    GLuint texID = 0;
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        texID = entity->getTexture();
    
    lua_pushunsigned(L, texID);
    
    return 1;
}
    
static int quadentity_get_texture_rect(lua_State* L)
{
    int ret = 4;
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->getTextureRect(&x, &y, &w, &h);
    
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pushnumber(L, w);
    lua_pushnumber(L, h);
    
    return ret;
}

static int quadentity_get_color(lua_State* L)
{
    int vidx = 0;
    uint32_t color;
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
    {
        if (lua_isnumber(L, 2))
            vidx = (int)luaL_checkinteger(L, 2);
        
        color = entity->getColor(vidx);

    }
    
    lua_pushunsigned(L, color);
    
    return 1;
}

static int quadentity_get_vertex_alpha(lua_State* L)
{
    int vidx = 0;
    uint8_t alpha = 0;
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
    {
        if (lua_isnumber(L, 2))
            vidx = (int)luaL_checkinteger(L, 2);
        
        alpha = entity->getVertexAlpha(vidx);
    }
    
    lua_pushunsigned(L, alpha);
    
    return 1;
}

static int quadentity_get_blend_mode(lua_State* L)
{
    int blend = BLEND_DEFAULT;
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        blend = (int)entity->getBlendMode();

    lua_pushinteger(L, blend);
    
    return 1;
}

static int quadentity_get_center_point(lua_State* L)
{
    float x = 0.0f;
    float y = 0.0f;
    
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->getCenterPoint(&x, &y);
        
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    
    return 2;
}

static int quadentity_init_sprite_animation(lua_State* L)
{
    int ret = 0;
    
    return ret;
}

static int quadentity_deinit_sprite_animation(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->deInitSpriteAnimation();
    
    return 0;
}

static int quadentity_play_sprite_animation(lua_State* L)
{
    QuadEntity* entity = NULL;
    bool success = false;
    
    BREAK_START;
    
    entity = quadentity_check(L, 1);
    if (entity == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    uint32_t name = luaL_checkunsigned(L, 2);
    
    success = entity->playSpriteAnimation(name);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int quadentity_set_frame(lua_State* L)
{
    int ret = 0;
    
    return ret;
}

static int quadentity_next_frame(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->nextFrame();
    
    return 0;
}

static int quadentity_prev_frame(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->prevFrame();
    
    return 0;
}

static int quadentity_stop_sprite_animation(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->stopSpriteAnimation();
    
    return 0;
}

static int quadentity_pause_sprite_animation(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->pauseSpriteAnimation();
    
    return 0;
}

static int quadentity_resume_sprite_animation(lua_State* L)
{
    QuadEntity* entity = NULL;
    
    entity = quadentity_check(L, 1);
    if (entity != NULL)
        entity->resumeSpriteAnimation();
    
    return 0;
}

static int quadentity_is_sprite_animating(lua_State* L)
{
    bool isAnimating = false;
    QuadEntity* entity = NULL;

    entity = quadentity_check(L, 1);
    if (entity != NULL)
        isAnimating = entity->isSpriteAnimating();
    
    lua_pushboolean(L, isAnimating?1:0);
    
    return 1;
}

static int quadentity_new(lua_State* L)
{
    int ret = 0;
    
    QuadEntity* entity = new QuadEntity();
    if (entity != NULL)
        ret = quadentity_push(L, entity);
    
    return ret;
}
    
static int quadentity_delete(lua_State* L)
{
    QuadEntity* entity = quadentity_check(L, 1);
    delete entity;
    
    return 0;
}
    
luaL_Reg sQuadEntityRegs[] =
{
    { "setTexture",     quadentity_set_texture },
    { "setTextureRect", quadentity_set_texture_rect },
    { "setColor",       quadentity_set_color },
    { "setVertexAlpha", quadentity_set_vertex_alpha },
    { "setBlendMode",   quadentity_set_blend_mode },
    { "setCenterPoint", quadentity_set_center_point },
    { "setWidth",       quadentity_set_width },
    { "setHeight",      quadentity_set_height },
    { "getQuad",        quadentity_get_quad },
    { "getTexture",     quadentity_get_texture },
    { "getTextureRect", quadentity_get_texture_rect },
    { "getColor",       quadentity_get_color },
    { "getVertexAlpha", quadentity_get_vertex_alpha },
    { "getBlendMode",   quadentity_get_blend_mode },
    { "getCenterPoint", quadentity_get_center_point },
    { "initAnimation",  quadentity_init_sprite_animation },
    { "deInitAnimation",quadentity_deinit_sprite_animation },
    { "playAnimation",  quadentity_play_sprite_animation },
    { "pauseAnimation", quadentity_pause_sprite_animation },
    { "resumeAnimation",quadentity_resume_sprite_animation},
    { "stopAnimation",  quadentity_stop_sprite_animation },
    { "nextFrame",      quadentity_next_frame },
    { "prevFrame",      quadentity_prev_frame },
    { "isAnimating",    quadentity_is_sprite_animating },
    { NULL, NULL }
};
    
void ScriptRegisterQuadEntity(lua_State* L)
{
    luaL_newmetatable(L, QUADENTITY_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sISceneEntityRegs, 0);
    luaL_setfuncs(L, sQuadEntityRegs, 0);

    lua_pop(L, 1);
    
    // register QuadEntity
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", quadentity_new);
    luaH_setfunc2table(L, "delete", quadentity_delete);
    
    lua_setglobal(L, QUADENTITY_LUA_NAME);
}
    
HGNAMESPACE_END