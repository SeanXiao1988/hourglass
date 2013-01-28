/**
 *  @file    HGSoundManager.h
 *  @brief   SoundManager header
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

#ifndef HGSOUNDMANAGER_H_
#define HGSOUNDMANAGER_H_

#include "HGAudioDef.h"
#include "HGSound.h"
#include "HGError.h"

#define SOUNDMANAGER SoundManager::getInstance()

#define SOUND_MAX_BUFFERS   64
#define SOUND_MAX_SOURCES   16

HGNAMESPACE_START

typedef std::list<ogg_buffer_t *> OggBufferList;
typedef std::list<ogg_source_t *> OggSourceList;

class SoundManager
{
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }
    
    // service
    
    HG_ERROR        initialize();
    void            deInitialize();
    
    // file management
    
    void            clear();
    
    ALuint          loadOggFile(const char* filename, bool looping = false);
    ALuint          forkOggFile(const char* filename);
    
    void            freeOggFile(const char* filename);
    void            freeOggSource(ALuint sourceID);
    
    // sound management
    void            soundPlay(ALuint sourceID, bool reset = false);
    void            soundPause(ALuint sourceID);
    void            soundResume(ALuint sourceID);
    void            soundStop(ALuint sourceID);
    
    void            soundPauseAll();
    void            soundResumeAll();
    void            soundStopAll();
    
private:
    ogg_buffer_t*   _loadOggFile(const char* filename);
    ogg_buffer_t*   _findBuffer(uint32_t fileNameHash);
    ogg_source_t*   _createSourceWithBuffer(ALuint bufferID);
    ogg_source_t*   _findSource(ALuint bufferID);
    void            _freeSourceWithBuffer(ALuint bufferID);
    
    ALCdevice*      mDevice;
    ALCcontext*     mContext;
    
    OggBufferList   mBufferList;
    OggSourceList   mSourceList;

private:
    SoundManager();
    ~SoundManager();
    SoundManager(const SoundManager& other);
    SoundManager& operator= (const SoundManager& rhs);
};

HGNAMESPACE_END

#endif // HGSOUNDMANAGER_H_
