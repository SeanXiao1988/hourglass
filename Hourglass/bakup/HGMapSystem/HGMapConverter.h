/**
 *  @file    HGMapConverter.h
 *  @brief   Map converter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/21
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

#ifndef HGMAPCONVERTER_H_
#define HGMAPCONVERTER_H_

#include "HGMapData.h"

namespace HG
{

typedef struct _mc_point_t_
{
    uint32_t x;
    uint32_t y;
}mc_point_t;

typedef std::list<mc_point_t *> MCPolygon;

typedef std::list<MCPolygon *> MCPolygonList;

class MapConverter
{
public:
    MapConverter();
    ~MapConverter();

    void            clear();
    bool            convert(MapData& data);
    MCPolygonList*  getPolygon();
    
    static  bool    SortPoints(mc_point_t *p1, mc_point_t *p2);
    static mc_point_t CenterPoint;
    
    void            debug_convert(MapData& data);
    void            debug_draw();

private:
	void			_addTile(TileType tile, uint32_t x, uint32_t y);
    void            _addPolygon(MCPolygon *polygon);
    void            _sortPolygonPoints(MCPolygon *polygon);
    void            _grahamScan(MCPolygon *polygon);
    MCPolygonList	mPolygons;
    int             mDebugIndex;
    uint32_t        mCurrentX;
    uint32_t        mCurrentY;
};

}

#endif // HGMAPCONVERTER_H_
