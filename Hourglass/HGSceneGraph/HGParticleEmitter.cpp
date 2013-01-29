/**
 *  @file    HGParticleEmitter.cpp
 *  @brief   Particle emitter implementation
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

#include "HGParticleEmitter.h"
#include "HGSceneNode.h"
#include "HGHash.h"

HGNAMESPACE_START

ParticleEmitter::ParticleEmitter()
    : mParticles(NULL)
    , mParticleCount(0)
    , mParticleIndex(0)
    , mEmissionRate(0)
    , mRemoveWhenFinish(false)
    , mDuration(0.0f)
    , mDurationVar(0.0f)
    , mCurrentDuration(0.0f)
    , mAngle(0.0f)
    , mAngleVar(0.0f)
    , mIsActive(false)
    , mSpeed(0.0f)
    , mSpeedVar(0.0f)
    , mTangentialAccel(0.0f)
    , mTangentialAccelVar(0.0f)
    , mRadialAccel(0.0f)
    , mRadialAccelVar(0.0f)
    , mStartRadius(0.0f)
    , mStartRadiusVar(0.0f)
    , mEndRadius(0.0f)
    , mEndRadiusVar(0.0f)
    , mAngularSpeed(0.0f)
    , mAngularSpeedVar(0.0f)
    , mStartSize(0.0f)
    , mStartSizeVar(0.0f)
    , mEndSize(0.0f)
    , mEndSizeVar(0.0f)
    , mStartSpin(0.0f)
    , mStartSpinVar(0.0f)
    , mEndSpin(0.0f)
    , mEndSpinVar(0.0f)
    , mMode(EMITTER_GRAVITY)
    , mParticlePositionType(PPositionTypeFree)
{
    memset(&mStartColor, 0, sizeof(color4f_t));
    memset(&mStartColorVar, 0, sizeof(color4f_t));
    memset(&mEndColor, 0, sizeof(color4f_t));
    memset(&mEndColorVar, 0, sizeof(color4f_t));
    memset(&mModeGravity, 0, sizeof(emitter_mode_gravity_t));
    memset(&mModeRadius, 0, sizeof(emitter_mode_radius_t));
}

ParticleEmitter::~ParticleEmitter()
{
    if (mParticles != NULL)
        delete[] mParticles;
    
    mParticles = NULL;
}

// composite

void ParticleEmitter::deInitialize()
{
    
}

EventResult ParticleEmitter::handleEvent(const Event &event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t ParticleEmitter::getComponentName()
{
    return Hash("ParticleEmitter");
}

// Scene Entity
void ParticleEmitter::update(const float dt)
{
    
}

void ParticleEmitter::render()
{
    
}

// private methods
void ParticleEmitter::_initParticle(particle_t *p)
{
    // timeToLive
    p->duration = mDuration + mDurationVar + RANDOM_MINUS1_1();
    p->duration = MAX(0.0f, p->duration);
    
    // position
    p->pos.x = mPositionVar.x + RANDOM_MINUS1_1();
    p->pos.y = mPositionVar.y + RANDOM_MINUS1_1();
    
    // Color
    color4f_t start;
    start.r = CLAMP(mStartColor.r + mStartColorVar.r * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    start.g = CLAMP(mStartColor.g + mStartColorVar.g * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    start.b = CLAMP(mStartColor.b + mStartColorVar.b * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    start.a = CLAMP(mStartColor.a + mStartColorVar.a * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    
    color4f_t end;
    end.r = CLAMP(mEndColor.r + mEndColorVar.r * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    end.g = CLAMP(mEndColor.g + mEndColorVar.g * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    end.b = CLAMP(mEndColor.b + mEndColorVar.b * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    end.a = CLAMP(mEndColor.a + mEndColorVar.a * RANDOM_MINUS1_1(), 0.0f, 1.0f);
    
    p->color = start;
    p->colorDelta.r = (end.r - start.r) / p->duration;
    p->colorDelta.g = (end.g - start.g) / p->duration;
    p->colorDelta.b = (end.b - start.b) / p->duration;
    p->colorDelta.a = (end.a - start.a) / p->duration;
    
    // size
    float sizeStart = mStartSize + mStartSizeVar * RANDOM_MINUS1_1();
    sizeStart = MAX(0.0f, sizeStart);
    
    p->size = sizeStart;
    
    if (mEndSize == PARTICLE_SIZE_START_EQUALS_END)
    {
        p->sizeDelta = 0.0f;
    }
    else
    {
        float sizeEnd = mEndSize + mEndSizeVar * RANDOM_MINUS1_1();
        sizeEnd = MAX(0.0f, sizeEnd);
        p->sizeDelta = (sizeEnd - sizeStart) / p->duration;
    }
    
    // rotation
    float angleStart = mStartSpin + mStartSpinVar * RANDOM_MINUS1_1();
    float angleEnd = mEndSpin + mEndSpinVar * RANDOM_MINUS1_1();
    p->rotation = angleStart;
    p->rotationDelta = (angleEnd - angleStart) / p->duration;
    
    // position
    if (mParticlePositionType == PPositionTypeFree)
    {
        p->startPos = Point2f(mSceneNode->getWorldX(), mSceneNode->getWorldY());
    }
    else
    {
        p->startPos = Point2f(0.0f, 0.0f);
    }
    
    // direction
    float angle = DEGREES_TO_RADIANS( mAngle + mAngleVar * RANDOM_MINUS1_1() );
    
    // Mode Gravity
    if (mMode == EMITTER_GRAVITY)
    {
        Point2f v(cosf(angle), sinf(angle));
        float s = mModeGravity.speed + mModeGravity.speedVar * RANDOM_MINUS1_1();
        
        // direction
        p->modeGravity.dir = v * s;
        
        // radial accel
        p->modeGravity.radialAccel = mModeGravity.radialAccel + mModeGravity.radialAccelVar * RANDOM_MINUS1_1();
        
        // tangential accel
        p->modeGravity.tangentialAccel = mModeGravity.tangentialAccel + mModeGravity.tangentialAccelVar * RANDOM_MINUS1_1();
    }
    // Mode Radius
    else
    {
        // Set the default diameter of the particle from the source position
        float radiusStart = mModeRadius.startRadius + mModeRadius.startRadiusVar * RANDOM_MINUS1_1();
        float radiusEnd = mModeRadius.endRadius + mModeRadius.endRadiusVar * RANDOM_MINUS1_1();
        
        p->modeRadius.radius = radiusStart;
        
        if (mModeRadius.endRadius == PARTICLE_RADIUS_START_EQUALS_END)
        {
            p->modeRadius.radiusDelta = 0.0f;
        }
        else
        {
            p->modeRadius.radiusDelta = (radiusEnd - radiusStart) / p->duration;
        }
        
        p->modeRadius.angle = angle;
        p->modeRadius.angularSpeed = DEGREES_TO_RADIANS(mModeRadius.angularSpeed + mModeRadius.angularSpeedVar * RANDOM_MINUS1_1());
    }
}

HGNAMESPACE_END
