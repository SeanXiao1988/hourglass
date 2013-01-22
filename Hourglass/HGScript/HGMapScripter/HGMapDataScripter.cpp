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

HGNAMESPACE_START

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
    
// MapData methods
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
    
static int mapdata_clear(lua_State* L)
{
    MapData* data = NULL;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    data->clear();
    
    BREAK_END;
    
    return 0;
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
    
static int mapdata_set_tile(lua_State* L)
{
    MapData* data = NULL;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    int gridX = (int)luaL_checkinteger(L, 2);
    int gridY = (int)luaL_checkinteger(L, 3);
    int type = (int)luaL_checkinteger(L, 4);
    
    data->setTile(gridX, gridY, (TileType)type);
    
    BREAK_END;
    
    return 0;
}
    
static int mapdata_get_width(lua_State* L)
{
    MapData* data = NULL;
    int width = 0;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    width = data->getWith();
    
    BREAK_END;
    
    lua_pushinteger(L, width);
    
    return 1;
}

static int mapdata_get_height(lua_State* L)
{
    MapData* data = NULL;
    int height = 0;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    height = data->getHeight();
    
    BREAK_END;
    
    lua_pushinteger(L, height);
    
    return 1;
}
    
static int mapdata_is_solid(lua_State* L)
{
    MapData* data = NULL;
    bool solid = true;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int special = 0;
    
    if (lua_isnumber(L, 4))
        special = (int)luaL_checkinteger(L, 4);
    
    solid = data->isSolid(x, y, special);
    
    BREAK_END;
    
    lua_pushboolean(L, solid?1:0);
    
    return 1;
}
    
static int mapdata_get_tile_type(lua_State* L)
{
    MapData* data = NULL;
    TileType type = Tile_Solid;

    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int special = 0;
    
    if (lua_isnumber(L, 4))
        special = (int)luaL_checkinteger(L, 4);
    
    type = data->getTileType(x, y, special);
    
    BREAK_END;
    
    lua_pushinteger(L, (int)type);
    
    return 1;
}
    
static int mapdata_get_tile_type_by_grid_coord(lua_State* L)
{
    MapData* data = NULL;
    TileType type = Tile_Solid;
    
    BREAK_START;
    
    data = mapdata_check(L, 1);
    if (data == NULL)
        break;
    
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);

    type = data->getTileTypeByGridCoord(x, y);
    
    BREAK_END;
    
    lua_pushinteger(L, (int)type);
    
    return 1;
}
    
static int mapdata_generate_tile_mask(lua_State* L)
{
    MapData::GenerateTileMask();
    
    return 0;
}
    
static int mapdata_new(lua_State* L)
{
    int ret = 0;
    
    BREAK_START;
    
    MapData* data = new MapData;
    ret = mapdata_push(L, data);
    
    BREAK_END;
    
    return ret;
}
    
static int mapdata_delete(lua_State* L)
{
    MapData* data = mapdata_check(L, 1);
    delete data;
    
    return 0;
}
    
luaL_Reg sMapDataRegs[] =
    {
        { "loadFromFile",   mapdata_load_from_file },
        { "clear",          mapdata_clear },
        { "saveToFile",     mapdata_save_to_file },
        { "setTile",        mapdata_set_tile },
        { "getWidth",       mapdata_get_width },
        { "getHeight",      mapdata_get_height },
        { "getTileType",    mapdata_get_tile_type },
        { "getTileTypeByCoord", mapdata_get_tile_type_by_grid_coord },
        { NULL, NULL }
    };
    
void ScriptRegisterMapData(lua_State* L)
{
    luaL_newmetatable(L, MAPDATA_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sMapDataRegs, 0);
    
    lua_pop(L, 1);
    
    // Register MapData to Lua
    lua_newtable(L);
    
    luaH_setfunc2table(L, "GenerateTileMask", mapdata_generate_tile_mask);
    luaH_setfunc2table(L, "new", mapdata_new);
    luaH_setfunc2table(L, "delete", mapdata_delete);
    
    lua_setglobal(L, MAPDATA_LUA_NAME);
    
    // constants
    
    lua_pushinteger(L, Tile_None);
    lua_setglobal(L, "Tile_Node");
    
    lua_pushinteger(L, Tile_Platform);
    lua_setglobal(L, "Tile_Platform");
    
    lua_pushinteger(L, Tile_Slope_Left_27_1);
    lua_setglobal(L, "Tile_Slope_Left_27_1");
    
    lua_pushinteger(L, Tile_Slope_Left_27_2);
    lua_setglobal(L, "Tile_Slope_Left_27_2");
    
    lua_pushinteger(L, Tile_Slope_Left_45);
    lua_setglobal(L, "Tile_Slope_Left_45");
    
    lua_pushinteger(L, Tile_Slope_Right_27_1);
    lua_setglobal(L, "Tile_Slope_Right_27_1");
    
    lua_pushinteger(L, Tile_Slope_Right_27_2);
    lua_setglobal(L, "Tile_Slope_Right_27_2");
    
    lua_pushinteger(L, Tile_Slope_Right_45);
    lua_setglobal(L, "Tile_Slope_Right_45");
    
    lua_pushinteger(L, Tile_Solid);
    lua_setglobal(L, "Tile_Solid");
    
    lua_pushinteger(L, Tiles_Count);
    lua_setglobal(L, "Tiles_Count");
}
    
HGNAMESPACE_END