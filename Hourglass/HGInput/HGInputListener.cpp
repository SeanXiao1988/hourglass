/**
 *  @file    HGInputListener.cpp
 *  @brief   InputListener implementation
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

#include "HGInputListener.h"
#include "HGHash.h"
#include "HGObjectManager.h"

namespace HG
{

void InputListener::RegisterComponentType(void)
{
    OBJECTMANAGER.subscribeToEvent(COMP_INPUT_LISTENER, EVENT_KEYBOARD);
}
    
void InputListener::deInitialize()
{
    
}
    
EventResult InputListener::handleEvent(const Event& event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t InputListener::getComponentName()
{
    return Hash("InputListener");
}

}
