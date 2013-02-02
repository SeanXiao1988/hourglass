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
        
        lua_pushstring(L, "speed");
        lua_gettable(L, idx);
        mode.speed = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "speedVar");
        lua_gettable(L, idx);
        mode.speedVar = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "tangentialAccel");
        lua_gettable(L, idx);
        mode.tangentialAccel = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "tangentialAccelVar");
        lua_gettable(L, idx);
        mode.tangentialAccelVar = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "radialAccel");
        lua_gettable(L, idx);
        mode.radialAccel = (float)luaL_checknumber(L, -1);
        
        lua_pushstring(L, "radialAccelVar");
        lua_gettable(L, idx);
        mode.radialAccelVar = (float)luaL_checknumber(L, -1);
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

HGNAMESPACE_END