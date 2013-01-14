/**
 *  @file    HGMapManager.h
 *  @brief   Map manager header
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

#ifndef HGMAPMANAGER_H_
#define HGMAPMANAGER_H_

#include "HGSystem.h"

namespace HG
{

class MapData;
    
typedef std::map<uint32_t, MapData*> MapMap;
    
#define MAPMANAGER MapManager::getInstance()
    
class MapManager
{
public:
    static MapManager& getInstance()
    {
        static MapManager instance;
        return instance;
    }
    
    void        initialize();
    void        deInitialize();
    
    MapData*    loadMap(const char* filename);
	MapData*	getMap(const uint32_t nameHash);

private:
    MapMap      mMaps;
    
private:
    MapManager();
    MapManager(const MapManager& other);
    ~MapManager();
    MapManager& operator= (const MapManager& rhs);
};
    
}


#endif // HGMAPMANAGER_H_
