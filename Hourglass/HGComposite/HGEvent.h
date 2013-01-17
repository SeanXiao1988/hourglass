/**
 *  @file    HGEvent.h
 *  @brief   Event class header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/06
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

#ifndef HGEVENT_H_
#define HGEVENT_H_

#include "HGEventDef.h"

namespace HG
{
    
class Event
{
public:
    Event() {}
    virtual ~Event() {}
    
    EventID eventID;
};
    
// input event
class EventKeyboard : public Event
{
public:
    explicit EventKeyboard(int key, int action);
    ~EventKeyboard();
    
    int     key;
    int     action;
};
    
}

#endif // HGEVENT_H_