/**
 *  @file    HGSoundManagerScripter.cpp
 *  @brief   Sound Manager script adapter implementation
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

#include "HGSoundManagerScripter.h"
#include "HGSoundManager.h"

#define SOUNDMANAGER_LUA_NAME "SOUNDMANAGER"

HGNAMESPACE_START

static int soundmanager_clear(lua_State* L)
{
    SOUNDMANAGER.clear();
    return 0;
}

static int soundmanager_load_ogg_file(lua_State* L)
{
    ALuint sid = 0;
    
    BREAK_START;
    
    const char* filename = luaL_checkstring(L, 1);
    if (filename == NULL)
        break;
    
    bool looping = luaH_checkboolean(L, 2);
    
    sid = SOUNDMANAGER.loadOggFile(filename, looping);
    
    BREAK_END;
    
    lua_pushunsigned(L, sid);
    
    return 1;
}

static int soundmanager_fork_ogg_file(lua_State* L)
{
    ALuint sid = 0;
    
    BREAK_START;
    
    const char* filename = luaL_checkstring(L, 1);
    if (filename == NULL)
        break;
    
    sid = SOUNDMANAGER.forkOggFile(filename);
    
    BREAK_END;
    
    lua_pushunsigned(L, sid);
    
    return 1;
}

static int soundmanager_free_ogg_file(lua_State* L)
{
    BREAK_START;
    
    const char* filename = luaL_checkstring(L, 1);
    if (filename == NULL)
        break;
    
    SOUNDMANAGER.freeOggFile(filename);
    
    BREAK_END;
    
    return 0;
}

static int soundmanager_free_ogg_source(lua_State* L)
{
    ALuint sid = lua_tounsigned(L, 1);
    SOUNDMANAGER.freeOggSource(sid);
    
    return 0;
}

static int soundmanager_sound_play(lua_State* L)
{
    ALuint sid = lua_tounsigned(L, 1);
    bool reset = luaH_checkboolean(L, 2);
    
    SOUNDMANAGER.soundPlay(sid, reset);
    
    return 0;
}

static int soundmanager_sound_pause(lua_State* L)
{
    ALuint sid = lua_tounsigned(L, 1);
    SOUNDMANAGER.soundPause(sid);
    
    return 0;
}

static int soundmanager_sound_resume(lua_State* L)
{
    ALuint sid = lua_tounsigned(L, 1);
    SOUNDMANAGER.soundResume(sid);
    
    return 0;
}

static int soundmanager_sound_stop(lua_State* L)
{
    ALuint sid = lua_tounsigned(L, 1);
    SOUNDMANAGER.soundStop(sid);
    
    return 0;
}

static int soundmanager_sound_pause_all(lua_State* L)
{
    SOUNDMANAGER.soundPauseAll();
    return 0;
}

static int soundmanager_sound_resume_all(lua_State* L)
{
    SOUNDMANAGER.soundResumeAll();
    return 0;
}

static int soundmanager_sound_stop_all(lua_State* L)
{
    SOUNDMANAGER.soundStopAll();
    return 0;
}

void ScriptRegisterSoundManager(lua_State* L)
{
    lua_newtable(L);
    
    luaH_setfunc2table(L, "clear",          soundmanager_clear);
    luaH_setfunc2table(L, "loadOggFile",    soundmanager_load_ogg_file);
    luaH_setfunc2table(L, "forkOggFile",    soundmanager_fork_ogg_file);
    luaH_setfunc2table(L, "freeOggFile",    soundmanager_free_ogg_file);
    luaH_setfunc2table(L, "freeOggSource",  soundmanager_free_ogg_source);
    luaH_setfunc2table(L, "soundPlay",      soundmanager_sound_play);
    luaH_setfunc2table(L, "soundPause",     soundmanager_sound_pause);
    luaH_setfunc2table(L, "soundResume",    soundmanager_sound_resume);
    luaH_setfunc2table(L, "soundStop",      soundmanager_sound_stop);
    luaH_setfunc2table(L, "soundPauseAll",  soundmanager_sound_pause_all);
    luaH_setfunc2table(L, "soundResumeAll", soundmanager_sound_resume_all);
    luaH_setfunc2table(L, "soundStopAll",   soundmanager_sound_stop_all);

    lua_setglobal(L, SOUNDMANAGER_LUA_NAME);
}

HGNAMESPACE_END