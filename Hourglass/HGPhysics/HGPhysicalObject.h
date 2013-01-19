/**
 *  @file    HGPhysicalObject.h
 *  @brief   Physics Object header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/09/16
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

#ifndef HGPHYSICALOBJECT_H_
#define HGPHYSICALOBJECT_H_

#include "HGSystem.h"
#include "HGEvent.h"
#include "HGIComponent.h"
#include "HGGeometry.hpp"
#include "HGPhysicsDef.h"

HGNAMESPACE_START

// forward declaration
class MapData;

class PhysicalObject : public IComponent
{
public:
    PhysicalObject();
    ~PhysicalObject();
    
    void                    setMap(MapData *map) { mMap = map; }
	void                    applyDisplacement(Vector2Di displace);
    void                    performDuckJump();
    
	tile_collider_t         getMapCollider();
    
	void                    debug_draw();
    void                    update(double dt);
    
    const PHYSICAL_OBJ_ID&  getPhsyicalObjID() const { return mPhyObjID; }
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_PHYSICAL_OBJECT; };
    virtual uint32_t        getComponentName();
    
private:
    void _updateCollider();
    
    static int _alignToEdge(int& displace, int& x);
    
    void _applyHorizontal(int displace);
    void _applyVerticalUp(int displace);
    void _applyVerticalDown(int displace);

    void _constructCollider(tile_collider_t& c);
    CollisionRects _constructCollisionRects();
    void _applyOffset(int x, int y);
    
    // object to object collision
    PHYSICAL_OBJ_ID mPhyObjID;
    CollisionRects  mCollisionRects;
    
    
    // map collision
    TileColliderStatus  mColliderStatus;
    Point2i         mColliderLower;
    Point2i         mColliderUpper;
    
    Point2i         mPosition;  // object's center position
    Point2i         mVelocity;  // object's velocity vector
    int             mMovement;  // object's horizontal movement
    int             mGravity;
    
    bool            mIsJumpEnable;
    bool            mIsFirstJump;
    bool            mIsJumping;
    bool            mIsDoubleJumpEnable;
    int             mDoubleJumpCount;
    int             mJumpEnergy;
    
	MapData			*mMap;
};

HGNAMESPACE_END

#endif // HGPHYSICALOBJECT_H_
