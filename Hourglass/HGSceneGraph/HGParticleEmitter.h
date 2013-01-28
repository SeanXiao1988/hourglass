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
    
    float       liveTime;
    
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

// Emitter class
class ParticleEmitter : public ISceneEntity
{
public:
    
    // ISceneEntity
    virtual void    update(const float dt);
	virtual void    render();
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_QUAD_ENTITY; };
    virtual uint32_t        getComponentName();
};

HGNAMESPACE_END

#endif // HGPARTICLEEMITTER_H_
