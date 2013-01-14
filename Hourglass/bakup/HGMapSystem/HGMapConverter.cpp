/**
 *  @file    HGMapConverter.cpp
 *  @brief   Map converter implementation
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

#include "HGMapConverter.h"
#include "HGGrahamScan.h"
#include "HGRender.h"

namespace HG
{
    
mc_point_t MapConverter::CenterPoint;
    
bool MapConverter::SortPoints(mc_point_t *p1, mc_point_t *p2)
{
    // http://stackoverflow.com/questions/6989100/sort-points-in-clockwise-order
    /*
    if (p1.x >= 0 && p2.x < 0)
        return true;
     */
    if (p1->x == 0 && p2->x == 0)
        return p1->y > p2->y;
    
    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (p1->x-CenterPoint.x) * (p2->y-CenterPoint.y) - (p2->x - CenterPoint.x) * (p1->y - CenterPoint.y);
    if (det < 0)
        return true;
    if (det > 0)
        return false;
    
    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (p1->x-CenterPoint.x) * (p1->x-CenterPoint.x) + (p1->y-CenterPoint.y) * (p1->y-CenterPoint.y);
    int d2 = (p2->x-CenterPoint.x) * (p2->x-CenterPoint.x) + (p2->y-CenterPoint.y) * (p2->y-CenterPoint.y);
    return d1 > d2;
}

MapConverter::MapConverter()
    : mDebugIndex(0)
    , mCurrentX(0)
    , mCurrentY(0)
{
}

MapConverter::~MapConverter()
{
    clear();
}

void MapConverter::clear()
{
    mDebugIndex = 0;
    mCurrentX = 0;
    mCurrentY = 0;
    
	if (mPolygons.empty())
		return;

	MCPolygonList::iterator polyIter = mPolygons.begin();
	for (; polyIter != mPolygons.end(); ++polyIter)
	{
		MCPolygon *pointList = *polyIter;
		if (pointList != NULL)
		{
			MCPolygon::iterator pointIter = pointList->begin();
			for (; pointIter != pointList->end(); ++pointIter)
			{
				delete *pointIter;
			}
		}
		pointList->clear();
		delete pointList;
	}
    mPolygons.clear();
}
        
bool MapConverter::convert(MapData& data)
{
    clear();
    
    int32_t mapWidth = data.getWith();
    int32_t mapHeight = data.getHeight();
    TileType *mapData = data.getData();
    
    BREAK_START;
    
    if (mapData == NULL)
        break;
    
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            uint32_t leftTopX = x * TILE_SIZE;
            uint32_t leftTopY = y * TILE_SIZE;
            
            TileType type = mapData[y*mapWidth + x];
            _addTile(type, leftTopX, leftTopY);
        }
    }
    
    BREAK_END;
    
    return false;
}

// add polygon into polygon list
void MapConverter::_addPolygon(MCPolygon *polygon)
{
    MCPolygonList::iterator polyIter = mPolygons.begin();
    for (; polyIter != mPolygons.end(); ++polyIter)
    {
        MCPolygon *originPolygon = *polyIter;
        
        // check for overlap
        int overlap = 0;
        uint32_t lapX[2];
        uint32_t lapY[2];
        
        MCPolygon::iterator originPoint = originPolygon->begin();
        for (; originPoint != originPolygon->end(); ++originPoint)
        {
            MCPolygon::iterator targetPoint = polygon->begin();
            for (; targetPoint != polygon->end(); ++targetPoint)
            {
                int originX = (*originPoint)->x;
                int originY = (*originPoint)->y;
                int targetX = (*targetPoint)->x;
                int targetY = (*targetPoint)->y;
                if (originX == targetX && originY == targetY && overlap < 2)
                {
                    lapX[overlap] = originX;
                    lapY[overlap] = originY;
                    overlap++;
                }
            }
        }
        // overlap, combine
        if (overlap >= 2)
        {
            // rip off overlap points
            for (int i = 0; i < 2; i++)
            {
                for (MCPolygon::iterator point = polygon->begin(); point != polygon->end(); ++point)
                {
                    if ((*point)->x == lapX[i] && (*point)->y == lapY[i])
                    {
                        delete *point;
                        polygon->erase(point);
                        break;
                    }
                }
            }
            // combine
            for (MCPolygon::iterator point = polygon->begin(); point != polygon->end(); ++point)
            {
                originPolygon->push_back(*point);
            }
            polygon->clear();
            delete polygon;
            //_sortPolygonPoints(originPolygon);
            _grahamScan(originPolygon);
            
            return;
        }
    }
    
    // new polygon
    mPolygons.push_back(polygon);
}

void MapConverter::_grahamScan(MCPolygon *polygon)
{
    if (polygon == NULL || polygon->empty())
        return;
    
    GrahamScan scanner;
    scanner.initialize(polygon);
    scanner.scan();
    
    gs_point_t *result = scanner.getPoints();
    gs_point_t *temp = result;
    if (result == NULL)
        return;
    
    for (MCPolygon::iterator iter = polygon->begin(); iter != polygon->end(); ++iter)
    {
        delete *iter;
    }
    polygon->clear();
    
    do
    {
        mc_point_t *newpoint = new mc_point_t;
        memset(newpoint, 0, sizeof(mc_point_t));
        newpoint->x = (uint32_t)temp->x;
        newpoint->y = (uint32_t)temp->y;
        polygon->push_back(newpoint);
        temp = temp->next;
        
    } while (temp != result);
}
    
void MapConverter::_sortPolygonPoints(MCPolygon *polygon)
{
    if (polygon == NULL)
        return;
    
    CenterPoint.x = 0;
    CenterPoint.y = 0;
    int pointCount = 0;
    MCPolygon::iterator points = polygon->begin();
    for (; points != polygon->end(); ++points)
    {
        CenterPoint.x += (*points)->x;
        CenterPoint.y += (*points)->y;
        pointCount++;
    }
    
    if (pointCount != 0)
    {
        CenterPoint.x = CenterPoint.x / pointCount;
        CenterPoint.y = CenterPoint.y / pointCount;
    }
    
    polygon->sort(&MapConverter::SortPoints);
}

// translate TileType to points info
void MapConverter::_addTile(TileType tile, uint32_t x, uint32_t y)
{
	uint32_t pointCount = 0;
	mc_point_t points[4];
	memset(points, 0, sizeof(mc_point_t) * 4);
    
    switch (tile)
    {
        case Tile_None:
        case Tile_Platform:
            return;
        
        case Tile_Solid:
        {
            pointCount = 4;
			points[0].x = x;
			points[0].y = y;
			points[1].x = x + TILE_SIZE;
			points[1].y = points[0].y;
			points[2].x = points[1].x;
			points[2].y = y + TILE_SIZE;
			points[3].x = points[0].x;
			points[3].y = points[2].y;
			break;
        }
        case Tile_Slope_Left_45:
        {
            pointCount = 3;
            points[0].x = x;
            points[0].y = y;
            points[1].x = x + TILE_SIZE;
            points[1].y = y + TILE_SIZE;
            points[2].x = points[0].x;
            points[2].y = points[1].y;
            break;
        }
        case Tile_Slope_Left_27_1:
        {
            pointCount = 4;
			points[0].x = x;
			points[0].y = y;
			points[1].x = x + TILE_SIZE;
			points[1].y = y + TILE_SIZE/2;
			points[2].x = points[1].x;
			points[2].y = y + TILE_SIZE;
			points[3].x = points[0].x;
			points[3].y = points[2].y;
            break;
        }
        case Tile_Slope_Left_27_2:
        {
            pointCount = 3;
            points[0].x = x;
            points[0].y = y + TILE_SIZE/2;
            points[1].x = x + TILE_SIZE;
            points[1].y = y + TILE_SIZE;
            points[2].x = points[0].x;
            points[2].y = points[1].y;
            break;
        }
        case Tile_Slope_Right_45:
        {
            pointCount = 3;
            points[0].x = x + TILE_SIZE;
            points[0].y = y;
            points[1].x = points[0].x;
            points[1].y = y + TILE_SIZE;
            points[2].x = x;
            points[2].y = points[1].y;
            break;
        }
        case Tile_Slope_Right_27_1:
        {
            pointCount = 3;
            points[0].x = x + TILE_SIZE;
            points[0].y = y + TILE_SIZE/2;
            points[1].x = points[0].x;
            points[1].y = y + TILE_SIZE;
            points[2].x = x;
            points[2].y = points[1].y;
            break;
        }
        case Tile_Slope_Right_27_2:
        {
            pointCount = 4;
            points[0].x = x;
            points[0].y = y + TILE_SIZE/2;
            points[1].x = x + TILE_SIZE;
            points[1].y = y;
            points[2].x = points[1].x;
            points[2].y = y + TILE_SIZE;
            points[3].x = points[0].x;
            points[3].y = points[2].y;
            break;
        }
            
        default:
            return;
    }

    
    MCPolygon *polygon = new MCPolygon;
    for (int i = 0; i < pointCount; i++)
    {
        mc_point_t *point = new mc_point_t;
        point->x = points[i].x;
        point->y = points[i].y;
        polygon->push_front(point);
    }
    
    _addPolygon(polygon);
}
    
void MapConverter::debug_convert(MapData& data)
{
    int32_t mapWidth = data.getWith();
    int32_t mapHeight = data.getHeight();
    TileType *mapData = data.getData();
    
    if (mapData == NULL)
        return;
    
    if (mDebugIndex < mapWidth * mapHeight)
    {
        uint32_t gridY = mDebugIndex / mapWidth;
        uint32_t gridX = mDebugIndex - gridY * mapWidth;
        uint32_t leftTopX = gridX * TILE_SIZE;
        uint32_t leftTopY = gridY * TILE_SIZE;
        
        TileType type = mapData[mDebugIndex];
        _addTile(type, leftTopX, leftTopY);
        
        mDebugIndex++;
        mCurrentX = leftTopX;
        mCurrentY = leftTopY;
    }
}
    
void MapConverter::debug_draw()
{
    Quad quad;
    memset(&quad, 0, sizeof(Quad));
    quad.blend = BLEND_DEFAULT;
    quad.tex = NULL;
    quad.v[0].x = mCurrentX;
    quad.v[0].y = mCurrentY;
    quad.v[1].x = mCurrentX + TILE_SIZE;
    quad.v[1].y = mCurrentY;
    quad.v[2].x = mCurrentX + TILE_SIZE;
    quad.v[2].y = mCurrentY + TILE_SIZE;
    quad.v[3].x = mCurrentX;
    quad.v[3].y = mCurrentY + TILE_SIZE;
    RENDER.setVertexColor(&quad.v[0], 0xFF808080);
    RENDER.setVertexColor(&quad.v[1], 0xFF808080);
    RENDER.setVertexColor(&quad.v[2], 0xFF808080);
    RENDER.setVertexColor(&quad.v[3], 0xFF808080);
    RENDER.renderQuad(&quad);
    
    if (mPolygons.empty())
		return;
    
    int colIndex = 0;
    uint32_t colors[4];
    colors[0] = 0xFF0000FF;
    colors[1] = 0x0000FFFF;
    colors[2] = 0x000000FF;
    colors[3] = 0x00FF00FF;
    
	MCPolygonList::iterator polyIter = mPolygons.begin();
	for (; polyIter != mPolygons.end(); ++polyIter)
	{
        float firstX = -1.0f;
        float firstY = -1.0f;
        float lastX = -1.0f;
        float lastY = -1.0f;
        
		MCPolygon *pointList = *polyIter;
		if (pointList == NULL)
            continue;
        
        MCPolygon::iterator pointIter = pointList->begin();
        for (; pointIter != pointList->end(); ++pointIter)
        {
            if (firstX == -1.0f)
            {
                firstX = (float)(*pointIter)->x;
                firstY = (float)(*pointIter)->y;
            }
            
            if (lastX != -1.0f)
            {
                RENDER.renderLine(lastX, lastY, (float)(*pointIter)->x, (float)(*pointIter)->y, colors[colIndex%4], 1.0f);
            }
            
            lastX = (float)(*pointIter)->x;
            lastY = (float)(*pointIter)->y;
        }
        RENDER.renderLine(lastX, lastY, firstX, firstY, colors[colIndex%4], 1.0f);
        
        colIndex++;
	}
}

}
