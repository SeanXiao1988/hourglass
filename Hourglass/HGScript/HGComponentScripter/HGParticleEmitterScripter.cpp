/**
 *  @file    HGParticleEmitterScripter.cpp
 *  @brief   ParticleEmitter script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/30
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

#include "HGParticleEmitterScripter.h"
#include "HGParticleEmitter.h"
#include "HGCompositeScripter.h"
#include "HGGraphicScripter.h"
#include "HGISceneEntityScripter.h"

#define PARTICLEEMITTER_METATABLE   "ParticleEmitterMetatable"
#define PARTICLEEMITTER_LUA_NAME    "ParticleEmitter"

HGNAMESPACE_START

ParticleEmitter* particleemitter_check(lua_State* L, int idx)
{
    return *static_cast<ParticleEmitter **>(luaL_checkudata(L, idx, PARTICLEEMITTER_METATABLE));
}

// push a ParticleEmitter to Lua
int particleemitter_push(lua_State* L, ParticleEmitter* emitter)
{
    int ret = 0;
    
    if (emitter != NULL)
    {
        ParticleEmitter** udata = static_cast<ParticleEmitter **>(lua_newuserdata(L, sizeof(ParticleEmitter *)));
        *udata = emitter;
        luaL_getmetatable(L, PARTICLEEMITTER_METATABLE);
        lua_setmetatable(L, -2);
        
        ret = 1;
    }
    
    return ret;
}

// emitter mode
static emitter_mode_gravity_t emitter_mode_gravity_check(lua_State* L, int idx)
{
    emitter_mode_gravity_t mode;
    
    if (lua_istable(L, idx))
    {
        lua_pushstring(L, "gravity");
        lua_gettable(L, idx);
        mode.gravity = point2f_check(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "speed");
        lua_gettable(L, idx);
        mode.speed = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "speedVar");
        lua_gettable(L, idx);
        mode.speedVar = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "tangentialAccel");
        lua_gettable(L, idx);
        mode.tangentialAccel = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "tangentialAccelVar");
        lua_gettable(L, idx);
        mode.tangentialAccelVar = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "radialAccel");
        lua_gettable(L, idx);
        mode.radialAccel = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        
        lua_pushstring(L, "radialAccelVar");
        lua_gettable(L, idx);
        mode.radialAccelVar = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }
    
    return mode;
}

static int emitter_mode_gravity_push(lua_State* L, emitter_mode_gravity_t* mode)
{
    if (mode == NULL)
        return 0;
    
    lua_newtable(L);
    lua_pushstring(L, "gravity");
    point2f_push(L, &mode->gravity);
    lua_settable(L, -3);
    
    lua_pushstring(L, "speed");
    lua_pushnumber(L, mode->speed);
    lua_settable(L, -3);
    
    lua_pushstring(L, "speedVar");
    lua_pushnumber(L, mode->speedVar);
    lua_settable(L, -3);
    
    lua_pushstring(L, "tangentialAccel");
    lua_pushnumber(L, mode->tangentialAccel);
    lua_settable(L, -3);
    
    lua_pushstring(L, "tangentialAccelVar");
    lua_pushnumber(L, mode->tangentialAccelVar);
    lua_settable(L, -3);
    
    lua_pushstring(L, "radialAccel");
    lua_pushnumber(L, mode->radialAccel);
    lua_settable(L, -3);
    
    lua_pushstring(L, "radialAccelVar");
    lua_pushnumber(L, mode->radialAccelVar);
    lua_settable(L, -3);
    
    return 1;
}

static emitter_mode_radius_t emitter_mode_radius_check(lua_State* L, int idx)
{
    emitter_mode_radius_t mode;
    memset(&mode, 0, sizeof(emitter_mode_radius_t));
    
    if (lua_istable(L, idx))
    {
        lua_pushstring(L, "startRadius");
        lua_gettable(L, idx);
        mode.startRadius = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "startRadiusVar");
        lua_gettable(L, idx);
        mode.startRadiusVar = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "endRadius");
        lua_gettable(L, idx);
        mode.endRadius = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "endRadiusVar");
        lua_gettable(L, idx);
        mode.endRadiusVar = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "angularSpeed");
        lua_gettable(L, idx);
        mode.angularSpeed = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "angularSpeedVar");
        lua_gettable(L, idx);
        mode.angularSpeedVar = (float)luaL_checknumber(L, -1);
    }
    
    return mode;
}

static int emitter_mode_radius_push(lua_State* L, emitter_mode_radius_t* mode)
{
    if (mode == NULL)
        return 0;

    lua_newtable(L);
    
    lua_pushstring(L, "startRadius");
    lua_pushnumber(L, mode->startRadius);
    lua_settable(L, -3);
    
    lua_pushstring(L, "startRadiusVar");
    lua_pushnumber(L, mode->startRadiusVar);
    lua_settable(L, -3);
    
    lua_pushstring(L, "endRadius");
    lua_pushnumber(L, mode->endRadius);
    lua_settable(L, -3);
    
    lua_pushstring(L, "endRadiusVar");
    lua_pushnumber(L, mode->endRadiusVar);
    lua_settable(L, -3);
    
    lua_pushstring(L, "angularSpeed");
    lua_pushnumber(L, mode->angularSpeed);
    lua_settable(L, -3);
    
    lua_pushstring(L, "angularSpeedVar");
    lua_pushnumber(L, mode->angularSpeedVar);
    lua_settable(L, -3);
    
    return 1;
}

// member methods
static int particleemitter_fire_emitter(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        emitter->fireEmitter();
    
    return 0;
}

static int particleemitter_stop_emitter(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        emitter->stopEmitter();
    
    return 0;
}

static int particleemitter_set_emitter_mode(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        EmitterMode mode = (EmitterMode)luaL_checkinteger(L, 2);
        emitter->setEmitterMode(mode);
    }
    
    return 0;
}

static int particleemitter_get_emitter_mode(lua_State* L)
{
    EmitterMode mode = EMITTER_GRAVITY;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        mode = emitter->getEmitterMode();

    lua_pushinteger(L, (int)mode);
    
    return 1;
}

static int particleemitter_set_particle_position_type(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        ParticlePositionType type = (ParticlePositionType)luaL_checkinteger(L, 2);
        emitter->setParticlePositionType(type);
    }
    
    return 0;
}

static int particleemitter_get_particle_position_type(lua_State* L)
{
    ParticlePositionType type = PPositionTypeFree;
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        type = emitter->getParticlePositionType();
    
    lua_pushinteger(L, (int)type);
    
    return 1;
}

static int particleemitter_set_texture(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    GLuint tex = (GLuint)luaL_checkunsigned(L, 2);
    
    if (emitter != NULL)
        emitter->setTexture(tex);
    
    return 0;
}

static int particleemitter_get_texture(lua_State* L)
{
    GLuint tex = 0;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        tex = emitter->getTexture();
    
    lua_pushunsigned(L, tex);
    
    return 1;
}

static int particleemitter_set_texture_rect(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        float x = 0.0f;
        float y = 0.0f;
        float w = 0.0f;
        float h = 0.0f;
        
        x = (float)luaL_checknumber(L, 2);
        y = (float)luaL_checknumber(L, 3);
        w = (float)luaL_checknumber(L, 4);
        h = (float)luaL_checknumber(L, 5);
        
        emitter->setTextureRect(x, y, w, h);
    }
    
    return 0;
}

static int particleemitter_set_total_particles(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        int32_t totoalParticles = (int32_t)luaL_checkunsigned(L, 2);
        emitter->setTotalParticles(totoalParticles);
    }
    
    return 0;
}

static int particleemitter_get_total_particles(lua_State* L)
{
    int32_t totalParticles = 0;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        totalParticles = emitter->getTotalParticles();
    
    lua_pushunsigned(L, totalParticles);
    
    return 1;
}

static int particleemitter_get_particle_count(lua_State* L)
{
    int32_t particleCount = 0;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        particleCount = emitter->getParticleCount();
    
    lua_pushinteger(L, particleCount);
    
    return 1;
}

static int particleemitter_set_emission_rate(lua_State* L)
{
    float rate = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        rate = (float)luaL_checknumber(L, 2);
        emitter->setEmissionRate(rate);
    }
    
    return 0;
}

static int particleemitter_get_emission_rate(lua_State* L)
{
    float rate = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        rate = emitter->getEmissionRate();
    
    lua_pushnumber(L, rate);
    
    return 1;
}

static int particleemitter_set_remove_when_finish(lua_State* L)
{
    bool remove = false;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        remove = luaH_checkboolean(L, 2);
        emitter->setRemoveWhenFinish(remove);
    }
    
    return 0;
}

static int particleemitter_is_remove_when_finish(lua_State* L)
{
    bool remove = false;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        remove = emitter->isRemoveWhenFinish();
    
    luaH_pushboolean(L, remove);
    
    return 1;
}

static int particleemitter_set_duration(lua_State* L)
{
    float duration = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        duration = (float)luaL_checknumber(L, 2);
        emitter->setDuration(duration);
    }
    
    return 0;
}

static int particleemitter_get_duration(lua_State* L)
{
    float duration = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        duration = emitter->getDuration();
    
    lua_pushnumber(L, duration);
    
    return 1;
}

static int particleemitter_set_lifetime(lua_State* L)
{
    float lifeTime = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        lifeTime = (float)luaL_checknumber(L, 2);
        emitter->setLifeTime(lifeTime);
    }
    
    return 0;
}

static int particleemitter_get_lifetime(lua_State* L)
{
    float lifeTime = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        lifeTime = emitter->getLifeTime();
    
    lua_pushnumber(L, lifeTime);
    
    return 1;
}

static int particleemitter_set_lifetime_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setLifeTimeVar(var);
    }
    
    return 0;
}

static int particleemitter_get_lifetime_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getLifeTimeVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_duration_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setDurationVar(var);
    }
    
    return 0;
}

static int particleemitter_get_duration_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getDurationVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_angle(lua_State* L)
{
    float angle = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        angle = (float)luaL_checknumber(L, 2);
        emitter->setAngle(angle);
    }
    
    return 0;
}

static int particleemitter_get_angle(lua_State* L)
{
    float angle = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        angle = emitter->getAngle();
    
    lua_pushnumber(L, angle);
    
    return 1;
}

static int particleemitter_set_angle_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setAngleVar(var);
    }
    
    return 0;
}

static int particleemitter_get_angle_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getAngleVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_position_var(lua_State* L)
{
    Point2f var(0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = point2f_check(L, -1);
        emitter->setPositionVar(var);
    }
    
    return 0;
}

static int particleemitter_get_position_var(lua_State* L)
{
    Point2f var(0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getPositionVar();

    point2f_push(L, &var);
    
    return 1;
}

static int particleemitter_set_active(lua_State* L)
{
    bool active = false;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        active = luaH_checkboolean(L, 2);
        emitter->setActive(active);
    }
    
    return 0;
}

static int particleemitter_is_active(lua_State* L)
{
    bool active = false;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        active = emitter->isActive();
    
    luaH_pushboolean(L, active);
    
    return 1;
}

static int particleemitter_set_gravity(lua_State* L)
{
    Point2f gravity(0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        gravity = point2f_check(L, -1);
        emitter->setGravity(gravity);
    }
    
    return 0;
}

static int particleemitter_get_gravity(lua_State* L)
{
    Point2f gravity(0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        gravity = emitter->getGravity();
    
    point2f_push(L, &gravity);
    
    return 0;
}

static int particleemitter_set_speed(lua_State* L)
{
    float speed = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        speed = (float)luaL_checknumber(L, 2);
        emitter->setSpeed(speed);
    }
    
    return 0;
}

static int particleemitter_get_speed(lua_State* L)
{
    float speed = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        speed = emitter->getSpeed();
    
    lua_pushnumber(L, speed);
    
    return 1;
}

static int particleemitter_set_speed_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setSpeedVar(var);
    }
    
    return 0;
}

static int particleemitter_get_speed_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getSpeedVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_tangential_accel(lua_State* L)
{
    float accel = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        accel = (float)luaL_checknumber(L, 2);
        emitter->setTangentialAccel(accel);
    }
    
    return 0;
}

static int particleemitter_get_tangential_accel(lua_State* L)
{
    float accel = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        accel = emitter->getTangentialAccel();
    
    lua_pushnumber(L, accel);
    
    return 1;
}

static int particleemitter_set_tangential_accel_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setTangentialAccelVar(var);
    }
    
    return 0;
}

static int particleemitter_get_tangential_accel_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getTangentialAccelVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_radial_accel(lua_State* L)
{
    float accel = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        accel = (float)luaL_checknumber(L, 2);
        emitter->setRadialAccel(accel);
    }
    
    return 0;
}

static int particleemitter_get_radial_accel(lua_State* L)
{
    float accel = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        accel = emitter->getRadialAccel();
    
    lua_pushnumber(L, accel);
    
    return 1;
}

static int particleemitter_set_radial_accel_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setRadialAccelVar(var);
    }
    
    return 0;
}

static int particleemitter_get_radial_accel_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getRadialAccelVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_emitter_mode_gravity(lua_State* L)
{
    emitter_mode_gravity_t mode;
    memset(&mode, 0, sizeof(emitter_mode_gravity_t));
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        mode = emitter_mode_gravity_check(L, 2);
        emitter->setEmitterModeGravity(mode);
    }
    
    return 0;
}

static int particleemitter_get_emitter_mode_gravity(lua_State* L)
{
    emitter_mode_gravity_t* mode = NULL;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        mode = emitter->getEmitterModeGravity();
    
    emitter_mode_gravity_push(L, mode);
    
    return 1;
}

static int particleemitter_set_emitter_mode_radius(lua_State* L)
{
    emitter_mode_radius_t mode;
    memset(&mode, 0, sizeof(emitter_mode_radius_t));
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        mode = emitter_mode_radius_check(L, 2);
        emitter->setEmitterModeRadius(mode);
    }
    
    return 0;
}

static int particleemitter_get_emitter_mode_radius(lua_State* L)
{
    emitter_mode_radius_t* mode = NULL;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        mode = emitter->getEmitterModeRadius();
    
    emitter_mode_radius_push(L, mode);
    
    return 1;
}

static int particleemitter_set_start_radius(lua_State* L)
{
    float radius = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        radius = (float)luaL_checknumber(L, 2);
        emitter->setStartRadius(radius);
    }
    
    return 0;
}

static int particleemitter_get_start_radius(lua_State* L)
{
    float radius = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        radius = emitter->getStartRadius();
    
    lua_pushnumber(L, radius);
    
    return 1;
}

static int particleemitter_set_start_radius_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setStartRadiusVar(var);
    }
    
    return 0;
}

static int particleemitter_get_start_radius_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getStartRadiusVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_end_radius(lua_State* L)
{
    float radius = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        radius = (float)luaL_checknumber(L, 2);
        emitter->setEndRadius(radius);
    }
    
    return 0;
}

static int particleemitter_get_end_radius(lua_State* L)
{
    float radius = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        radius = emitter->getEndRadius();
    
    lua_pushnumber(L, radius);
    
    return 1;
}

static int particleemitter_set_end_radius_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setEndRadiusVar(var);
    }
    
    return 0;
}

static int particleemitter_get_end_radius_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getEndRadiusVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_angular_speed(lua_State* L)
{
    float speed = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        speed = (float)luaL_checknumber(L, 2);
        emitter->setAngularSpeed(speed);
    }
    
    return 0;
}

static int particleemitter_get_angular_speed(lua_State* L)
{
    float speed = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        speed = emitter->getAngularSpeed();
    
    lua_pushnumber(L, speed);
    
    return 1;
}

static int particleemitter_set_angular_speed_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setAngularSpeedVar(var);
    }
    
    return 0;
}

static int particleemitter_get_angular_speed_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getAngularSpeedVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_start_size(lua_State* L)
{
    float size = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        size = (float)luaL_checknumber(L, 2);
        emitter->setStartSize(size);
    }
    
    return 0;
}

static int particleemitter_get_start_size(lua_State* L)
{
    float size = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        size = emitter->getStartSize();
    
    lua_pushnumber(L, size);
    
    return 1;
}

static int particleemitter_set_start_size_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setStartSizeVar(var);
    }
    
    return 0;
}

static int particleemitter_get_start_size_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getStartSizeVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_end_size(lua_State* L)
{
    float size = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        size = (float)luaL_checknumber(L, 2);
        emitter->setEndSize(size);
    }
    
    return 0;
}

static int particleemitter_get_end_size(lua_State* L)
{
    float size = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        size = emitter->getEndSize();
    
    lua_pushnumber(L, size);
    
    return 1;
}

static int particleemitter_set_end_size_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setEndSizeVar(var);
    }
    
    return 0;
}

static int particleemitter_get_end_size_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getEndSizeVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_start_color(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        color4f_t color = color4_check(L, -1);
        emitter->setStartColor(color);
    }
    
    return 0;
}

static int particleemitter_get_start_color(lua_State* L)
{
    color4f_t color = color4f(0.0f, 0.0f, 0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        color = emitter->getStartColor();
    
    color4f_push(L, &color);
    
    return 1;
}

static int particleemitter_set_start_color_var(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        color4f_t var = color4_check(L, -1);
        emitter->setStartColorVar(var);
    }
    
    return 0;
}

static int particleemitter_get_start_color_var(lua_State* L)
{
    color4f_t var = color4f(0.0f, 0.0f, 0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getStartColorVar();
    
    color4f_push(L, &var);
    
    return 1;
}

static int particleemitter_set_end_color(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        color4f_t color = color4_check(L, -1);
        emitter->setEndColor(color);
    }
    
    return 0;
}

static int particleemitter_get_end_color(lua_State* L)
{
    color4f_t color = color4f(0.0f, 0.0f, 0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        color = emitter->getEndColor();
    
    color4f_push(L, &color);
    
    return 1;
}

static int particleemitter_set_end_color_var(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        color4f_t var = color4_check(L, -1);
        emitter->setEndColorVar(var);
    }
    
    return 0;
}

static int particleemitter_get_end_color_var(lua_State* L)
{
    color4f_t var = color4f(0.0f, 0.0f, 0.0f, 0.0f);
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getEndColorVar();
    
    color4f_push(L, &var);

    return 1;
}

static int particleemitter_set_start_spin(lua_State* L)
{
    float spin = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        spin = (float)luaL_checknumber(L, 2);
        emitter->setStartSpin(spin);
    }
    
    return 0;
}

static int particleemitter_get_start_spin(lua_State* L)
{
    float spin = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        spin = emitter->getStartSpin();
    
    lua_pushnumber(L, spin);
    
    return 1;
}

static int particleemitter_set_start_spin_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setStartSpinVar(var);
    }
    
    return 0;
}

static int particleemitter_get_start_spin_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getStartSpinVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

static int particleemitter_set_end_spin(lua_State* L)
{
    float spin = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        spin = (float)luaL_checknumber(L, 2);
        emitter->setEndSpin(spin);
    }
    
    return 0;
}

static int particleemitter_get_end_spin(lua_State* L)
{
    float spin = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        spin = emitter->getEndSpin();
    
    lua_pushnumber(L, spin);
    
    return 1;
}

static int particleemitter_set_end_spin_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
    {
        var = (float)luaL_checknumber(L, 2);
        emitter->setEndSpinVar(var);
    }
    
    return 0;
}

static int particleemitter_get_end_spin_var(lua_State* L)
{
    float var = 0.0f;
    
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    if (emitter != NULL)
        var = emitter->getEndSpinVar();
    
    lua_pushnumber(L, var);
    
    return 1;
}

// new, delete
static int particleemitter_new(lua_State* L)
{
    int ret = 0;
    ParticleEmitter* emitter = new ParticleEmitter();
    if (emitter != NULL)
        ret = particleemitter_push(L, emitter);
    
    return ret;
}

static int particleemitter_delete(lua_State* L)
{
    ParticleEmitter* emitter = particleemitter_check(L, 1);
    delete emitter;
    
    return 0;
}

luaL_Reg sParticleEmitterRegs[] =
{
    { "fireEmitter",                particleemitter_fire_emitter },
    { "stopEmitter",                particleemitter_stop_emitter },
    { "setEmitterMode",             particleemitter_set_emitter_mode },
    { "getEmitterMode",             particleemitter_get_emitter_mode },
    { "setParticlePositionType",    particleemitter_set_particle_position_type },
    { "getParticlePositionType",    particleemitter_get_particle_position_type },
    { "setTexture",                 particleemitter_set_texture },
    { "getTexture",                 particleemitter_get_texture },
    { "setTextureRect",             particleemitter_set_texture_rect },
    { "setTotalParticles",          particleemitter_set_total_particles },
    { "getTotalParticles",          particleemitter_get_total_particles },
    { "getParticleCount",           particleemitter_get_particle_count },
    { "setEmissionRate",            particleemitter_set_emission_rate },
    { "getEmissionRate",            particleemitter_get_emission_rate },
    { "setRemoveWhen_finish",       particleemitter_set_remove_when_finish },
    { "isRemoveWhen_finish",        particleemitter_is_remove_when_finish },
    { "setDuration",                particleemitter_set_duration },
    { "getDuration",                particleemitter_get_duration },
    { "setLifeTime",                particleemitter_set_lifetime },
    { "getLifeTime",                particleemitter_get_lifetime },
    { "setLifeTimeVar",             particleemitter_set_lifetime_var },
    { "getLifeTimeVar",             particleemitter_get_lifetime_var },
    { "setDurationVar",             particleemitter_set_duration_var },
    { "getDurationVar",             particleemitter_get_duration_var },
    { "setAngle",                   particleemitter_set_angle },
    { "getAngle",                   particleemitter_get_angle },
    { "setAngleVar",                particleemitter_set_angle_var },
    { "getAngleVar",                particleemitter_get_angle_var },
    { "setPositionVar",             particleemitter_set_position_var },
    { "getPositionVar",             particleemitter_get_position_var },
    { "setActive",                  particleemitter_set_active },
    { "isActive",                   particleemitter_is_active },
    { "setGravity",                 particleemitter_set_gravity },
    { "getGravity",                 particleemitter_set_gravity },
    { "setSpeed",                   particleemitter_set_speed },
    { "getSpeed",                   particleemitter_get_speed },
    { "setSpeedVar",                particleemitter_set_speed_var },
    { "getSpeedVar",                particleemitter_get_speed_var },
    { "setTangentialAccel",         particleemitter_set_tangential_accel },
    { "getTangentialAccel",         particleemitter_get_tangential_accel },
    { "setTangentialAccelVar",      particleemitter_set_tangential_accel_var },
    { "getTangentialAccelVar",      particleemitter_get_tangential_accel_var },
    { "setRadialAccel",             particleemitter_set_radial_accel },
    { "getRadialAccel",             particleemitter_get_radial_accel },
    { "setRadialAccel_var",         particleemitter_set_radial_accel_var },
    { "getRadialAccel_var",         particleemitter_get_radial_accel_var },
    { "setEmitterModeGravity",      particleemitter_set_emitter_mode_gravity },
    { "getEmitterModeGravity",      particleemitter_get_emitter_mode_gravity },
    { "setEmitterModeRadius",       particleemitter_set_emitter_mode_radius },
    { "getEmitterModeRadius",       particleemitter_get_emitter_mode_radius },
    { "setStartRadius",             particleemitter_set_start_radius },
    { "getStartRadius",             particleemitter_get_start_radius },
    { "setStartRadiusVar",          particleemitter_set_start_radius_var },
    { "getStartRadiusVar",          particleemitter_get_start_radius_var },
    { "setEndRadius",               particleemitter_set_end_radius },
    { "getEndRadius",               particleemitter_get_end_radius },
    { "setEndRadiusVar",            particleemitter_set_end_radius_var },
    { "getEndRadiusVar",            particleemitter_get_end_radius_var },
    { "setAngularSpeed",            particleemitter_set_angular_speed },
    { "getAngularSpeed",            particleemitter_get_angular_speed },
    { "setAngularSpeedVar",         particleemitter_set_angular_speed_var },
    { "getAngularSpeedVar",         particleemitter_get_angular_speed_var },
    { "setStartSize",               particleemitter_set_start_size },
    { "getStartSize",               particleemitter_get_start_size },
    { "setStartSizeVar",            particleemitter_set_start_size_var },
    { "getStartSizeVar",            particleemitter_get_start_size_var },
    { "setEndSize",                 particleemitter_set_end_size },
    { "getEndSize",                 particleemitter_get_end_size },
    { "setEndSizeVar",              particleemitter_set_end_size_var },
    { "getEndSizeVar",              particleemitter_get_end_size_var },
    { "setStartColor",              particleemitter_set_start_color },
    { "getStartColor",              particleemitter_get_start_color },
    { "setStartColorVar",           particleemitter_set_start_color_var },
    { "getStartColorVar",           particleemitter_get_start_color_var },
    { "setEndColor",                particleemitter_set_end_color },
    { "getEndColor",                particleemitter_get_end_color },
    { "setEndColorVar",             particleemitter_set_end_color_var },
    { "getEndColorVar",             particleemitter_get_end_color_var },
    { "setStartSpin",               particleemitter_set_start_spin },
    { "getStartSpin",               particleemitter_get_start_spin },
    { "setStartSpinVar",            particleemitter_set_start_spin_var },
    { "getStartSpinVar",            particleemitter_get_start_spin_var },
    { "setEndSpin",                 particleemitter_set_end_spin },
    { "getEndSpin",                 particleemitter_get_end_spin },
    { "setEndSpinVar",              particleemitter_set_end_spin_var },
    { "getEndSpinVar",              particleemitter_get_end_spin_var },
    { NULL, NULL }
};

void ScriptRegisterParticleEmitter(lua_State* L)
{
    luaL_newmetatable(L, PARTICLEEMITTER_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    luaL_setfuncs(L, sIComponentRegs, 0);
    luaL_setfuncs(L, sISceneEntityRegs, 0);
    luaL_setfuncs(L, sParticleEmitterRegs, 0);
    
    lua_pop(L, 1);
    
    // register ParticleEmitter
    lua_newtable(L);
    
    luaH_setfunc2table(L, "new", particleemitter_new);
    luaH_setfunc2table(L, "delete", particleemitter_delete);
    
    lua_setglobal(L, PARTICLEEMITTER_LUA_NAME);
    
    // constants
    lua_pushinteger(L, EMITTER_GRAVITY);
    lua_setglobal(L, "EMITTER_GRAVITY");
    
    lua_pushinteger(L, EMITTER_RADIUS);
    lua_setglobal(L, "EMITTER_RADIUS");
    
    lua_pushinteger(L, PPositionTypeFree);
    lua_setglobal(L, "PPositionTypeFree");
    
    lua_pushinteger(L, PPositionTypeGroup);
    lua_setglobal(L, "PPositionTypeGroup");
    
    lua_pushinteger(L, PPositionTypeRelative);
    lua_setglobal(L, "PPositionTypeRelative");
}

HGNAMESPACE_END
