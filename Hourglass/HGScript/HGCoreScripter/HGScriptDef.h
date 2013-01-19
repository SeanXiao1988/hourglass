/**
 *  @file    HGScriptDef.h
 *  @brief   Script definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/24
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

#ifndef HGSCRIPTDEF_H_
#define HGSCRIPTDEF_H_

//#define SCRIPT_POINTER_STRING "4ddr355"

#include "lua.hpp"
#include "HGSystem.h"

// http://cfc.kizzx2.com/index.php/binding-c-classes-to-lua-a-step-by-step-example-for-beginners/

HGNAMESPACE_START
    
static void lua_stack_dump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    printf("\n----- STACK DUMP BEGIN -----\n");
    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {
            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;
                
            case LUA_TBOOLEAN:  /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
                
            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;
                
            default:  /* other values */
                printf("%s", lua_typename(L, t));
                break;
                
        }
        printf("  ");  /* put a separator */
    }
    printf("\n----- STACK DUMP END -----\n");  /* end the listing */
}

static int luaH_tofunction(lua_State* L, int idx)
{
    int func = 0;
    if (lua_type(L, idx) == LUA_TFUNCTION)
    {
        lua_pushvalue(L, idx);
        func = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    
    return func;
}
    
HGNAMESPACE_END

#endif // HGSCRIPTDEF_H_
