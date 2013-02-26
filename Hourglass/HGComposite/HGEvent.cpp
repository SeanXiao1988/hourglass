/**
 *  @file    HGEvent.cpp
 *  @brief   Event implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/17
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

#include "HGEvent.h"

HGNAMESPACE_START

EventKeyboard::EventKeyboard(int key, int action)
    : key(key)
    , action(action)
{
    eventID = EVENT_KEYBOARD;
}

// Mouse
EventMouse::EventMouse(int x, int y, int action)
    : x(x)
    , y(y)
    , action(action)
{
    eventID = EVENT_MOUSE;
}

// Collision
EventCollision::EventCollision(void* obj, void* rects, int collisionType)
    : obj(obj)
    , collisionRects(rects)
    , collisionType(collisionType)
{
    eventID = EVENT_COLLISION;
}
    
HGNAMESPACE_END