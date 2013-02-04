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
#include "HGRender.h"
#include "HGHash.h"

HGNAMESPACE_START

ParticleEmitter::ParticleEmitter()
    : mParticles(NULL)
    , mTotalParticles(0)
    , mParticleCount(0)
    , mParticleIndex(0)
    , mEmissionRate(0.0f)
    , mEmitCounter(0.0f)
    , mRemoveWhenFinish(false)
    , mDuration(0.0f)
    , mDurationVar(0.0f)
    , mCurrentDuration(0.0f)
    , mAngle(0.0f)
    , mAngleVar(0.0f)
    , mIsActive(false)
    , mLifeTime(0.0f)
    , mLifeTimeVar(0.0f)
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
    , mTexWidth(0.0f)
    , mTexHeight(0.0f)
    , mMode(EMITTER_GRAVITY)
    , mParticlePositionType(PPositionTypeFree)
{
    memset(&mStartColor, 0, sizeof(color4f_t));
    memset(&mStartColorVar, 0, sizeof(color4f_t));
    memset(&mEndColor, 0, sizeof(color4f_t));
    memset(&mEndColorVar, 0, sizeof(color4f_t));
    memset(&mModeGravity, 0, sizeof(emitter_mode_gravity_t));
    memset(&mModeRadius, 0, sizeof(emitter_mode_radius_t));
    
    quad_set_default(&mQuad);
    mQuad.blend = BLEND_ALPHAADD;
}

ParticleEmitter::~ParticleEmitter()
{
    if (mParticles != NULL)
        delete[] mParticles;
    
    mParticles = NULL;
    
    if (mQuad.tex != 0)
    {
        RENDER.textureFree(mQuad.tex);
    }
    
    detachFromSceneNode();
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
    if (mParticles == NULL)
        return;
    
    if (mIsActive && mEmissionRate != 0.0f)
    {
        float rate = 1.0f / mEmissionRate;
        
        if (mParticleCount < mTotalParticles)
        {
            mEmitCounter += dt;
        }
        
        while (mParticleCount < mTotalParticles && mEmitCounter > rate)
        {
            _addParticle();
            mEmitCounter -= rate;
        }
        
        mCurrentDuration += dt;
        if (mDuration != -1.0f && mDuration < mCurrentDuration)
        {
            stopEmitter();
        }
    }
    
    mParticleIndex = 0;
    
    while (mParticleIndex < mParticleCount)
    {
        particle_t* p = &mParticles[mParticleIndex];
        
        p->lifeTime -= dt;
        if (p->lifeTime > 0.0f)
        {
            // mode gravity
            if (mMode == EMITTER_GRAVITY)
            {
                Point2f tmp, radial, tangential;
                
                // radial acceleration
                if (p->pos.x || p->pos.y)
                {
                    radial = p->pos;
                    radial.normalize();
                }
                tangential = radial;
                radial *= p->modeGravity.radialAccel;
                
                // tangential acceleration
                float newy = tangential.x;
                tangential.x = -tangential.y;
                tangential.y = newy;
                tangential *= p->modeGravity.tangentialAccel;
                
                // (gravity + radial + tangential) * dt
                tmp = radial + tangential + mModeGravity.gravity;
                tmp *= dt;
                p->modeGravity.dir += tmp;
                tmp = p->modeGravity.dir * dt;
                p->pos += tmp;
            }
            // mode radius
            else
            {
                // update the angle and radius of the particle
                p->modeRadius.angle += p->modeRadius.angularSpeed * dt;
                p->modeRadius.radius += p->modeRadius.radiusDelta * dt;
                
                p->pos.x = - cosf(p->modeRadius.angle) * p->modeRadius.radius;
                p->pos.y = - sinf(p->modeRadius.angle) * p->modeRadius.radius;
            }
            
            // color
            p->color.r += (p->colorDelta.r * dt);
            p->color.g += (p->colorDelta.g * dt);
            p->color.b += (p->colorDelta.b * dt);
            p->color.a += (p->colorDelta.a * dt);
            
            // size
            p->size += (p->sizeDelta * dt);
            p->size = MAX(0.0f, p->size);
            
            // angle
            p->rotation += (p->rotationDelta * dt);
            
            //
            mParticleIndex++;
        }
        else
        {
            // life < 0.0f
            if (mParticleIndex != mParticleCount-1)
                mParticles[mParticleIndex] = mParticles[mParticleCount-1];
            
            mParticleCount--;
            
            if (mParticleCount == 0 && mRemoveWhenFinish)
            {
                mSceneNode->detachEntity();
            }
        }
    }
}

void ParticleEmitter::render()
{
    uint8_t alpha = ((uint8_t)mSceneNode->getRenderAlpha());
    quad_set_alpha(&mQuad, alpha);
    
    for (int i = 0; i < mParticleCount; i++)
    {
        particle_t* p = &mParticles[i];
        
        glm::mat4 particleMat(1.0f);
    
        particleMat *= glm::translate(glm::mat4(1.0f), glm::vec3(p->pos.x + p->startPos.x, p->pos.y + p->startPos.y, 1.0f));
        particleMat *= glm::rotate(glm::mat4(1.0f), p->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::vec4 result = particleMat * glm::vec4(-p->size/2.0f, -p->size/2.0f, 1.0f, 1.0f);
        mQuad.v[0].x = result[0];
        mQuad.v[0].y = result[1];
        mQuad.v[0].z = result[2];
        
        result = particleMat * glm::vec4(p->size/2.0f, -p->size/2.0f, 1.0f, 1.0f);
        mQuad.v[1].x = result[0];
        mQuad.v[1].y = result[1];
        mQuad.v[1].z = result[2];
        
        result = particleMat * glm::vec4(p->size/2.0f, p->size/2.0f, 1.0f, 1.0f);
        mQuad.v[2].x = result[0];
        mQuad.v[2].y = result[1];
        mQuad.v[2].z = result[2];
        
        result = particleMat * glm::vec4(-p->size/2.0f, p->size/2.0f, 1.0f, 1.0f);
        mQuad.v[3].x = result[0];
        mQuad.v[3].y = result[1];
        mQuad.v[3].z = result[2];
        
        quad_set_color_4f(&mQuad, p->color);
        RENDER.renderQuad(&mQuad);
    }
}

// control
void ParticleEmitter::fireEmitter()
{
    mIsActive = true;
}

void ParticleEmitter::stopEmitter()
{
    mIsActive = false;
    mCurrentDuration = mDuration;
    mEmitCounter = 0.0f;
}

void ParticleEmitter::setTotalParticles(int32_t total)
{
    if (mParticles != NULL || total <= 0)
        delete[] mParticles;
    
    mParticles = NULL;
    
    mParticles = new particle_t[total];
    mTotalParticles = total;
}

void ParticleEmitter::setTextureRect(float x, float y, float w, float h)
{
    float tw = (float)RENDER.textureGetWidth(mQuad.tex);
    float th = (float)RENDER.textureGetHeight(mQuad.tex);
    mTexWidth = w;
    mTexHeight = h;
    quad_set_texture_rect(&mQuad, x, y, w, h, tw, th);
}

void ParticleEmitter::setDuration(float duration)
{
    mDuration = duration;
    if (duration > 0.0f)
    {
        mEmissionRate = mTotalParticles / mDuration;
    }
    else
    {
        mEmissionRate = 1.0f;
    }
}

void ParticleEmitter::setEmitterModeGravity(const emitter_mode_gravity_t &mode)
{
    memcpy(&mModeGravity, &mode, sizeof(emitter_mode_gravity_t));
}

void ParticleEmitter::setEmitterModeRadius(const emitter_mode_radius_t &mode)
{
    memcpy(&mModeRadius, &mode, sizeof(emitter_mode_radius_t));
}

// private methods
void ParticleEmitter::_initParticle(particle_t *p)
{
    // timeToLive
    p->lifeTime = mLifeTime + mLifeTimeVar + RANDOM_MINUS1_1();
    p->lifeTime = MAX(0.0f, p->lifeTime);
    
    // position
    p->pos.x = mPositionVar.x * RANDOM_MINUS1_1();
    p->pos.y = mPositionVar.y * RANDOM_MINUS1_1();
    
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
    p->colorDelta.r = (end.r - start.r) / p->lifeTime;
    p->colorDelta.g = (end.g - start.g) / p->lifeTime;
    p->colorDelta.b = (end.b - start.b) / p->lifeTime;
    p->colorDelta.a = (end.a - start.a) / p->lifeTime;
    
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
        p->sizeDelta = (sizeEnd - sizeStart) / p->lifeTime;
    }
    
    // rotation
    float angleStart = mStartSpin + mStartSpinVar * RANDOM_MINUS1_1();
    float angleEnd = mEndSpin + mEndSpinVar * RANDOM_MINUS1_1();
    p->rotation = angleStart;
    p->rotationDelta = (angleEnd - angleStart) / p->lifeTime;
    
    // position
    //p->startPos = Point2f(mSceneNode->getWorldX() - mTexWidth/2.0f, mSceneNode->getWorldY() - mTexHeight/2.0f);
    p->startPos = Point2f(mSceneNode->getWorldX(), mSceneNode->getWorldY());
    
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
            p->modeRadius.radiusDelta = (radiusEnd - radiusStart) / p->lifeTime;
        }
        
        p->modeRadius.angle = angle;
        p->modeRadius.angularSpeed = DEGREES_TO_RADIANS(mModeRadius.angularSpeed + mModeRadius.angularSpeedVar * RANDOM_MINUS1_1());
    }
}

void ParticleEmitter::_addParticle()
{
    if (mParticleCount >= mTotalParticles)
        return;
    
    particle_t* p = &mParticles[mParticleCount];
    _initParticle(p);
    mParticleCount++;
}

HGNAMESPACE_END
