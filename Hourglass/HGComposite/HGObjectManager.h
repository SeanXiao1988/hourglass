/**
 *  @file    HGObjectManager.h
 *  @brief   Object manager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/14
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

#ifndef HGOBJECTMANAGER_H_
#define HGOBJECTMANAGER_H_

#include "HGEvent.h"
#include "HGIComponent.h"

#define OBJECTMANAGER ObjectManager::getInstance()

namespace HG
{

class Data;
    
// Object
typedef std::map<ComponentTypeID, IComponent*>  ComponentsMap;
typedef std::map<uint32_t, ComponentsMap*>      ObjectMap;
    
// Event
typedef std::set<ComponentTypeID>               EventComponentSet;
    
class ObjectManager
{
public:
    static ObjectManager& getInstance()
    {
        static ObjectManager instance;
        return instance;
    }
    
    void            initialize();
    void            deInitialize();
    
    bool            isObjectExists(uint32_t name);
    bool            deleteObject(uint32_t name);
	void			deleteObjects();
    
    ComponentsMap*  getObjectComponents(uint32_t name);
    IComponent*     queryComponent(uint32_t name, ComponentTypeID ctid);
    
    // component
    void            registerBuiltinComponents();
    ComponentTypeID getComponentTypeByName(uint32_t name);
    bool            addComponentToObject(uint32_t objectName, IComponent* comp);
    
    // event
    void            subscribeToEvent(ComponentTypeID ctid, EventID etid);
    void            postEvent(uint32_t objName, const Event& event);
    void            broadcastEvent(const Event& event);
    
private:
    ObjectMap               mObjects;
    EventComponentSet       mEventComponentSet[EVENT_COUNT];
    
private:
    ObjectManager();
    ~ObjectManager();
    ObjectManager(const ObjectManager& other);
    ObjectManager& operator= (const ObjectManager& rhs);
};

}

#endif // HGOBJECTMANAGER_H_
