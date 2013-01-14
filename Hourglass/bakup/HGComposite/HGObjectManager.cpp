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
#include "HGParamParser.h"
#include "HGData.h"

#include "HGPhysicalObject.h"
#include "HGSceneNode.h"
#include "HGQuadEntity.h"

#include "HGHash.h"

namespace HG
{
    
ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::initialize()
{
    memset(mComponentInfo, 0, sizeof(component_info_t) * COMP_COUNT);
	registerBuiltinComponents();
}

void ObjectManager::deInitialize()
{
    deleteObjects();
}

bool ObjectManager::loadObjectsFromFile(const char *filename)
{
    bool ret = false;
    
    Data *data = Data::CreateFromFileContent(filename);
    
    BREAK_START;

        if (data == NULL)
            break;
        
        ret = loadObjects(data);
        
    BREAK_END;
    
    delete data;
    
    return ret;
}

bool ObjectManager::loadObjects(Data *data)
{
    bool ret = false;
    
    BREAK_START;
        
        if (data == NULL)
            break;

        ParamParser parser(data->getData());
        
        for (param_node_t *curNode = parser.getCurrentNode(); curNode != NULL; curNode = parser.moveToNextNode())
        {
            if (parser.getParameterType() != TypeHashObject)
            {
                ret = false;
                break;
            }
            
            uint32_t objectID = createObject(parser);
            //PostMessage(oId, MT_OBJECT_CREATED);
        }
        
    BREAK_END;
    
    return ret;
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

// notice here, we don't pass reference, cause the parser will be moveToNext()
// so we need a copy of it
uint32_t ObjectManager::createObject(ParamParser node)
{
    return createObject(node, node.getName());
}

    
uint32_t ObjectManager::createObject(ParamParser node, uint32_t name)
{
    uint32_t ret = 0;
    
    BREAK_START;
        
        if(node.getParameterType() != TypeHashObject)
            break;
        
        if (isObjectExists(name))
        {
            ret = name;
            break;
        }
        
        // Create all the components, and add them to the db
        for (param_node_t *child = node.moveToFirstChild(); child != NULL; child = node.moveToNextNode())
        {
            IComponent *comp = createComponent(name, node);
			addComponentToObject(comp, name);
        }
        
        ret = name;
        
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
            
            ComponentDeleteMethod delFunc = mComponentInfo[compMapIter->first].deleteMethod;
            if (delFunc)
            {
                component->deInit();
                delFunc(component);
            }
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
            
            ComponentDeleteMethod delFunc = mComponentInfo[compMapIter->first].deleteMethod;
            if (delFunc)
            {
                component->deInit();
                delFunc(component);
            }
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
    
// Component Methods
IComponent* ObjectManager::createComponent(uint32_t ownerName, ParamParser node)
{
    IComponent *ret = NULL;
    
	BREAK_START;
    
		ComponentTypeID ctid = node.getComponentType();
		if (ctid == COMP_NONE)
			break;

		ComponentCreateMethod newFunc = mComponentInfo[ctid].createMethod;
		if (newFunc == NULL)
			break;

		IComponent *newComp = newFunc();
		if (newComp != NULL)
		{
			newComp->mObjectName = ownerName;
			newComp->init(&node);
		}
		
		ret = newComp;

    BREAK_END;

    return ret;
}

void ObjectManager::registerBuiltinComponents()
{
    PhysicalObject::RegisterComponentType();
    SceneNode::RegisterComponentType();
    QuadEntity::RegisterComponentType();
}

void ObjectManager::registerComponent(ComponentTypeID ctid, uint32_t compName, ComponentCreateMethod createMethod, ComponentDeleteMethod deleteMethod)
{
	mComponentInfo[ctid].createMethod = createMethod;
	mComponentInfo[ctid].deleteMethod = deleteMethod;
	mComponentInfo[ctid].hashName = compName;
}
    
ComponentTypeID ObjectManager::getComponentTypeByName(uint32_t name)
{
    ComponentTypeID ret = COMP_NONE;

    BREAK_START;
    
		for (int ctid = (int)COMP_NONE; ctid < (int)COMP_COUNT; ctid++)
		{
			if (mComponentInfo[ctid].hashName == name)
			{
				ret = (ComponentTypeID)ctid;
				break;
			}
		}
        
    BREAK_END;

    return ret;
}
    
bool ObjectManager::addComponentToObject(IComponent* comp, uint32_t objectName)
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
        }

		ObjectMap::iterator objectIter = mObjects.find(objectName);
		if (objectIter == mObjects.end())
			break;

		ComponentsMap *compsMap = objectIter->second;
        if (compsMap == NULL)
            break;
        
        compsMap->insert(ComponentsMap::value_type(comp->getComponentTypeID(), comp));
        ret = true;

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

        EventComponentSet &eventCompSet = mEventComponentSet[event.getEventID()];
        EventComponentSet::iterator setIter = eventCompSet.begin();
        for (; setIter != eventCompSet.end(); ++setIter)
        {
            ComponentsMap::iterator compMapIter = compMap->find((*setIter));
            if (compMapIter != compMap->end())
                (compMapIter->second)->handleEvent(event);
        }
    }
}
    
void ObjectManager::broadcastEvent(const Event& event)
{
    ObjectMap::iterator objmapIter = mObjects.begin();
    for (; objmapIter != mObjects.end(); ++objmapIter)
    {
        ComponentsMap *compMap = objmapIter->second;
        if (compMap == NULL)
            continue;

        EventComponentSet &eventCompSet = mEventComponentSet[event.getEventID()];
        EventComponentSet::iterator eventSetIter = eventCompSet.begin();
        for (; eventSetIter != eventCompSet.end(); ++eventSetIter)
        {
            ComponentsMap::iterator compMapIter = compMap->find(*(eventSetIter));
            if (compMapIter != compMap->end())
                (compMapIter->second)->handleEvent(event);
            
        }
    }
}

}