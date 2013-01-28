/**
 *  @file    HGParticleEmitter.h
 *  @brief   Particle emitter header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/28
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

#ifndef HGPARTICLEEMITTER_H_
#define HGPARTICLEEMITTER_H_

#include "HGISceneEntity.h"
#include "HGGraphicsDef.h"
#include "HGGeometry.hpp"

HGNAMESPACE_START

// https://github.com/cocos2d/cocos2d-x/blob/master/cocos2dx/particle_nodes/CCParticleSystem.h

// particle mode
typedef struct _particle_mode_gravity_t_
{
    Point2f dir;
    float   radialAccel;
    float   tangentialAccel;
}particle_mode_gravity_t;

typedef struct _particle_mode_radius_t_
{
    float   angle;
    float   angularSpeed;
    float   radius;
    float   radiusDelta;
}particle_mode_radius_t;

// particle
typedef struct _particle_t_
{
    Point2f     pos;
    Point2f     startPos;
    color4f_t   color;
    color4f_t   colorDelta;
    
    float       size;
    float       sizeDelta;
    
    float       rotation;
    float       rotationDelta;
    
    float       currentDuration;
    
    float       u,v,w,h;
    
    particle_mode_gravity_t gravityMode;
    particle_mode_radius_t  radiusMode;
}particle_t;

// emitter mode
typedef struct _emitter_mode_gravity_t_
{
    Point2f gravity;
    float   speed;
    float   speedVar;
    float   tangentialAccel;
    float   tangentialAccelVar;
    float   radialAccel;
    float   radialAccelVar;
}emitter_mode_gravity_t;

typedef struct _emitter_mode_radius_t
{
    float   startRadius;
    float   startRadiusVar;
    float   endRadius;
    float   endRadiusVar;
    float   angularSpeed;
    float   angularSpeedVar;
}emitter_mode_radius_t;

typedef enum _emitter_mode_
{
    EMITTER_GRAVITY = 0,
    EMITTER_RADIUS
}EmitterMode;

// Emitter class
class ParticleEmitter : public ISceneEntity
{
public:
    ParticleEmitter();
    ParticleEmitter(int32_t count);
    ~ParticleEmitter();
    
    // ISceneEntity
    virtual void    update(const float dt);
	virtual void    render();
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_PARTICLE_EMITTER; };
    virtual uint32_t        getComponentName();
    
    //
    void            setParticleCount(int32_t count);
    const int32_t&  getParticleCount() const { return mParticleCount; }
    
    void            setEmissionRate(int32_t particles);
    const int32_t&  getEmissionRate() const { return mEmissionRate; }
    
    void            setRemoveWhenFinish(bool remove) { mRemoveWhenFinish = remove; }
    const bool&     isRemoveWhenFinish() const { return mRemoveWhenFinish; }
    
    void            setDuration(float duration);
    const float&    getDuration() const { return mDuration; }
    
    void            setDurationVar(float durationVar);
    const float&    getDurationVar() const { return mDurationVar; }
    
    void            setAngle(float angle) { mAngle = angle; }
    const float&    getAngle() const { return mAngle; }
    
    void            setAngleVar(float angleVar) { mAngleVar = angleVar; }
    const float&    getAngleVar() const { return mAngleVar; }
    
    // mode gravity
    void            setGravity(const Point2f& gravity) { mGravity = gravity; }
    const Point2f&  getGravity() const { return mGravity; }
    
    void            setActive(bool active) { mIsActive = active; }
    const bool&     isActive() const { return mIsActive; }
    
private:
    particle_t*     mParticles;
    int32_t         mParticleCount;
    int32_t         mParticleIndex;
    int32_t         mEmissionRate;
    
    bool            mRemoveWhenFinish;
    
    float           mDuration;
    float           mDurationVar;
    float           mCurrentDuration;
    float           mAngle;
    float           mAngleVar;
    
    bool            mIsActive;
    
    Point2f         mPositionVar;
    
    // mode gravity
    Point2f         mGravity;
    float           mSpeed;
    float           mSpeedVar;
    float           mTangentialAccel;
    float           mTangentialAccelVar;
    float           mRadialAccel;
    float           mRadialAccelVar;
    
    // mode radius
    float           mStartRadius;
    float           mStartRadiusVar;
    float           mEndRadius;
    float           mEndRadiusVar;
    float           mAngularSpeed;
    float           mAngularSpeedVar;
    
    // mode both
    float           mStartSize;
    float           mStartSizeVar;
    float           mEndSize;
    float           mEndSizeVar;
    color4f_t       mStartColor;
    color4f_t       mStartColorVar;
    color4f_t       mEndColor;
    color4f_t       mEndColorVar;
    float           mStartSpin;
    float           mStartSpinVar;
    float           mEndSpin;
    float           mEndSpinVar;
    
    Quad            mQuad;
    
    EmitterMode     mMode;
};

HGNAMESPACE_END

#endif // HGPARTICLEEMITTER_H_
