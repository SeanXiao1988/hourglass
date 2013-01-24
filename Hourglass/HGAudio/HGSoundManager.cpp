/**
 *  @file    HGSoundManager.cpp
 *  @brief   SoundManager implementation
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

#include "HGSoundManager.h"
#include "HGHash.h"

HGNAMESPACE_START

SoundManager::SoundManager()
    :mDevice(NULL)
    ,mContext(NULL)
{
    
}

SoundManager::~SoundManager()
{
    
}

HG_ERROR SoundManager::initialize()
{
    HG_ERROR err = HG_OK;
    
    BREAK_START;
    
    mDevice = alcOpenDevice(NULL);
    if (mDevice == NULL)
    {
        err = HG_ERROR_AL_DEVICE_NOT_SUPPORT;
        break;
    }
    
    mContext = alcCreateContext(mDevice, NULL);
    if (mContext == NULL)
    {
        err = HG_ERROR_AL_CONTEXT_FAILED;
        break;
    }
    
    alcMakeContextCurrent(mContext);
    
    alListenerf(AL_GAIN, 1.0f);
    
    // Doppler
    alDopplerFactor(1.0f);
    alDopplerVelocity(343.0f);
    
    BREAK_END;
    
    return err;
}

void SoundManager::deInitialize()
{
    clear();
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

void SoundManager::clear()
{
    OggSourceList::iterator srcIter = mSourceList.begin();
    for (; srcIter != mSourceList.end(); ++srcIter)
    {
        ogg_source_destory(*srcIter);
    }
    mSourceList.clear();
    
    OggBufferList::iterator bufIter = mBufferList.begin();
    for (; bufIter != mBufferList.end(); ++bufIter)
    {
        ogg_buffer_destory(*bufIter);
    }
    mBufferList.clear();
}

ALuint SoundManager::loadOggFile(const char *filename)
{
    ALuint sourceID = 0;
    
    BREAK_START;
    
    if (filename == NULL || mBufferList.size() >= SOUND_MAX_BUFFERS || mSourceList.size() >= SOUND_MAX_SOURCES)
        break;
    
    ogg_buffer_t* buffer = NULL;
    
    buffer = _loadOggFile(filename);
    if (buffer == NULL)
        break;
    
    ogg_source_t* source = NULL;
    source = _findSource(buffer->buffer);
    if (source == NULL)
         source = _createSourceWithBuffer(buffer->buffer);
    
    if (source == NULL)
        break;
    
    sourceID = source->source;
    
    BREAK_END;
    
    return sourceID;
}

ALuint SoundManager::forkOggFile(const char *filename)
{
    ALuint sourceID = 0;
    
    BREAK_START;
    
    if (filename == NULL || mSourceList.size() >= SOUND_MAX_SOURCES)
        break;
    
    uint32_t fileNameHash = Hash(filename);
    ogg_buffer_t* buffer = NULL;
    buffer = _findBuffer(fileNameHash);
    if (buffer == NULL)
        buffer = _loadOggFile(filename);
    
    if (buffer == NULL)
        break;
    
    ogg_source_t* source = _createSourceWithBuffer(buffer->buffer);
    if (source == NULL)
        break;
    
    sourceID = source->source;
    
    BREAK_END;
    
    return sourceID;
}

void SoundManager::freeOggFile(const char* filename)
{
    BREAK_START;
    
    if (filename == NULL)
        break;
    
    ogg_buffer_t* buffer = _findBuffer(Hash(filename));
    if (buffer == NULL)
        break;
    
    _freeSourceWithBuffer(buffer->buffer);
    
    OggBufferList::iterator iter = mBufferList.begin();
    for (; iter != mBufferList.end(); ++iter)
    {
        if (*iter == buffer)
        {
            ogg_buffer_destory(*iter);
            mBufferList.erase(iter);
            break;
        }
    }
    
    BREAK_END;
}

void SoundManager::freeOggSource(ALuint sourceID)
{
    BREAK_START;
    
    if (sourceID == 0)
        break;
    
    OggSourceList::iterator iter = mSourceList.begin();
    for (; iter != mSourceList.end(); ++iter)
    {
        if ((*iter)->source == sourceID)
        {
            ogg_source_destory(*iter);
            mSourceList.erase(iter);
            break;
        }
    }
    
    BREAK_END;
}

ogg_buffer_t* SoundManager::_loadOggFile(const char* filename)
{
    ogg_buffer_t* buffer = NULL;
    
    BREAK_START;
    
    if (filename == NULL)
        break;
    
    // find exist buffer
    uint32_t nameHash = Hash(filename);
    buffer = _findBuffer(nameHash);
    if (buffer != NULL)
        break;
    
    // not found, create
    buffer = ogg_buffer_create();
    ogg_buffer_loadfile(buffer, filename);
    // error
    if (buffer == NULL)
        break;
    
    mBufferList.push_back(buffer);
    
    BREAK_END;
    
    return buffer;
}

ogg_buffer_t* SoundManager::_findBuffer(uint32_t fileNameHash)
{
    ogg_buffer_t* buffer = NULL;
    
    BREAK_START;
    
    OggBufferList::iterator iter = mBufferList.begin();
    for (; iter != mBufferList.end(); ++iter)
    {
        if ((*iter)->nameHash == fileNameHash)
        {
            buffer = *iter;
            break;
        }
    }
    
    BREAK_END;
    
    return buffer;
}

ogg_source_t* SoundManager::_findSource(ALuint bufferID)
{
    ogg_source_t* source = 0;
    
    BREAK_START;
    
    OggSourceList::iterator iter = mSourceList.begin();
    for (; iter != mSourceList.end(); ++iter)
    {
        if ((*iter)->buffer == bufferID)
        {
            source = *iter;
            break;
        }
    }
    
    BREAK_END;
    
    return source;
}

ogg_source_t* SoundManager::_createSourceWithBuffer(ALuint bufferID)
{
    ogg_source_t* source = NULL;
    
    BREAK_START;
    
    if (bufferID == 0)
        break;
    
    source = ogg_source_create();
    if (source == NULL)
        break;
    
    ogg_source_bind_buffer(source, bufferID);
    
    mSourceList.push_back(source);
    
    BREAK_END;
    
    return source;
}

void SoundManager::_freeSourceWithBuffer(ALuint bufferID)
{
    OggSourceList::iterator iter = mSourceList.begin();
    for (; iter != mSourceList.end(); )
    {
        if ((*iter)->buffer == bufferID)
        {
            ogg_source_destory(*iter);
            mSourceList.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

HGNAMESPACE_END