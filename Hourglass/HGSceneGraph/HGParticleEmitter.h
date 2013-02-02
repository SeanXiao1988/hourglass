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

// Particle
#define PARTICLE_DURATION_INFINITY          -1.0f
#define PARTICLE_SIZE_START_EQUALS_END      -1.0f
#define PARTICLE_RADIUS_START_EQUALS_END    -1.0f

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
    
    float       lifeTime;
    
    float       u,v,w,h;
    
    particle_mode_gravity_t modeGravity;
    particle_mode_radius_t  modeRadius;
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

// Constants
typedef enum _emitter_mode_
{
    EMITTER_GRAVITY = 0,
    EMITTER_RADIUS
}EmitterMode;

typedef enum _particle_position_type_t_
{
    PPositionTypeFree = 0,
    PPositionTypeRelative,
    PPositionTypeGroup
}ParticlePositionType;

// Emitter class
class ParticleEmitter : public ISceneEntity
{
public:
    ParticleEmitter();
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
    
    // control
    void            fireEmitter();
    void            stopEmitter();
    
    //
    void            setEmitterMode(EmitterMode mode) { mMode = mode; }
    EmitterMode     getEmitterMode() { return mMode; }
    
    void            setParticlePositionType(ParticlePositionType type) { mParticlePositionType = type; }
    ParticlePositionType getParticlePositionType() { return mParticlePositionType; }
    
    void            setTexture(GLuint tex) { mQuad.tex = tex; }
    GLuint          getTexture() { return mQuad.tex; }
    
    void            setTextureRect(float x, float y, float w, float h);
    
    void            setTotalParticles(int32_t total);
    const int32_t&  getTotalParticles() const { return mTotalParticles; }
    
    const int32_t&  getParticleCount() const { return mParticleCount; }
    
    void            setEmissionRate(float rate) { mEmissionRate = rate; };
    const float&    getEmissionRate() const { return mEmissionRate; }
    
    void            setRemoveWhenFinish(bool remove) { mRemoveWhenFinish = remove; }
    const bool&     isRemoveWhenFinish() const { return mRemoveWhenFinish; }
    
    void            setDuration(float duration);
    const float&    getDuration() const { return mDuration; }
    
    void            setLifeTime(float life) { mLifeTime = life; }
    const float&    getLifeTime() const { return mLifeTime; }
    
    void            setLifeTimeVar(float var) { mLifeTimeVar = var; }
    const float&    getLifeTimeVar() const { return mLifeTimeVar; }
    
    void            setDurationVar(float var) { mDurationVar = var; }
    const float&    getDurationVar() const { return mDurationVar; }
    
    void            setAngle(float angle) { mAngle = angle; }
    const float&    getAngle() const { return mAngle; }
    
    void            setAngleVar(float var) { mAngleVar = var; }
    const float&    getAngleVar() const { return mAngleVar; }
    
    void            setPositionVar(Point2f var) { mPositionVar = var; }
    const Point2f&  getPositionVar() const { return mPositionVar; }
    
    void            setActive(bool active) { mIsActive = active; }
    const bool&     isActive() const { return mIsActive; }
    
    // mode gravity
    void            setGravity(const Point2f& gravity) { mGravity = gravity; }
    const Point2f&  getGravity() const { return mGravity; }
    
    void            setSpeed(float speed) { mSpeed = speed; }
    const float&    getSpeed() const { return mSpeed; }
    
    void            setSpeedVar(float var) { mSpeedVar = var; }
    const float&    getSpeedVar() { return mSpeedVar; }
    
    void            setTangentialAccel(float accel) { mTangentialAccel = accel; }
    const float&    getTangentialAccel() const { return mTangentialAccel; }
    
    void            setTangentialAccelVar(float var) { mTangentialAccelVar = var; }
    const float&    getTangentialAccelVar() const { return mTangentialAccelVar; }
    
    void            setRadialAccel(float accel) { mRadialAccel = accel; }
    const float&    getRadialAccel() const { return mRadialAccel; }
    
    void            setRadialAccelVar(float var) { mRadialAccelVar = var; }
    const float&    getRadialAccelVar() const { return mRadialAccelVar; }
    
    void            setEmitterModeGravity(const emitter_mode_gravity_t& mode);
    emitter_mode_gravity_t* getEmitterModeGravity() { return &mModeGravity; }
    
    // mode radius
    void            setStartRadius(float radius) { mStartRadius = radius; }
    const float&    getStartRadius() const { return mStartRadius; }
    
    void            setStartRadiusVar(float var) { mStartRadiusVar = var; }
    const float&    getStartRadiusVar() const { return mStartRadiusVar; }
    
    void            setEndRadius(float radius) { mEndRadius = radius; }
    const float&    getEndRadius() const { return mEndRadius; }
    
    void            setEndRadiusVar(float var) { mEndRadiusVar = var; }
    const float&    getEndRadiusVar() const { return mEndRadiusVar; }
    
    void            setAngularSpeed(float speed) { mAngularSpeed = speed; }
    const float&    getAngularSpeed() const { return mAngularSpeed; }
    
    void            setAngularSpeedVar(float var) { mAngularSpeedVar = var; }
    const float&    getAngularSpeedVar() const { return mAngularSpeedVar; }
    
    void            setEmitterModeRadius(const emitter_mode_radius_t& mode);
    emitter_mode_radius_t* getEmitterModeRadius() { return &mModeRadius; }
    
    // mode both
    void            setStartSize(float size) { mStartSize = size; }
    const float&    getStartSize() const { return mStartSize; }

    void            setStartSizeVar(float var) { mStartSizeVar = var; }
    const float&    getStartSizeVar() const { return mStartSizeVar; }

    void            setEndSize(float size) { mEndSize = size; }
    const float&    getEndSize() const { return mEndSize; }

    void            setEndSizeVar(float var) { mEndSizeVar= var; }
    const float&    getEndSizeVar() const { return mEndSizeVar; }

    void            setStartColor(const color4f_t& color) { mStartColor = color; }
    color4f_t       getStartColor() { return mStartColor; }

    void            setStartColorVar(const color4f_t& var) { mStartColorVar = var; }
    color4f_t       getStartColorVar() { return mStartColorVar; }

    void            setEndColor(const color4f_t& color) { mEndColor = color; }
    color4f_t       getEndColor() { return mEndColor; }

    void            setEndColorVar(const color4f_t& var) { mEndColorVar = var; }
    color4f_t       getEndColorVar() { return mEndColorVar; }

    void            setStartSpin(float spin) { mStartSpin = spin; }
    const float&    getStartSpin() const { return mStartSpin; }
    
    void            setStartSpinVar(float var) { mStartSpinVar = var; }
    const float&    getStartSpinVar() const { return mStartSpinVar; }

    void            setEndSpin(float spin) { mEndSpin = spin; }
    const float&    getEndSpin() const { return mEndSpin; }

    void            setEndSpinVar(float var) { mEndSpinVar = var; }
    const float&    getEndSpinVar() const { return mEndSpinVar; }

private:
    void            _initParticle(particle_t* p);
    void            _addParticle();
    
    particle_t*     mParticles;
    int32_t         mTotalParticles;
    int32_t         mParticleCount;
    int32_t         mParticleIndex;
    
    float           mEmissionRate;
    float           mEmitCounter;
    
    bool            mRemoveWhenFinish;
    
    float           mDuration;
    float           mDurationVar;
    float           mCurrentDuration;
    float           mAngle;
    float           mAngleVar;
    
    bool            mIsActive;
    
    Point2f         mPositionVar;
    
    float           mLifeTime;
    float           mLifeTimeVar;
    
    // mode gravity
    Point2f         mGravity;
    float           mSpeed;
    float           mSpeedVar;
    float           mTangentialAccel;
    float           mTangentialAccelVar;
    float           mRadialAccel;
    float           mRadialAccelVar;
    emitter_mode_gravity_t  mModeGravity;
    
    // mode radius
    float           mStartRadius;
    float           mStartRadiusVar;
    float           mEndRadius;
    float           mEndRadiusVar;
    float           mAngularSpeed;
    float           mAngularSpeedVar;
    emitter_mode_radius_t   mModeRadius;
    
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
    float           mTexWidth;
    float           mTexHeight;
    
    EmitterMode     mMode;
    ParticlePositionType mParticlePositionType;
};

HGNAMESPACE_END

#endif // HGPARTICLEEMITTER_H_
