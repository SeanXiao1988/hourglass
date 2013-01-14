/**
 *  @file    HGPhysicsManager.h
 *  @brief   Physics manager header
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

#ifndef HGPHYSICSMANAGER_H_
#define HGPHYSICSMANAGER_H_

#include "HGSystem.h"
#include "HGMutex.h"

namespace HG
{

#define PHYSICSMANAGER PhysicsManager::getInstance()

class PhysicalObject;

typedef std::map<uint32_t, PhysicalObject*> PhysicalObjMap;
    
class PhysicsManager
{
public:
    static PhysicsManager& getInstance()
    {
        static PhysicsManager instance;
        return instance;
    }
    
    void            initialize();
    void            deInitialize();
    
    void            update(float dt);
    void            debug_draw();
    
    // PhysicalObject
    PhysicalObject* createObject(const std::string& name);
    
    PhysicalObject* getObject(const std::string& name);
    void            deleteObject(PhysicalObject* obj);
    void            deleteObject(const std::string& name);
    void            deleteAllObjects();
    
    // Constants
    const float&    getFriction() const { return mFriction; }
    const float&    getRestitution() const { return mRestitution; }
    
    void            setFriction(const float friction) { mFriction = friction; }
    void            setRestitution(const float res) { mRestitution = res; }

private:
    float   mFriction;
    float   mRestitution;
    
    Mutex                   mMutex;
    PhysicalObjMap          mPhysicalObjects;
    
private:
    PhysicsManager();
    PhysicsManager(const PhysicsManager& other);
    ~PhysicsManager();
    PhysicsManager& operator= (const PhysicsManager& rhs);
};

}

#endif // HGPHYSICSMANAGER_H_
