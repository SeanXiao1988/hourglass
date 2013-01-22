/**
 *  @file    HGAnimationCompilerScripter.cpp
 *  @brief   Animation compiler lua adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/04
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

#include "HGSpriteAnimationCompiler.h"
#include "HGSpriteAnimationCompilerScripter.h"

HGNAMESPACE_START
    
static int sprite_animationcompiler_clear(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    SPRITEANIMATIONCOMPILER.clear();
    
    BREAK_END;
    
    return ret;
}

static int sprite_animationcompiler_parse_xml(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    if (!lua_isstring(L, 1))
        break;
    
    const char* filename = luaL_checkstring(L, 1);
    sprite_frames_header_t* header = SPRITEANIMATIONCOMPILER.parseXML(filename);
    if (header == NULL)
        break;
    
    lua_pushlightuserdata(L, header);
    ret = 1;
    
    BREAK_END;
    
    return ret;
}

static int sprite_animationcompiler_delete_animation(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    if (!lua_islightuserdata(L, 1))
        break;
    
    sprite_frames_header_t* header = (sprite_frames_header_t *)lua_touserdata(L, 1);
    if (header == NULL)
        break;
    
    delete header;
    
    BREAK_END;
    
    return ret;
}
    
void ScriptRegisterSpriteAnimationCompiler(lua_State* L)
{
    BREAK_START;
    
    if (L == NULL)
        break;
    
    lua_newtable(L);
    
    luaH_setfunc2table(L, "clear", sprite_animationcompiler_clear);
    luaH_setfunc2table(L, "parseXML", sprite_animationcompiler_parse_xml);
    luaH_setfunc2table(L, "deleteAnimation", sprite_animationcompiler_delete_animation);
    
    lua_setglobal(L, "SPRITEANIMATIONCOMPILER");
    
    BREAK_END;

}

HGNAMESPACE_END
