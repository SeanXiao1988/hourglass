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

// member methods

HGNAMESPACE_END