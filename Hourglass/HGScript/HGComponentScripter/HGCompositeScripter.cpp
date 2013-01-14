/**
 *  @file    HGCompositeScripter.cpp
 *  @brief   Composite script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/06
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

#include "HGCompositeScripter.h"
#include "HGComponentDef.h"
#include "HGIComponent.h"

#include "HGQuadEntityScripter.h"
#include "HGSceneNodeScripter.h"

namespace HG
{
    
IComponent* icomponent_check(lua_State* L, int idx)
{
    IComponent* comp = NULL;
    
    BREAK_START;
    
    if (!lua_isuserdata(L, idx))
        break;
    
    comp = *static_cast<IComponent **>(lua_touserdata(L, idx));
    
    BREAK_END;
    
    return comp;
}
    
int icomponent_push(lua_State* L, IComponent* comp)
{
    int ret = 0;
    
    BREAK_START;
    
    if (comp == NULL)
        break;
    
    switch (comp->getComponentTypeID())
    {
        case COMP_QUAD_ENTITY:
            ret = quadentity_push(L, (QuadEntity *)comp);
            break;
        
        case COMP_SCENE_NODE:
            ret = scenenode_push(L, (SceneNode *)comp);
            
        default:
            break;
    }
    
    BREAK_END;
    
    return ret;
}
    
// IComponent methods
static int icomponent_get_component_type_id(lua_State* L)
{
    ComponentTypeID tid = COMP_NONE;
    
    BREAK_START;
    
    IComponent *comp = icomponent_check(L, 1);
    if (comp == NULL)
        break;
    
    tid = comp->getComponentTypeID();
    
    BREAK_END;
    
    lua_pushinteger(L, (int)tid);
    
    return 1;
}
    
static int icomponent_get_component_name(lua_State* L)
{
    uint32_t name = 0;
    
    BREAK_START;
    
    IComponent *comp = icomponent_check(L, 1);
    if (comp == NULL)
        break;
    
    name = comp->getComponentName();
    
    BREAK_END;
    
    lua_pushunsigned(L, name);
    
    return 1;
}
    
static int icomponent_get_object_name(lua_State* L)
{
    uint32_t name = 0;
    
    BREAK_START;
    
    IComponent* comp = icomponent_check(L, 1);
    if (comp == NULL)
        break;
    
    BREAK_END;
    
    lua_pushunsigned(L, name);
    
    return 1;
}
    
luaL_Reg sIComponentRegs[] =
{
    { "getComponentTypeID", icomponent_get_component_type_id },
    { "getComponentName",   icomponent_get_component_name },
    { "getObjectName",      icomponent_get_object_name },
    { NULL, NULL }
};

void ScriptRegisterComposite(lua_State* L)
{
    luaL_newmetatable(L, ICOMPONENT_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    
    lua_pop(L, 1);

    // register constants
    lua_pushinteger(L, COMP_NONE);
    lua_setglobal(L, "COMP_NONE");

    lua_pushinteger(L, COMP_SCENE_NODE);
    lua_setglobal(L, "COMP_SCENE_NODE");

    lua_pushinteger(L, COMP_QUAD_ENTITY);
    lua_setglobal(L, "COMP_QUAD_ENTITY");

    lua_pushinteger(L, COMP_MAP_ENTITY);
    lua_setglobal(L, "COMP_MAP_ENTITY");

    lua_pushinteger(L, COMP_PHYSICAL_OBJECT);
    lua_setglobal(L, "COMP_PHYSICAL_OBJECT");

    lua_pushinteger(L, COMP_INPUT_LISTENER);
    lua_setglobal(L, "COMP_INPUT_LISTENER");
    
    lua_pushinteger(L, COMP_COUNT);
    lua_setglobal(L, "COMP_COUNT");
}

}
