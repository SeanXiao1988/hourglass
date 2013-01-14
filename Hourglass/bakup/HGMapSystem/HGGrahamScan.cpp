/**
 *  @file    HGGrahamScan.cpp
 *  @brief   Graham Scan implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/22
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

#include "HGGrahamScan.h"

namespace HG
{

GrahamScan::GrahamScan()
    : mFirstPoint(NULL)
{
}
    
GrahamScan::~GrahamScan()
{
    clear();
}
    
void GrahamScan::initialize(MCPolygon *polygon)
{
    if (polygon == NULL || polygon->empty())
        return;
    
    if (mFirstPoint != NULL)
        clear();
    
    // find minimum y point
    MCPolygon::iterator iter = polygon->begin();
    MCPolygon::iterator minY = polygon->begin();
    for (; iter != polygon->end(); ++iter)
    {
        if ((*iter)->y < (*minY)->y)
            minY = iter;
    }
    
    for (iter = polygon->begin(); iter != polygon->end(); ++iter)
    {
        double angle = _findAngle((*iter)->x, (*iter)->y, (*minY)->x, (*minY)->y);
        _addPoint((*iter)->x, (*iter)->y, angle);
    }
    
    if (mFirstPoint == NULL)
        return;
    
    gs_point_t *tempPoint = mFirstPoint;
    do
    {
        tempPoint = tempPoint->next;
    } while (tempPoint->next != NULL);
    
    tempPoint->next = mFirstPoint;
    mFirstPoint->prev = tempPoint;
}
    
void GrahamScan::clear()
{
    if (mFirstPoint == NULL)
        return;
    
    gs_point_t *temp = mFirstPoint->next;
    while (temp != NULL && temp != mFirstPoint)
    {
        temp = temp->next;
        delete mFirstPoint->next;
        mFirstPoint->next = temp;
    }
    
    delete mFirstPoint;
    mFirstPoint = NULL;
}
    
void GrahamScan::scan()
{
    if (mFirstPoint != NULL && mFirstPoint->next != NULL)
        _scan(mFirstPoint->next);
}
    
void GrahamScan::_scan(gs_point_t *p)
{
    gs_point_t *tempPrev;
    gs_point_t *tempNext;
    
    // the hull is closed, scan done
    if (p == mFirstPoint)
        return;
    
    // concave point, delete
    if ( !_isConvexPoint(p) )
    {
        tempPrev = p->prev;
        tempNext = p->next;
        tempPrev->next = tempNext;
        tempNext->prev = tempPrev;
        
        delete p;
        _scan(tempPrev);
        
    }
    else
        _scan(p->next);
}
    
void GrahamScan::_addPoint(double x, double y, double angle)
{
    gs_point_t *tempPoint;
    gs_point_t *tempPointA;
    gs_point_t *tempPointB;
    gs_point_t *curPoint;
    
    tempPoint = new gs_point_t;
    tempPoint->x = x;
    tempPoint->y = y;
    tempPoint->angle = angle;
    tempPoint->next = NULL;
    tempPoint->prev = NULL;
    
    
    if (mFirstPoint == NULL)
    {
        mFirstPoint = tempPoint;
        return;
    }
    
    // greate angle
    if (mFirstPoint->next == NULL && tempPoint->angle >= mFirstPoint->angle)
    {
        mFirstPoint->next = tempPoint;
        tempPoint->prev = mFirstPoint;
        return;
    }
    
    curPoint = mFirstPoint;
    
    // find a greater angle point
    while (tempPoint->angle >= curPoint->angle && curPoint->next != NULL)
        curPoint = curPoint->next;
    
    // if is first point, add to front
    if (curPoint == mFirstPoint)
    {
        mFirstPoint->prev = tempPoint;
        tempPoint->next = mFirstPoint;
        mFirstPoint = tempPoint;
        return;
    }
    // last node, append
    else if (curPoint->next == NULL && tempPoint->angle >= curPoint->angle)
    {
        curPoint->next = tempPoint;
        tempPoint->prev = curPoint;
        return;
    }
    // intermediate point found, insert
    else
    {
        tempPointA = curPoint->prev;
        tempPointB = curPoint->prev->next;
        tempPoint->next = tempPointB;
        tempPoint->prev = tempPointA;
        tempPoint->prev->next = tempPoint;
        tempPoint->next->prev = tempPoint;
    }
}
    
bool GrahamScan::_isConvexPoint(gs_point_t *p)
{
    double cwAngle  = _findAngle(p->x,p->y,p->prev->x,p->prev->y); // compute clockwise angle
    double ccwAngle = _findAngle(p->x,p->y,p->next->x,p->next->y); //compute counter-clockwise
    double difAngle = 0.0;
    
    bool isConvex = false;
    
    //drawLine(P->prev, P->next, 3);
    
    BREAK_START;
    
        if (cwAngle > ccwAngle)
        {
            difAngle = cwAngle - ccwAngle;
            
            if (difAngle > 180.0)
            {
                isConvex = false;
                break;
            }
            else
            {
                isConvex = true;
                break; // convex
            }
        }
        else if (cwAngle < ccwAngle)
        {
            difAngle = ccwAngle - cwAngle;
            
            if (difAngle > 180.0)
            {
                isConvex = true;
                break;
            }
            else
            {
                isConvex = false;
                break;
            }
        }
        else if (cwAngle == ccwAngle)
        {
            break;
        }
    
    BREAK_END;
    
    return isConvex;
}
    
double GrahamScan::_findAngle(double x1, double y1, double x2, double y2)
{
    double deltaX=(double)(x2 - x1);
    double deltaY=(double)(y2 - y1);
    double angle;
    
    if (deltaX == 0.0 && deltaY == 0.0)
        return 0.0;
    
    angle = atan2(deltaY, deltaX) * HG_RAD2DEG;
    
    if (angle < 0.0)
        angle += 360.0;
    
    return angle;
}

}
