/**
 *  @file    HGPhysicsDef.h
 *  @brief   Physics definitions
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/10
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

#ifndef HGPHYSICSDEF_H_
#define HGPHYSICSDEF_H_

#include "HGSystem.h"

HGNAMESPACE_START
    
typedef enum
{
    MC_STATUS_IN_AIR = 0,
    MC_STATUS_ON_GROUND,
    MC_STATUS_ON_SLOPE
}TileColliderStatus;

// map collider structure
//
// o _ upper collider
//   ^
//   |
// o - middle collider
//   |
//   v
// o - lower collider
// the distance between each point is less
// than a TILE_SIZE
//
typedef struct
{
    Point2i             upper_climber;
    Point2i             lower_climber;
    
    // test if air or some shit
    // this detector must above lower_slope_climber TILE_SIZE height
    Point2i             middle_detector;
    
    TileColliderStatus  status;
}tile_collider_t;

typedef enum
{
    PHYSICAL_MODE_DEFAULT = 0,  // test ground and obj-obj
    PHYSICAL_MODE_FLOAT,        // test obj-obj only
    PHYSICAL_MODE_COUNT
}PhysicalMode;
    
typedef std::vector<Recti> CollisionRects;

HGNAMESPACE_END

#endif // HGPHYSICSDEF_H_
