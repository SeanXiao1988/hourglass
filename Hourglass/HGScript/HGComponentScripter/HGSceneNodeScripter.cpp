/**
 *  @file    HGSceneNodeScripter.cpp
 *  @brief   Scene node lua adapter implementation
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

#include "HGSceneNodeScripter.h"
#include "HGSceneNode.h"
#include "HGSceneManager.h"
#include "HGISceneEntity.h"
#include "HGHash.h"

#include "HGISceneEntityScripter.h"
#include "HGIAnimationScripter.h"
#include "HGCompositeScripter.h"

#define SCENENODE_METATABLE "SceneNodeMetatable"
#define SCENENODE_LUA_NAME  "SceneNode"

HGNAMESPACE_START

SceneNode* scenenode_check(lua_State* L, int idx)
{
    return *static_cast<SceneNode **>((luaL_checkudata(L, idx, SCENENODE_METATABLE)));
}
    
// Push a SceneNode to Lua
int scenenode_push(lua_State* L, SceneNode* node)
{
    int ret = 0;
    
    if (node != NULL)
    {
        SceneNode** udata = static_cast<SceneNode **>(lua_newuserdata(L, sizeof(SceneNode*)));
        *udata = node;
        luaL_getmetatable(L, SCENENODE_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}
    
static int scenenode_set_alpha(lua_State* L)
{
    float alpha = 0;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    alpha = (float)luaL_checknumber(L, 2);
    node->setAlpha(alpha);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_x(lua_State* L)
{
    float x = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    x = (float)luaL_checknumber(L, 2);
    node->setX(x);
    
    BREAK_END;
    
    return 0;
}

static int scenenode_set_y(lua_State* L)
{
    float y = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    y = (float)luaL_checknumber(L, 2);
    node->setY(y);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_xy(lua_State* L)
{
	float x = 0.0f;
    float y = 0.0f;
    
	BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        break;
    
    x = (float)luaL_checknumber(L, 2);
    y = (float)luaL_checknumber(L, 3);
    node->setXY(x, y);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_scale_x(lua_State* L)
{
    float sx = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    sx = (float)luaL_checknumber(L, 2);
    node->setScaleX(sx);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_scale_y(lua_State* L)
{
    float sy = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    sy = (float)luaL_checknumber(L, 2);
    node->setScaleY(sy);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_scale_xy(lua_State* L)
{
	float sx = 0.0f;
    float sy = 0.0f;
    
	BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        break;
    
    sx = (float)luaL_checknumber(L, 2);
    sy = (float)luaL_checknumber(L, 3);
    node->setScale(sx, sy);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_rotation(lua_State* L)
{
    float rot = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    rot = (float)luaL_checknumber(L, 2);
    node->setRotation(rot);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_visibility(lua_State* L)
{
    bool visibility = true;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isboolean(L, 2))
        break;
    
	visibility = lua_toboolean(L, 2) == 0 ? false : true;
    
    node->setVisibility(visibility);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_world_x(lua_State* L)
{
    float wx = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    wx = (float)luaL_checknumber(L, 2);
    node->setWorldX(wx);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_world_y(lua_State* L)
{
    float wy = 0.0f;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2))
        break;
    
    wy = (float)luaL_checknumber(L, 2);
    node->setWorldY(wy);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_set_world_xy(lua_State* L)
{
	float wx = 0.0f;
    float wy = 0.0f;
    
	BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        break;
    
    wx = (float)luaL_checknumber(L, 2);
    wy = (float)luaL_checknumber(L, 3);
    node->setWorldXY(wx, wy);
    
    BREAK_END;
    
    return 0;
}

static int scenenode_get_hashname(lua_State* L)
{
    uint32_t hashname = 0;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        hashname = node->getHashName();
    
    lua_pushunsigned(L, hashname);
    
    return 1;
}
     
static int scenenode_get_alpha(lua_State* L)
{
    float alpha = 0;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        alpha = node->getAlpha();
    
    lua_pushnumber(L, alpha);
    
    return 1;
}

static int scenenode_get_x(lua_State* L)
{
    float x = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        x = node->getX();
    
    lua_pushnumber(L, x);
    
    return 1;
}

static int scenenode_get_y(lua_State* L)
{
    float y = 0.0f;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        y = node->getY();
    
    lua_pushnumber(L, y);
    
    return 1;
}

static int scenenode_get_xy(lua_State* L)
{
    float x = 0.0f;
    float y = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
    {
        x = node->getX();
        y = node->getY();
    }
    
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    
    return 2;
}

static int scenenode_get_scale_x(lua_State* L)
{
    float sx = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        sx = node->getScaleX();
    
    lua_pushnumber(L, sx);
    
    return 1;
}

static int scenenode_get_scale_y(lua_State* L)
{
    float sy = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        sy = node->getScaleY();
    
    lua_pushnumber(L, sy);
    
    return 1;
}

static int scenenode_get_scale_xy(lua_State* L)
{
    float sx = 0.0f;
    float sy = 0.0f;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
    {
        sx = node->getScaleX();
        sy = node->getScaleY();
    }
    
    lua_pushnumber(L, sx);
    lua_pushnumber(L, sy);
    
    return 2;
}

static int scenenode_get_rotation(lua_State* L)
{
    float rot = 0.0f;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        rot = node->getRotation();
    
    lua_pushnumber(L, rot);
    
    return 1;
}

static int scenenode_get_visibility(lua_State* L)
{
    bool visibility = false;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        visibility = node->getVisibility();
    
    luaH_pushboolean(L, visibility);
    
    return 1;
}

static int scenenode_get_world_x(lua_State* L)
{
    float wx = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        wx = node->getWorldX();
    
    lua_pushnumber(L, wx);
    
    return 1;
}

static int scenenode_get_world_y(lua_State* L)
{
    float wy = 0.0f;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        wy = node->getWorldY();
    
    lua_pushnumber(L, wy);
    
    return 1;
}

static int scenenode_get_world_xy(lua_State* L)
{
    float wx = 0.0f;
    float wy = 0.0f;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
    {
        wx = node->getWorldX();
        wy = node->getWorldY();
    }
    
    lua_pushnumber(L, wx);
    lua_pushnumber(L, wy);
    
    return 2;
}

static int scenenode_get_layer(lua_State* L)
{
    int layer = 0;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        layer = node->getLayer();
    
    lua_pushinteger(L, layer);
    
    return 1;
}

static int scenenode_increase_layer(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->increaseLayer();
    
    return 0;
}

static int scenenode_decrease_layer(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->decreaseLayer();
    
    return 0;
}

static int scenenode_bring_to_top(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->bringToTop();
    
    return 0;
}

static int scenenode_send_to_bottom(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->sendToBottom();
    
    return 0;
}
    
static int scenenode_get_parent(lua_State* L)
{
    int ret = 0;
    
	BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    SceneNode* parent = node->getParent();
    if (parent == NULL)
        break;
    
    ret = scenenode_push(L, parent);
    
    BREAK_END;
    
    return ret;
}
    
static int scenenode_set_parent(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
    {
        SceneNode* parent = NULL;
        
        if (lua_isstring(L, 2))
        {
            const char* parentName = luaL_checkstring(L, 2);
            parent = SCENEMANAGER.getNode(parentName);
        }
        else
        {
            parent = scenenode_check(L, 2);
        }
        
        node->setParent(parent);
    }
    
    return 0;
}
    
static int scenenode_get_child(lua_State* L)
{
    int ret = 0;

    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
    {
        const char* childName = luaL_checkstring(L, 2);
        
        SceneNode* child = node->getChild(childName);
        
        ret = scenenode_push(L, child);
    }
    
    return ret;
}
    
static int scenenode_add_child(lua_State* L)
{
    int ret = 0;
    
	BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    SceneNode* child = scenenode_check(L, 2);
    if (child == NULL)
        break;
    
    node->addChild(child);
    
    BREAK_END;
    
    return ret;
}

static int scenenode_remove_child(lua_State* L)
{
    uint32_t childHash = 0;
    SceneNode* child = NULL;
    
    BREAK_START;
    
    if (lua_isstring(L, 2))
    {
        const char* childName = luaL_checkstring(L, 1);
        childHash = Hash(childName);
        child = SCENEMANAGER.getNode(childHash);
    }
    else
    {
        child = scenenode_check(L, 1);
    }
    
    if (child == NULL)
        break;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    node->removeChild(child);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_remove_all(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->removeAll();
    
    return 0;
}
    
static int scenenode_attach_entity(lua_State* L)
{    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    ISceneEntity* entity = isceneentity_check(L, 2);
    if (entity == NULL)
        break;
    
    node->attachEntity(entity);
    
    BREAK_END;
    
    return 0;
}
    
static int scenenode_detach_entity(lua_State* L)
{
    SceneNode* node = scenenode_check(L, 1);
    if (node != NULL)
        node->detachEntity();
    
    return 0;
}

static int scenenode_get_entity(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    SceneNode* node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    ISceneEntity* entity = node->getEntity();
    if (entity == NULL)
        break;
    
    ret = icomponent_push(L, static_cast<IComponent *>(entity));
    
    BREAK_END;
    
    return ret;
}
    
    
static int scenenode_add_animation(lua_State* L)
{
    BREAK_START;
    
    SceneNode* node = NULL;
    node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    IAnimation* anim = ianimation_check(L, 2);
    if (anim == NULL)
        break;
    
    uint32_t name = 0;
    if(lua_isnumber(L, 3))
    {
        name = luaL_checkunsigned(L, 3);
    }
    else if (lua_isstring(L, 3))
    {
        const char* animName = luaL_checkstring(L, 3);
        name = Hash(animName);
    }
    
    node->addAnimation(anim, name);
    
    BREAK_END;
    
    return 0;
}

static int scenenode_get_animation(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    SceneNode* node = NULL;
    node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    uint32_t name = 0;
    if (lua_isnumber(L, 2))
    {
        name = luaL_checkunsigned(L, 2);
    }
    else if (lua_isstring(L, 2))
    {
        const char* animName = luaL_checkstring(L, 2);
        name = Hash(animName);
    }
    
    IAnimation* anim = node->getAnimation(name);
    if (anim == NULL)
        break;
    
    ret = ianimation_push(L, anim);
    
    BREAK_END;
    
    return ret;
}

static int scenenode_remove_animation(lua_State* L)
{
    BREAK_START;
    
    SceneNode* node = NULL;
    node = scenenode_check(L, 1);
    if (node == NULL)
        break;
    
    uint32_t name = 0;
    if (lua_isnumber(L, 2))
    {
        name = luaL_checkunsigned(L, 2);
    }
    else if (lua_isstring(L, 2))
    {
        const char* animName = luaL_checkstring(L, 2);
        name = Hash(animName);
    }
    
    node->removeAnimation(name);
    
    BREAK_END;
    
    return 0;
}

static int scenenode_remove_all_animations(lua_State* L)
{
    SceneNode* node = NULL;
    node = scenenode_check(L, 1);
    if (node != NULL)
        node->removeAllAnimations();
    
    return 0;
}
    
luaL_Reg sSceneNodeRegs[] =
{
    { "setAlpha",               scenenode_set_alpha },
    { "setX",                   scenenode_set_x },
    { "setY",                   scenenode_set_y },
    { "setXY",                  scenenode_set_xy },
    { "setScaleX",              scenenode_set_scale_x },
    { "setScaleY",              scenenode_set_scale_y },
    { "setScaleXY",             scenenode_set_scale_xy },
    { "setRotation",            scenenode_set_rotation },
    { "setVisibility",          scenenode_set_visibility },
    { "setWorldX",              scenenode_set_world_x },
    { "setWorldY",              scenenode_set_world_y },
    { "setWorldXY",             scenenode_set_world_xy },
    { "getHashName",            scenenode_get_hashname },
    { "getAlpha",               scenenode_get_alpha },
    { "getX",                   scenenode_get_x },
    { "getY",                   scenenode_get_y },
    { "getXY",                  scenenode_get_xy },
    { "getScaleX",              scenenode_get_scale_x },
    { "getScaleY",              scenenode_get_scale_y },
    { "getScaleXY",             scenenode_get_scale_xy },
    { "getRotation",            scenenode_get_rotation },
    { "getVisibility",  		scenenode_get_visibility },
    { "getWorldX",      		scenenode_get_world_x },
    { "getWorldY",      		scenenode_get_world_y },
    { "getWorldXY",             scenenode_get_world_xy },
    { "getLayer",               scenenode_get_layer },
    { "increaseLayer",          scenenode_increase_layer },
    { "decreaseLayer",          scenenode_decrease_layer },
    { "bringToTop",             scenenode_bring_to_top },
    { "sendToBottom",           scenenode_send_to_bottom },
    { "setParent",              scenenode_set_parent },
    { "getParent",              scenenode_get_parent },
    { "getChild",               scenenode_get_child },
    { "addChild",               scenenode_add_child },
    { "removeChild",            scenenode_remove_child },
    { "removeAll",              scenenode_remove_all },
    { "attachEntity",           scenenode_attach_entity },
    { "detachEntity",           scenenode_detach_entity },
    { "getEntity",              scenenode_get_entity },
    { "addAnimation",           scenenode_add_animation },
    { "getAnimation",           scenenode_get_animation },
    { "removeAnimation",        scenenode_remove_animation },
    { "removeAllAnimations",    scenenode_remove_all_animations },
    { NULL, NULL }
};
        
void ScriptRegisterSceneNode(lua_State* L)
{
    luaL_newmetatable(L, SCENENODE_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sSceneNodeRegs, 0);
    
    lua_pop(L, 1);
}
    
HGNAMESPACE_END