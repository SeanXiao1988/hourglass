/**
 *  @file    HGEventManager.h
 *  @brief   Event manager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/07
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

#ifndef HGEVENTMANAGER_H_
#define HGEVENTMANAGER_H_

#include "HGEvent.h"

namespace HG
{
    
class GameObject;
typedef std::list<GameObject *>ListenerList;
typedef std::map<uint32_t, ListenerList*> ListenerMap;

#define EVENTMANAGER EventManager::getInstance()
    
class EventManager
{
public:
    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }
    
    void initialize();
    void deInitialize();
    
    void broadcastEvent(const Event& event);
    void postEvent(GameObject *listener, const Event& event);
    
    void addListener(GameObject *listener, EventID eventID);
    void removeListener(GameObject *listener);
    void removeListenerByEventID(EventID eventID);
    void removeAllListeners();
    
private:
    ListenerMap mListenerMap;
    
private:
    EventManager();
    ~EventManager();
    EventManager(const EventManager& other);
    EventManager& operator= (const EventManager& rhs);
};
    
}

#endif // HGEVENTMANAGER_H_
