/**
 *  @file    HGEventManager.cpp
 *  @brief   Event manager implementation
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

#include "HGEventManager.h"

namespace HG
{
    
EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::initialize()
{
    mListenerMap.clear();
}

void EventManager::deInitialize()
{
    ListenerMap::iterator mapIter = mListenerMap.begin();
    for (; mapIter != mListenerMap.end(); ++mapIter)
    {
        delete mapIter->second;
    }
    
    mListenerMap.clear();
}

void EventManager::broadcastEvent(const Event &event)
{
    do
    {
        if (mListenerMap.empty())
            break;
        
        EventID eid = event.getEventID();
        ListenerList *listenerList = NULL;
        ListenerMap::iterator mapIter = mListenerMap.find(eid);
        
        if (mapIter == mListenerMap.end())
            break;
        
        listenerList = mapIter->second;
        if (listenerList->empty())
            break;
        
        ListenerList::iterator listIter;
        for (listIter = listenerList->begin(); listIter != listenerList->end(); ++listIter)
        {
            (*listIter)->handleEvent(event);
        }
        
    } while (0);
}

void EventManager::postEvent(GameObject *listener, const Event &event)
{
    if (listener == NULL)
        return;
    
    listener->handleEvent(event);
}

void EventManager::addListener(GameObject *listener, EventID eventID)
{
    ListenerMap::iterator mapIter = mListenerMap.find(eventID);
 
    // listener list for eventID is empty, need create first
    if (mapIter == mListenerMap.end())
    {
        ListenerList *newList = new ListenerList;
        mListenerMap.insert(ListenerMap::value_type(eventID, newList));
        newList->push_back(listener);
    }
    else
    {
        (mapIter->second)->push_back(listener);
    }
}

void EventManager::removeListener(GameObject *listener)
{
    do
    {
        if (mListenerMap.empty())
            break;
        
        ListenerMap::iterator mapIter = mListenerMap.begin();
        for (; mapIter != mListenerMap.end(); ++mapIter)
        {
            ListenerList *list = mapIter->second;
            if (list == NULL)
                continue;
            
            ListenerList::iterator listIter = list->begin();
            for (; listIter != list->end(); ++listIter)
            {
                if (*listIter == listener)
                {
                    list->erase(listIter);
                    break;
                }
            }
        }
        
    } while (0);
}

void EventManager::removeListenerByEventID(EventID eventID)
{
    if (mListenerMap.empty())
        return;
    
    ListenerMap::iterator mapIter = mListenerMap.find(eventID);
    if (mapIter == mListenerMap.end())
        return;
    
    ListenerList *list = mapIter->second;
    if (list == NULL)
        return;
    
    list->clear();
}

void EventManager::removeAllListeners()
{
    do
    {
        if (mListenerMap.empty())
            break;
        
        ListenerMap::iterator mapIter = mListenerMap.begin();
        for (; mapIter != mListenerMap.end(); ++mapIter)
        {
            ListenerList *list = mapIter->second;
            if (list == NULL)
                continue;
            
            list->clear();
        }
        
    } while (0);
}
    
}