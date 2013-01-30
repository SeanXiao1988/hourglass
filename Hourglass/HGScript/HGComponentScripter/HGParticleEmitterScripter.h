/**
 *  @file    HGParticleEmitterScripter.h
 *  @brief   ParticleEmitter script adapter header
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

#ifndef HGPARTICLEEMITTERSCRIPTER_H_
#define HGPARTICLEEMITTERSCRIPTER_H_

#include "HGScriptDef.h"

HGNAMESPACE_START

class ParticleEmitter;

extern luaL_Reg sParticleEmitterRegs[];

extern ParticleEmitter* particleemitter_check(lua_State* L, int idx);
extern int particleemitter_push(lua_State* L, ParticleEmitter* emitter);
extern void ScriptRegisterParticleEmitter(lua_State* L);

HGNAMESPACE_END

#endif // HGPARTICLEEMITTERSCRIPTER_H_
