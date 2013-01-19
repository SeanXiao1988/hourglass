/**
 *  @file    HGPhysicsManager.cpp
 *  @brief   Physics manager implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/09/14
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

#include "HGPhysicsManager.h"
#include "HGPhysicalObject.h"
#include "HGHash.h"

HGNAMESPACE_START

PhysicsManager::PhysicsManager()
    : mFriction(0.0f)
    , mRestitution(0.0f)
{

}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::initialize()
{
}

void PhysicsManager::deInitialize()
{
    deleteAllObjects();
}

void PhysicsManager::update(float dt)
{

}

void PhysicsManager::debug_draw()
{
    if (!mPhysicalObjects.empty())
    {
        PhysicalObjMap::iterator iter = mPhysicalObjects.begin();
        for (; iter != mPhysicalObjects.end(); ++iter)
        {
            // (*iter).second->render();
        }
    }
}

// PhysicalObject
PhysicalObject* PhysicsManager::createObject(const std::string& name)
{
    mMutex.lock();
    
    PhysicalObject * obj = new PhysicalObject();
    mPhysicalObjects.insert(PhysicalObjMap::value_type(Hash(name.c_str()), obj));
    
    mMutex.unlock();
    
    return obj;
}
 
PhysicalObject* PhysicsManager::getObject(const std::string& name)
{
    if (mPhysicalObjects.empty())
    {
        return NULL;
    }
    
    PhysicalObjMap::iterator iter = mPhysicalObjects.find(Hash(name.c_str()));
	if (iter != mPhysicalObjects.end())
	{
		return (*iter).second;
	}
    
    return NULL;
}
    
void PhysicsManager::deleteObject(PhysicalObject* obj)
{
    if (obj == NULL || mPhysicalObjects.empty())
    {
        return;
    }
    
    mMutex.lock();
    
    PhysicalObjMap::iterator iter = mPhysicalObjects.begin();
    for (; iter != mPhysicalObjects.end(); ++iter)
    {
        if ((*iter).second == obj)
        {
            delete (*iter).second;
            mPhysicalObjects.erase(iter);
            break;
        }
    }
    
    mMutex.unlock();
}
    
void PhysicsManager::deleteObject(const std::string& name)
{
    mMutex.lock();
    
    PhysicalObject* obj = getObject(name);
    if (obj != NULL)
    {
        PhysicalObjMap::iterator iter = mPhysicalObjects.begin();
        for (; iter != mPhysicalObjects.end(); ++iter)
        {
            if ((*iter).second == obj)
            {
                delete (*iter).second;
                mPhysicalObjects.erase(iter);
                break;
            }
        }
    }
    
    mMutex.unlock();
}
    
void PhysicsManager::deleteAllObjects()
{
    mMutex.lock();
    
    if (!mPhysicalObjects.empty())
    {
        PhysicalObjMap::iterator iter = mPhysicalObjects.begin();
        for (; iter != mPhysicalObjects.end(); ++iter)
        {
            delete (*iter).second;
        }
    }
    
    mPhysicalObjects.clear();
    
    mMutex.unlock();
}
    
HGNAMESPACE_END
