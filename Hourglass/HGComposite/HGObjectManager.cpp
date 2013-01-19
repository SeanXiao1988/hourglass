/**
 *  @file    HGObjectManager.cpp
 *  @brief   Object manager implementation
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

#include "HGObjectManager.h"
#include "HGData.h"

#include "HGPhysicalObject.h"
#include "HGInputListener.h"
#include "HGSceneNode.h"
#include "HGQuadEntity.h"
#include "HGMapData.h"

#include "HGHash.h"

HGNAMESPACE_START
    
ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::initialize()
{
	registerBuiltinComponents();
}

void ObjectManager::deInitialize()
{
    deleteObjects();
}

bool ObjectManager::isObjectExists(uint32_t name)
{
    bool ret = false;
    
    BREAK_START;
    
    if (mObjects.empty())
        break;
    
    ObjectMap::iterator iter = mObjects.find(name);
    if (iter == mObjects.end())
        break;
    else
        ret = true;
        
    BREAK_END;
    
    return ret;
}
    
bool ObjectManager::deleteObject(uint32_t name)
{
    bool ret = false;
    
    BREAK_START;
        
        if (!isObjectExists(name))
            break;
        
		ObjectMap::iterator objectIter = mObjects.find(name);
		if (objectIter == mObjects.end())
			break;

		ComponentsMap *compMap = objectIter->second;
		if (compMap == NULL)
			break;

		ComponentsMap::iterator compMapIter = compMap->begin();
		for (; compMapIter != compMap->end(); ++compMapIter)
		{
			IComponent *component = compMapIter->second;
			if (component == NULL)
				continue;
            
            component->deInitialize();
            delete component;
		}
        
		compMap->clear();
		delete compMap;
		mObjects.erase(objectIter);
		ret = true;
        
    BREAK_END;
    
    return ret;
}

void ObjectManager::deleteObjects()
{
	ObjectMap::iterator objectIter = mObjects.begin();
	for (; objectIter != mObjects.end(); ++objectIter)
	{
		ComponentsMap *compMap = objectIter->second;
		if (compMap == NULL)
			continue;

		ComponentsMap::iterator compMapIter = compMap->begin();
		for (; compMapIter != compMap->end(); ++compMapIter)
		{
            IComponent *component = compMapIter->second;
            if (component == NULL)
                continue;
            
            component->deInitialize();
            delete component;
		} // ComponentsMap
		compMap->clear();
		delete compMap;
	} // ObjectMap
	mObjects.clear();
}
    
ComponentsMap* ObjectManager::getObjectComponents(uint32_t name)
{
    ComponentsMap *ret = NULL;
 
    BREAK_START;
    
        if (mObjects.empty())
            break;
        
        ObjectMap::iterator iter = mObjects.find(name);
        if (iter == mObjects.end())
            break;
        
        ret = iter->second;

    BREAK_END;
    
    return ret;
}
    
IComponent* ObjectManager::queryComponent(uint32_t name, ComponentTypeID ctid)
{
    IComponent *ret = NULL;
    
    BREAK_START;
    
        if (!isObjectExists(name))
            break;
        
        ObjectMap::iterator objIter = mObjects.find(name);
        if (objIter == mObjects.end())
            break;

        ComponentsMap *compMap = objIter->second;
        ComponentsMap::iterator compIter = compMap->find(ctid);
        if (compIter == compMap->end())
            break;
        
        ret = compIter->second;
        
    BREAK_END;
    
    return ret;
}
    
void ObjectManager::registerBuiltinComponents()
{
    InputListener::RegisterComponentType();
    PhysicalObject::RegisterComponentType();
    SceneNode::RegisterComponentType();
    QuadEntity::RegisterComponentType();
    MapData::RegisterComponentType();
}
    
ComponentTypeID ObjectManager::getComponentTypeByName(uint32_t name)
{
    ComponentTypeID ret = COMP_NONE;

    BREAK_START;
        
    BREAK_END;

    return ret;
}
    
bool ObjectManager::addComponentToObject(uint32_t objectName, IComponent* comp)
{
    bool ret = false;
    
    BREAK_START;
    
    if (comp == NULL)
        break;
    
    // object not exists
    if (!isObjectExists(objectName))
    {
        ComponentsMap *compMap = new ComponentsMap;
        compMap->insert(ComponentsMap::value_type(comp->getComponentTypeID(), comp));
        mObjects.insert(ObjectMap::value_type(objectName, compMap));
        comp->mObjectName = objectName;
        ret = true;
    }
    else
    {
        ObjectMap::iterator objectIter = mObjects.find(objectName);
        if (objectIter == mObjects.end())
            break;

        ComponentsMap *compsMap = objectIter->second;
        if (compsMap == NULL)
            break;
        
        compsMap->insert(ComponentsMap::value_type(comp->getComponentTypeID(), comp));
        comp->mObjectName = objectName;
        ret = true;
    }

    BREAK_END;
    
    return ret;
}
    
// event
void ObjectManager::subscribeToEvent(ComponentTypeID ctid, EventID etid)
{
    mEventComponentSet[etid].insert(ctid);
}
    
void ObjectManager::postEvent(uint32_t objName, const Event& event)
{
    if (!isObjectExists(objName))
        return;
    
    ComponentsMap *compMap = getObjectComponents(objName);
    
    if (compMap != NULL)
    {

        EventComponentSet &eventCompSet = mEventComponentSet[event.eventID];
        EventComponentSet::iterator setIter = eventCompSet.begin();
        for (; setIter != eventCompSet.end(); ++setIter)
        {
            printf("%d\n", *setIter);
            ComponentsMap::iterator compMapIter = compMap->find((*setIter));
            if (compMapIter != compMap->end())
                (compMapIter->second)->handleEvent(event);
        }
    }
}
    
void ObjectManager::broadcastEvent(const Event& event)
{
    // iterate objects
    ObjectMap::iterator objmapIter = mObjects.begin();
    for (; objmapIter != mObjects.end(); ++objmapIter)
    {
        // get object's components
        ComponentsMap *compMap = objmapIter->second;
        if (compMap == NULL)
            continue;

        // find out what component subscript to this event
        EventComponentSet &eventCompSet = mEventComponentSet[event.eventID];
        EventComponentSet::iterator eventSetIter = eventCompSet.begin();
        for (; eventSetIter != eventCompSet.end(); ++eventSetIter)
        {
            ComponentsMap::iterator compMapIter = compMap->find(*(eventSetIter));
            if (compMapIter != compMap->end())
                (compMapIter->second)->handleEvent(event);
            
        }
    }
}

HGNAMESPACE_END