/**
 *  @file    HGPhysicalObject.cpp
 *  @brief   Physics object implementation
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

#include "HGHash.h"
#include "HGPhysicalObject.h"
#include "HGObjectManager.h"
#include "HGMapData.h"

#include "HGRender.h"

#define GRAVITY             1
#define JUMP_ENEGRY         8
#define JUMP_INIT_HEIGHT    8
#define JUMP_ENEGRY_DELTA   2
#define HOR_MOVE_SPEED      4

HGNAMESPACE_START
        
void PhysicalObject::RegisterComponentType()
{
}
    
void PhysicalObject::deInitialize()
{
    
}
    
EventResult PhysicalObject::handleEvent(const Event &event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t PhysicalObject::getComponentName()
{
    return Hash("PhysicalComponent");
}

PhysicalObject::PhysicalObject()
    : IComponent()
    , mMovement(0)
    , mGravity(1)
    , mIsJumpEnable(true)
    , mIsJumping(false)
    , mIsFirstJump(true)
    , mIsDoubleJumpEnable(true)
    , mDoubleJumpCount(1)
    , mJumpEnergy(8)
    , mPhyObjID(PHYSICAL_UNKNOWN)
{
    mPosition.setValue(0, 0);
    mColliderLower.setValue(0, 0);
    mColliderUpper.setValue(0, 0);
}

PhysicalObject::~PhysicalObject()
{
    mCollisionRects.clear();
}
    
// Physical Object method
void PhysicalObject::applyDisplacement(Vector2Di displace)
{
    // if map
    if (mMap == NULL)
        return;
    
    if (displace.y < 0)
    {
        _applyVerticalUp(displace.y);
    }
    else
    {
        _applyVerticalDown(displace.y);
    }
    
    _applyHorizontal(displace.x);
}
    
void PhysicalObject::performDuckJump()
{
    if (mMap == NULL)
        return;
    
    if (mColliderStatus != MC_STATUS_ON_GROUND)
        return;
    
    tile_collider_t collider;
    _constructCollider(collider);
    
    int destX = collider.lower_climber.x;
    int destY = collider.lower_climber.y + 1;
    if (mMap->getTileTypeByGridCoord(destX/TILE_SIZE, destY/TILE_SIZE) == Tile_Platform)
    {
        _applyOffset(0, 2);
        mColliderStatus = MC_STATUS_IN_AIR;
    }
}
    
tile_collider_t PhysicalObject::getMapCollider()
{
    tile_collider_t collider;
    _constructCollider(collider);
    
    return collider;
}
    
int PhysicalObject::_alignToEdge(int& displace, int& x)
{
    if (displace < 0)
    {
        return (x/TILE_SIZE + 1) * TILE_SIZE;
    }
    else
    {
        return (x/TILE_SIZE) * TILE_SIZE - 1;
    }
}
    
void PhysicalObject::_applyHorizontal(int displace)
{
    BREAK_START;
    
    if (displace == 0.0f || mMap == NULL)
        break;
    
    // limit maximum speed
    displace = CLAMP(displace, -TILE_SIZE, TILE_SIZE);
    
    tile_collider_t collider;
    _constructCollider(collider);
    
    int curLowerX = collider.lower_climber.x;
    int curLowerY = collider.lower_climber.y;
    int curUpperY = collider.upper_climber.y;
    
    int destUpperX = collider.upper_climber.x + displace;
    
    // test upper climber first
    if (mMap->getTileType(destUpperX, curUpperY) != Tile_None)
    {
        destUpperX = _alignToEdge(displace, destUpperX);
        int offsetX = destUpperX - curLowerX;
        _applyOffset(offsetX, 0);
        break;
    }
    
    //* test if middle detector collide with non non-solid tiles
    //  this can prevent collider move through single tile
    if (collider.middle_detector.x != -1)
    {
        TileType middleType = mMap->getTileType(collider.middle_detector.x+displace, collider.middle_detector.y);
        if (middleType != Tile_None && mColliderStatus != MC_STATUS_ON_SLOPE)
        {
            int destX = collider.middle_detector.x+displace;
            destX = _alignToEdge(displace, destX);
            int offsetX = destX - curLowerX;
            _applyOffset(offsetX, 0);
            break;
        }
    }
    //*/
    
    // collider is in the air
    if (mColliderStatus == MC_STATUS_IN_AIR)
    {
        int destLowerX = collider.lower_climber.x + displace;
        int destUpperX = collider.upper_climber.x + displace;
        
        TileType destTileTypeLower = mMap->getTileType(destLowerX, curLowerY);
        TileType destTileTypeUpper = mMap->getTileType(destUpperX, curUpperY);
        
        // nothing in my way
        if (destTileTypeLower == Tile_None && destTileTypeUpper == Tile_None)
        {
            _applyOffset(displace, 0);
            break;
        }
        // hit the wall
        else if (destTileTypeUpper == Tile_Solid || destTileTypeLower == Tile_Solid)
        {
            // calculate the finalX
            destLowerX = _alignToEdge(displace, destLowerX);
            int offsetX = destLowerX - curLowerX;
            _applyOffset(offsetX, 0);
            break;
        }
        // upper climber hit the slope, for now , we just treat it as solid tile
        else if (destTileTypeUpper != Tile_None && destTileTypeUpper != Tile_Solid)
        {
            // calculate the finalX
            destLowerX = _alignToEdge(displace, destLowerX);
            int offsetX = destLowerX - curLowerX;
            _applyOffset(offsetX, 0);
            break;
        }
        // lower climber hit the slope tile
        else if (destTileTypeLower != Tile_None && destTileTypeLower != Tile_Solid)
        {
            // move out of current tile
            if (curLowerX/TILE_SIZE != destLowerX/TILE_SIZE)
            {
                // calculate the right edge's height of the slope tile
                int tileIndex = (displace < 0) ? (TILE_SIZE - 1) : 0;
                int tileEdgeHeight = MapData::TileMask[destTileTypeLower][tileIndex];
                // lower climber lower than tile's edge,
                if (curLowerY%TILE_SIZE <= (TILE_SIZE - tileEdgeHeight))
                {
                    // empty part of the tile
                    if (!mMap->isSolid(destLowerX, curLowerY))
                    {
                        int offsetX = destLowerX - curLowerX;
                        _applyOffset(offsetX, 0);
                        break;
                    }
                    // attach to slope's surface
                    else
                    {
                        int slopeHeight = MapData::TileMask[destTileTypeLower][destLowerX%TILE_SIZE];
                        int destY = (curLowerY/TILE_SIZE + 1) * TILE_SIZE - slopeHeight;
                        int offsetX = destLowerX - curLowerX;
                        int offsetY = destY - curLowerY;
                        _applyOffset(offsetX, offsetY);
                        mColliderStatus = MC_STATUS_ON_SLOPE;
                        break;
                    }
                }
                // lower climber higher than tile's edge, collision
                else
                {
                    // calculate the finalX
                    destLowerX = _alignToEdge(displace, destLowerX);
                    int offsetX = destLowerX - curLowerX;
                    _applyOffset(offsetX, 0);
                    break;
                }
                
            }
            // move within the tile
            else
            {
                // empty part of the tile
                if (!mMap->isSolid(destLowerX, curLowerY))
                {
                    int offsetX = destLowerX - curLowerX;
                    _applyOffset(offsetX, 0);
                    break;
                }
                // attach to slope's surface
                else
                {
                    int slopeHeight = MapData::TileMask[destTileTypeLower][destLowerX%TILE_SIZE];
                    int destY = (curLowerY/TILE_SIZE + 1) * TILE_SIZE - slopeHeight;
                    int offsetX = destLowerX - curLowerX;
                    int offsetY = destY - curLowerY;
                    _applyOffset(offsetX, offsetY);
                    mColliderStatus = MC_STATUS_ON_SLOPE;
                    break;
                }
            }
        }
    }
    // on ground or on slope
    else
    {
        int curLowerX = collider.lower_climber.x;
        int curLowerY = collider.lower_climber.y;
        TileType curTileTypeLower = mMap->getTileType(curLowerX, curLowerY, DETECT_PLATFORM_DOWN);
        
        // add resistance factor to displace
        int unitDisplace = displace / abs(displace);
        displace = (int)((float)displace * tile_resistance_factor[curTileTypeLower]);
        if (displace == 0)
            displace = unitDisplace;
        
        int destLowerX = collider.lower_climber.x + displace;
        TileType destTileTypeLower = mMap->getTileType(destLowerX, curLowerY, DETECT_PLATFORM_UP);
        
        // move within the tile
        if (curLowerX/TILE_SIZE == destLowerX/TILE_SIZE)
        {
            if (mColliderStatus == MC_STATUS_ON_SLOPE)
            {
                int slopeHight = MapData::TileMask[curTileTypeLower][destLowerX%TILE_SIZE];
                int destY = (curLowerY/TILE_SIZE + 1) * TILE_SIZE - slopeHight;
                int offsetX = destLowerX - curLowerX;
                int offsetY = destY - curLowerY;
                _applyOffset(offsetX, offsetY);
                break;
            }
            // on ground
            else
            {
                int offsetX = destLowerX - curLowerX;
                _applyOffset(offsetX, 0);
                break;
            }
        }
        // move out the tile
        else
        {
            // move into a slope
            if (destTileTypeLower != Tile_None && destTileTypeLower != Tile_Solid)
            {
                int slopeHight = MapData::TileMask[destTileTypeLower][destLowerX%TILE_SIZE];
                int destY = (curLowerY/TILE_SIZE + 1) * TILE_SIZE - slopeHight;
                int offsetX = destLowerX - curLowerX;
                int offsetY = destY - curLowerY;
                _applyOffset(offsetX, offsetY);
                mColliderStatus = MC_STATUS_ON_SLOPE;
                break;
            }
            // into a non-solid or a solid tile
            // need to check destination tile's above and below tile type
            else
            {
                TileType aboveTile = mMap->getTileTypeByGridCoord(destLowerX/TILE_SIZE, curLowerY/TILE_SIZE-1);
                TileType belowTile = mMap->getTileTypeByGridCoord(destLowerX/TILE_SIZE, curLowerY/TILE_SIZE+1);
                
                // slope tile above, move
                if (aboveTile != Tile_None && aboveTile != Tile_Solid && aboveTile != Tile_Platform)
                {
                    int slopeHeight = MapData::TileMask[aboveTile][destLowerX%TILE_SIZE];
                    int destY = (curLowerY/TILE_SIZE) * TILE_SIZE - slopeHeight;
                    int offsetX = destLowerX - curLowerX;
                    int offsetY = destY - curLowerY;
                    _applyOffset(offsetX, offsetY);
                    mColliderStatus = MC_STATUS_ON_SLOPE;
                    break;
                }
                // slope tile below, move
                else if (belowTile != Tile_None && belowTile != Tile_Solid && belowTile != Tile_Platform)
                {
                    int slopeHeight = MapData::TileMask[aboveTile][destLowerX%TILE_SIZE];
                    int destY = (curLowerY/TILE_SIZE + 1) * TILE_SIZE - slopeHeight;
                    int offsetX = destLowerX - curLowerX;
                    int offsetY = destY - curLowerY;
                    _applyOffset(offsetX, offsetY);
                    mColliderStatus = MC_STATUS_ON_SLOPE;
                    break;
                }
                // non-slope tile above or below
                else
                {
                    // if player is on slope, treat platform tile as solid tile
                    // if not, treat platform tile as non-solid
                    if (mColliderStatus == MC_STATUS_ON_SLOPE)
                        destTileTypeLower = mMap->getTileType(destLowerX, curLowerY, DETECT_PLATFORM_DOWN);
                    // nothing beneath the feet
                    if (destTileTypeLower == Tile_None)
                    {
                        if (belowTile == Tile_None)
                        {
                            int offsetX = destLowerX - curLowerX;
                            _applyOffset(offsetX, 0);
                            mColliderStatus = MC_STATUS_IN_AIR;
                            break;
                        }
                        else
                        {
                            int offsetX = destLowerX - curLowerX;
                            int destY = (curLowerY/TILE_SIZE+1) * TILE_SIZE - 1;
                            int offsetY = destY - curLowerY;
                            _applyOffset(offsetX, offsetY);
                            mColliderStatus = MC_STATUS_ON_GROUND;
                            break;
                        }
                    }
                    else if (destTileTypeLower == Tile_Solid)
                    {
                        int destX = 0;
                        if (displace < 0)
                            destX = (curLowerX/TILE_SIZE) * TILE_SIZE;
                        else
                            destX = (curLowerX/TILE_SIZE + 1) * TILE_SIZE - 1;

                        int offsetX = destX - curLowerX;
                        
                        // on slope
                        if (curTileTypeLower != Tile_None && curTileTypeLower != Tile_Solid)
                        {
                            int slopeHeight = MapData::TileMask[curTileTypeLower][destX%TILE_SIZE];
                            int destY = (curLowerY/TILE_SIZE+1)*TILE_SIZE - slopeHeight - 1;
                            int offsetY = destY - curLowerY;
                            _applyOffset(offsetX, offsetY);
                            mColliderStatus = MC_STATUS_ON_SLOPE;
                        }
                        else
                        {
                            _applyOffset(offsetX, 0);
                            mColliderStatus = MC_STATUS_ON_GROUND;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    BREAK_END;
}
    
void PhysicalObject::_applyVerticalUp(int displace)
{
    BREAK_START;
    
    if (displace >= 0 || mMap == NULL)
        break;
    
    // limit maximum speed
    displace = abs(displace) > TILE_SIZE ? TILE_SIZE : abs(displace);
    displace = -displace;
    
    tile_collider_t collider;
    _constructCollider(collider);
    
    int curX = collider.upper_climber.x;
    int curY = collider.upper_climber.y;
    int destY = collider.upper_climber.y + displace;
    TileType destTileType = mMap->getTileType(curX, destY);
    
    // fly~
    if (destTileType == Tile_None)
    {
        _applyOffset(0, displace);
        mColliderStatus = MC_STATUS_IN_AIR;
        break;
    }
    // ouch! there is the ceiling
    else if (destTileType == Tile_Solid)
    {
        // calculate the finalY of the upper_collider
        destY = ((destY / TILE_SIZE) + 1) * TILE_SIZE;
        int offsetY = destY - curY;
        _applyOffset(0, offsetY);
        mColliderStatus = MC_STATUS_IN_AIR;
        break;
    }
    else // upper slope not implement yet
    {
        // slope ? use the same algorithm as solid tile
        destY = ((destY / TILE_SIZE) + 1) * TILE_SIZE;
        int offsetY = destY - curY;
        _applyOffset(0, offsetY);
        mColliderStatus = MC_STATUS_IN_AIR;
        break;
    }
    
    BREAK_END;
}
    
void PhysicalObject::_applyVerticalDown(int displace)
{
    BREAK_START;
    
    if (displace <= 0 || mMap == NULL)
        break;
    
    // limit maximum speed
    displace = displace > TILE_SIZE ? TILE_SIZE : displace;
    
    tile_collider_t collider;
    _constructCollider(collider);
    
    // already on ground/slope, ignore the displacement
    if (mColliderStatus == MC_STATUS_ON_GROUND || mColliderStatus == MC_STATUS_ON_SLOPE)
    {
        if (!mMap->isSolid(collider.lower_climber.x, collider.lower_climber.y+1))
            mColliderStatus = MC_STATUS_IN_AIR;
        else
            break;
    }
    
    int curX = collider.lower_climber.x;
    int curY = collider.lower_climber.y;
    int destY = collider.lower_climber.y + displace;
    
    TileType curTileType = mMap->getTileType(curX, curY, DETECT_PLATFORM_DOWN);
    TileType destTileType = mMap->getTileType(curX, destY, DETECT_PLATFORM_DOWN);
    
    // player is in a slope tile and is about to move down
    if (curTileType != Tile_None && curTileType != Tile_Solid)
    {
        // about to move out the current tile
        if (curY / TILE_SIZE != destY / TILE_SIZE)
        {
            // calculate the Y above the below tile
            destY = (curY / TILE_SIZE + 1) * TILE_SIZE;
            // move to the current slope surface
            destY -= MapData::TileMask[curTileType][curX % TILE_SIZE];
            
            int offsetY =  destY - curY;
            _applyOffset(0, offsetY);
            mColliderStatus = MC_STATUS_ON_SLOPE;
            break;
        }
        // not going out of current tile
        else
        {
            // still in the air
            if (!mMap->isSolid(curX, destY))
            {
                int offsetY = destY - curY;
                _applyOffset(0, offsetY);
                break;
            }
            // about to hit the slope surface
            else
            {
                // move to the bottom of the slope tile
                destY = (curY / TILE_SIZE + 1) * TILE_SIZE;
                // move to the surface of the slope tile
                destY -= MapData::TileMask[curTileType][curX % TILE_SIZE];
                
                int offsetY = destY - curY;
                _applyOffset(0, offsetY);
                mColliderStatus = MC_STATUS_ON_SLOPE;
                break;
            }
        }
    }
    // player is in the air, is about to move down
    else
    {
        // nothing, might be air or the empty part of a slope tile
        if (!mMap->isSolid(curX, destY, DETECT_PLATFORM_DOWN))
        {
            int offsetY = destY - curY;
            _applyOffset(0, offsetY);
            break;
        }
        // hit the solid tile or the solid part of a slope tile
        else
        {
            if (destTileType == Tile_Solid)
            {
                destY = (curY / TILE_SIZE + 1) * TILE_SIZE - 1;
                int offsetY = destY - curY;
                _applyOffset(0, offsetY);
                mColliderStatus = MC_STATUS_ON_GROUND;
                break;
            }
            // hit solid part of a slope tile
            else
            {
                destY = (destY / TILE_SIZE + 1) * TILE_SIZE;
                destY -= MapData::TileMask[destTileType][curX % TILE_SIZE];
                
                int offsetY = destY - curY;
                _applyOffset(0, offsetY);
                mColliderStatus = MC_STATUS_ON_SLOPE;
                break;
            }
        }
    }
    
    BREAK_END;
}
    
// construct map collider with current position
void PhysicalObject::_constructCollider(tile_collider_t &c)
{
    c.upper_climber = mColliderUpper + mPosition;
    c.lower_climber = mColliderLower + mPosition;
    if (c.lower_climber.y - c.upper_climber.y > TILE_SIZE)
    {
        c.middle_detector.x = c.lower_climber.x;
        c.middle_detector.y = c.lower_climber.y - TILE_SIZE;
    }
    else
    {
        c.middle_detector.setValue(-1, -1);
    }
}
    
// construct object-object collision rectangles with current position
CollisionRects PhysicalObject::_constructCollisionRects()
{
    CollisionRects rects;
    CollisionRects::iterator iter = mCollisionRects.begin();
    for (; iter != mCollisionRects.end(); ++iter)
    {
        Recti rect = *iter;
        rect.addOffset(mPosition);
        rects.push_back(rect);
    }
    
    return rects;
}
    
void PhysicalObject::_applyOffset(int x, int y)
{
    mPosition.x += x;
    mPosition.y += y;
}
    
void PhysicalObject::debug_draw()
{
    int colorGround = 0xFF6600FF;
    int colorAir = 0x0000FFFF;
    int colorSlope = 0xFF99FFFF;
    int color = 0x00000000;
    switch (mColliderStatus)
    {
        case MC_STATUS_IN_AIR:
            color = colorAir;
            break;
            
        case MC_STATUS_ON_GROUND:
            color = colorGround;
            break;
            
        case MC_STATUS_ON_SLOPE:
            color = colorSlope;
            break;
            
        default:
            break;
    }
    
    /*
     map_point_t displace;
     int mouseX = 0;
     int mouseY = 0;
     glfwGetMousePos(&mouseX, &mouseY);
     displace.x = (mouseX - mCollider.upper_slope_climber.x);
     displace.y = (mouseY - mCollider.upper_slope_climber.y) + 10;
     applyDisplacement(displace);
     */
    tile_collider_t collider;
    _constructCollider(collider);
    
    RENDER.renderLine(collider.lower_climber.x, collider.lower_climber.y, collider.upper_climber.x, collider.upper_climber.y, color, 1.0f);
    
    mMovement = 0;
    mVelocity.x = 0;
    if (glfwGetKey('D') == GLFW_PRESS)
    {
        mMovement = HOR_MOVE_SPEED;
    }
    
    if (glfwGetKey('A') == GLFW_PRESS)
    {
        mMovement = -HOR_MOVE_SPEED;
    }
    
    if (glfwGetKey('S') == GLFW_PRESS && glfwGetKey('K') == GLFW_PRESS)
    {
        performDuckJump();
    }
    else if (glfwGetKey('K') == GLFW_PRESS)
    {
        if (mIsFirstJump && mIsJumpEnable)
        {
            mVelocity.y = -JUMP_INIT_HEIGHT;
            mIsJumping = true;
            mIsFirstJump = false;
            mIsJumpEnable = false;
        }
        else if (mIsJumping && mJumpEnergy > 0)
        {
            mVelocity.y -= JUMP_ENEGRY_DELTA;
            mJumpEnergy -= JUMP_ENEGRY_DELTA;
        }
        else if (mIsDoubleJumpEnable && mDoubleJumpCount > 0)
        {
            printf("Double Jump\n");
            printf("-=-=-=-=-=-=-=\n");
            mJumpEnergy = JUMP_INIT_HEIGHT;
            mIsDoubleJumpEnable = false;
            mIsFirstJump = true;
            mIsJumpEnable = true;
            mDoubleJumpCount--;
        }
    }
    
    if (glfwGetKey('K') == GLFW_RELEASE)
    {
        mIsDoubleJumpEnable = true;
        mIsJumpEnable = true;
        mIsJumping = false;
    }
    
    RENDER.renderBox(mPosition.x - 10, mPosition.y - 10, 20, 20, 0x00000080, 1.0);
    
    CollisionRects collisionRects = _constructCollisionRects();
    for (CollisionRects::iterator iter = collisionRects.begin(); iter != collisionRects.end(); ++iter)
    {
        float x = (float)(*iter).getLeft();
        float y = (float)(*iter).getTop();
        float w = (float)(*iter).getWidth();
        float h = (float)(*iter).getHeight();
        RENDER.renderBox(x, y, w, h, 0xFF000080, 1.0);
    }
    
//    applyDisplacement(displace);
}
    
void PhysicalObject::update(double dt)
{
    if (dt <= 0.0)
        return;

	float prevX = mPosition.x;
	float prevY = mPosition.y;
    
    //mVelocity.setValue(0.0f, 0.0f);
	// base velocity is a combination of horizontal movement control and
	// acceleration downward due to gravity.
	mVelocity.x += mMovement;//mAcceleration * dt;
    mVelocity.y = CLAMP(mVelocity.y + GRAVITY, -TILE_SIZE, TILE_SIZE);
    
	// m_velocity.y = DoJump(m_velocity.y, dt);
    
	
	// Apply pseudo-drag horizontally.
	//if (m_bIsOnGround)
	//	m_velocity.x *= GroundDragFactor;
	//else
	//	m_velocity.x *= AirDragFactor;
    
	// Prevent the player from running faster than his top speed.
    mVelocity.x = CLAMP(mVelocity.x, -TILE_SIZE, TILE_SIZE);
    
	// Apply velocity.
    Vector2Di disp;
    disp.setValue((int)mVelocity.x, (int)mVelocity.y);
    applyDisplacement(disp);
    
	// If the collision stopped us from moving, reset the velocity to zero.
    
	if (mPosition.x == prevX)
		mVelocity.x = 0;
	if (mPosition.y == prevY)
		mVelocity.y = 0;
    
    if (mColliderStatus == MC_STATUS_ON_GROUND || mColliderStatus == MC_STATUS_ON_SLOPE)
    {
        mIsJumping = false;
        mIsDoubleJumpEnable = false;
        mDoubleJumpCount = 1;
        mIsFirstJump = true;
        mJumpEnergy = JUMP_ENEGRY;
    }
}
    
HGNAMESPACE_END