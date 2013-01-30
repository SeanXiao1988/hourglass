/**
 *  @file    HGSceneManagerScripter.cpp
 *  @brief   Scene manager lua adapter implementation
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

#include "HGSceneManagerScripter.h"
#include "HGSceneNodeScripter.h"
#include "HGScriptManager.h"
#include "HGSceneManager.h"
#include "HGSceneNode.h"

HGNAMESPACE_START

static int scenemanager_create_node(lua_State* L)
{
    int ret = 0;
    BREAK_START;

    const char* name = luaL_checkstring(L, 1);
    if (name == NULL)
        break;
    
    lua_pop(L, 1);
    
    SceneNode* node = SCENEMANAGER.createNode(name);
    if (node == NULL)
        break;
    
    ret = scenenode_push(L, node);

    BREAK_END;

    return ret;
}
    /*
static int scenemanager_delete_node(lua_State* L)
{
    BREAK_START;
    
    if (lua_islightuserdata(L, 1))
    {
        SceneNode* node = scenenode_check(L, 1);
        SCENEMANAGER.deleteNode(node);
    }
    else if (lua_isstring(L, 1))
    {
        const char* name = luaL_checkstring(L, 1);
        SCENEMANAGER.deleteNode(name);
    }
    
    BREAK_END;
    
    return 0;
}
    */
static int scenemanager_get_node(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    const char* name = luaL_checkstring(L, 1);
    if (name == NULL)
        break;
    
    SceneNode* node = SCENEMANAGER.getNode(name);
    if (node == NULL)
        break;
    
    lua_pop(L, 1);

    ret = scenenode_push(L, node);
    
    BREAK_END;
    
    return ret;
}
    
static int scenemanager_get_root(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    SceneNode* root = SCENEMANAGER.getRoot();
    if (root == NULL)
        break;
    
    ret = scenenode_push(L, root);
    
    BREAK_END;
    
    return ret;
}

void ScriptRegisterSceneManager(lua_State* L)
{
    lua_newtable(L);
    
    luaH_setfunc2table(L, "createNode", scenemanager_create_node);
    luaH_setfunc2table(L, "getNode", scenemanager_get_node);
    luaH_setfunc2table(L, "getRoot", scenemanager_get_root);
    
    lua_setglobal(L, "SCENEMANAGER");
}

HGNAMESPACE_END
