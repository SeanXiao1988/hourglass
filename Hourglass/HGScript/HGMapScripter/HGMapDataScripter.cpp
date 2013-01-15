/**
 *  @file    HGMapDataScripter.cpp
 *  @brief   Map data script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/15
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

#include "HGMapDataScripter.h"
#include "HGMapData.h"

#define MAPDATA_METATABLE   "MapDataMetatable"
#define MAPDATA_LUA_NAME    "MapData"

namespace HG
{

MapData* mapdata_check(lua_State* L, int idx)
{
    return *static_cast<MapData **>((luaL_checkudata(L, idx, MAPDATA_METATABLE)));
}
    
// Push a MapData to Lua
int mapdata_push(lua_State* L, MapData* data)
{
    int ret = 0;
    
    BREAK_START;
    
    if (data == NULL)
        break;
    
    MapData** udata = static_cast<MapData **>(lua_newuserdata(L, sizeof(MapData *)));
    *udata = data;
    luaL_getmetatable(L, MAPDATA_METATABLE);
    lua_setmetatable(L, -2);
    
    ret = 1;
    
    BREAK_END;
    
    return ret;
}
    
static int mapdata_load_from_file(lua_State* L)
{
    MapData* data = NULL;
    bool loaded = false;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    const char* filename = luaL_checkstring(L, 2);
    if (filename == NULL)
        break;
    
    loaded = data->loadFromFile(filename);
    
    BREAK_END;
    
    lua_pushboolean(L, loaded?1:0);
    
    return 1;
}
    
static int mapdata_save_to_file(lua_State* L)
{
    MapData* data = NULL;
    bool saved = false;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    const char* filename = luaL_checkstring(L, 2);
    if (filename == NULL)
        break;
    
    saved = data->saveToFile(filename);
    
    BREAK_END;
    
    lua_pushboolean(L, saved?1:0);
    
    return 1;
}
    
static int mapdata_new(lua_State* L)
{
    return 0;
}
    
static int mapdata_delete(lua_State* L)
{
    return 0;
}
    
}
