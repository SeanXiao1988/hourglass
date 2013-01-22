/**
 *  @file    HGAudioManagerScripter.cpp
 *  @brief   AudioManager script adapter implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/22
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

#include "HGAudioManagerScripter.h"
#include "HGAudioManager.h"

#define AUDIOMANAGER_LUA_NAME "AUDIOMANAGER"

HGNAMESPACE_START

static int audiomanager_audio_load(lua_State* L)
{
    int32_t audioID = -1;
    
    BREAK_START;
    
    const char* filename = luaL_checkstring(L, 1);
    if (filename == NULL)
        break;
    
    bool loop = false;
    if (lua_isboolean(L, 2))
        loop = lua_toboolean(L, 2) == 0 ? false : true;
    
    audioID = AUDIOMANAGER.audioLoad(filename, loop);
    
    BREAK_END;
    
    lua_pushinteger(L, audioID);
    
    return 1;
}

static int audiomanager_audio_play(lua_State* L)
{
    bool success = false;
    
    BREAK_START;
    
    int32_t audioID = (int32_t)luaL_checkinteger(L, 1);
    
    bool reset = false;
    if (lua_isboolean(L, 2))
        reset = lua_toboolean(L, 2) == 0 ? false : true;
    
    success = AUDIOMANAGER.audioPlay(audioID, reset);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int audiomanager_audio_pause(lua_State* L)
{
    bool success = false;
    
    BREAK_START;
    
    int32_t audioID = (int32_t)luaL_checkinteger(L, 1);
    success = AUDIOMANAGER.audioPause(audioID);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int audiomanager_audio_pause_all(lua_State* L)
{
    bool success = AUDIOMANAGER.audioPauseAll();
    lua_pushboolean(L, success?1:0);
    return 1;
}

static int audiomanager_audio_resume(lua_State* L)
{
    bool success = false;
    
    BREAK_START;
    
    int32_t audioID = (int32_t)luaL_checkinteger(L, 1);
    success = AUDIOMANAGER.audioResume(audioID);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int audiomanager_audio_resume_all(lua_State* L)
{
    bool success = AUDIOMANAGER.audioResumeAll();
    lua_pushboolean(L, success?1:0);
    return 1;
}

static int audiomanager_audio_stop(lua_State* L)
{
    bool success = false;
    
    BREAK_START;
    
    int32_t audioID = (int32_t)luaL_checkinteger(L, 1);
    success = AUDIOMANAGER.audioStop(audioID);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int audiomanager_audio_stop_all(lua_State* L)
{
    bool success = AUDIOMANAGER.audioStopAll();
    lua_pushboolean(L, success?1:0);
    return 1;
}

static int audiomanager_audio_free(lua_State* L)
{
    bool success = false;
    
    BREAK_START;
    
    int32_t audioID = (int32_t)luaL_checkinteger(L, 1);
    success = AUDIOMANAGER.audioFree(audioID);
    
    BREAK_END;
    
    lua_pushboolean(L, success?1:0);
    
    return 1;
}

static int audiomanager_audio_free_all(lua_State* L)
{
    bool success = AUDIOMANAGER.audioFreeAll();
    lua_pushboolean(L, success?1:0);
    return 1;
}

void ScriptRegisterAudioManager(lua_State* L)
{
    BREAK_START;
    
    lua_newtable(L);
    
    luaH_setfunc2table(L, "audioLoad",      audiomanager_audio_load);
    luaH_setfunc2table(L, "audioPlay",      audiomanager_audio_play);
    luaH_setfunc2table(L, "audioPause",     audiomanager_audio_pause);
    luaH_setfunc2table(L, "audioPauseAll",  audiomanager_audio_pause_all);
    luaH_setfunc2table(L, "audioResume",    audiomanager_audio_resume);
    luaH_setfunc2table(L, "audioResumeAll", audiomanager_audio_resume_all);
    luaH_setfunc2table(L, "audioStop",      audiomanager_audio_stop);
    luaH_setfunc2table(L, "audioStopAll",   audiomanager_audio_stop_all);
    luaH_setfunc2table(L, "audioFree",      audiomanager_audio_free);
    luaH_setfunc2table(L, "audioFreeAll",   audiomanager_audio_free_all);
    
    lua_setglobal(L, AUDIOMANAGER_LUA_NAME);
    
    BREAK_END;
}

HGNAMESPACE_END