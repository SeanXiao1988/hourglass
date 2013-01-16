/**
 *  @file    HGMapData.h
 *  @brief   Map Data
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  map storage: 0   1   2   3   ... w-1
 *               w   w+1 w+2 w+3 ... 2w-1
 *
 *  @internal
 *  Created:  2012/10/11
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

#ifndef HGMAPDATA_H_
#define HGMAPDATA_H_

#include "HGSystem.h"
#include "HGMapDef.h"
#include "HGMapData.h"
#include "HGIComponent.h"
#include "HGGraphicsDef.h"

namespace HG
{

class MapData : public IComponent
{
public:
    MapData();
    ~MapData();
    
    static void GenerateTileMask();

    bool        loadFromFile(const char* filename);
    void        clear();
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_MAP_DATA; }
    virtual uint32_t        getComponentName();
    
    // Editor
    bool        createMap(int w, int h);
    bool        saveToFile(const char* filename);
    void        setTile(int gridX, int gridY, TileType type);
    void        setTexture(GLuint tex);
    void        update();
    void        verify();
    int32_t     getWith() { return mMapHeader.width; }
    int32_t     getHeight() { return mMapHeader.height; }
    TileType*   getData() const { return mData; }

    bool        isSolid(int x, int y, int special = 0);
    TileType    getTileType(int x, int y, int special = 0);
    TileType    getTileTypeByGridCoord(int gridX, int gridY);
    void        debug_draw(int x, int y);

    static int  TileMask[Tiles_Count][TILE_SIZE];
    
private:
    MapData(const MapData& other);
    MapData& operator= (const MapData& rhs);

    map_header_t    mMapHeader;
    TileType*       mData;
    GLuint          mDebugTexture;
};

}
#endif // HGMAPDATA_H_
