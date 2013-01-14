/**
 *  @file    HGGrahamScan.h
 *  @brief   Graham Scan
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

#include "HGSystem.h"
#include "HGMapConverter.h"

#define HG_RAD2DEG 57.295779513082

namespace HG
{

typedef struct _gs_point_t_
{
    double x;
    double y;
    double angle; // intermediate value
    struct _gs_point_t_ *prev;
    struct _gs_point_t_ *next;
}gs_point_t;
    
class GrahamScan
{
public:
    GrahamScan();
    ~GrahamScan();

    void    initialize(MCPolygon *polygon);
    void    scan();
    void    clear();
    
    gs_point_t* getPoints() { return mFirstPoint; }
    
private:
    void    _scan(gs_point_t *p);
    void    _addPoint(double x, double y, double angle);
    bool    _isConvexPoint(gs_point_t *p);
    double  _findAngle(double x1, double y1, double x2, double y2);

    gs_point_t *mFirstPoint;
};


}
