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
#include "HGEventScripter.h"
#include "HGInputListenerScripter.h"
#include "HGScriptManager.h"

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
    EventResult result = EVENT_RESULT_IGNORED;
    lua_State* L = SCRIPTMANAGER.getState();
    
    switch (event.eventID)
    {
        case EVENT_KEYBOARD:
            if (mScriptCallback != "")
            {
                lua_getglobal(L, mScriptCallback.c_str());
                if (lua_isnil(L, 1))
                    break;
                inputlistener_push(L, this);
                event_push(L, &event);
                lua_call(L, 2, 0);
                result = EVENT_RESULT_PROCESSED;
            }
            break;
            
        default:
            break;
    }

    return result;
}

uint32_t InputListener::getComponentName()
{
    return Hash("InputListener");
}

}
