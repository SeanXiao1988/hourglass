/**
 *  @file    HGMapManager.cpp
 *  @brief   Map manager implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/19
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

#include "HGMapManager.h"
#include "HGMapData.h"
#include "HGHash.h"

HGNAMESPACE_START
    
MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}
    
void MapManager::initialize()
{
    
}
    
void MapManager::deInitialize()
{
    for (MapMap::iterator iter = mMaps.begin(); iter != mMaps.end(); ++iter)
    {
        delete iter->second;
    }
    
    mMaps.clear();
}
    
MapData* MapManager::loadMap(const char *filename)
{
    MapData *ret = NULL;
    
    BREAK_START;
    
    if (filename == NULL)
        break;
    
    uint32_t hashName = Hash(filename);
    MapMap::iterator query = mMaps.find(hashName);
    if (query != mMaps.end())
    {
        ret = query->second;
        break;
    }
    
    ret = new MapData();
    if (!ret->loadFromFile(filename))
    {
        delete ret;
        ret = NULL;
        break;
    }
    
    mMaps.insert(MapMap::value_type(hashName, ret));
    
    BREAK_END;
    
    return ret;
}

MapData* MapManager::getMap(const uint32_t nameHash)
{
	MapData* ret = NULL;

	BREAK_START;

	MapMap::iterator iter = mMaps.find(nameHash);
	if (iter == mMaps.end())
		break;

	ret = iter->second;

	BREAK_END;

	return ret;
}
    
HGNAMESPACE_END