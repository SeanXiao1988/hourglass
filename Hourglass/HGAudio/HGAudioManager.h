/**
 *  @file    HGAudioManager.h
 *  @brief   AudioManager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/19
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

#ifndef HGAUDIOMANAGER_H_
#define HGAUDIOMANAGER_H_

#include "HGAudioDef.h"
#include "HGError.h"

#define AUDIOMANAGER AudioManager::getInstance()

#define AUDIO_MAX_BUFFERS   64
#define AUDIO_MAX_SOURCES   16

HGNAMESPACE_START

using namespace glm;
    
class AudioManager
{
public:
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }
    
    HG_ERROR    initialize();
    void        deInitialize();
    
    int32_t     audioLoad(const char* filename, bool roop = false);
    bool        audioPlay(int32_t audioID, bool reset = false);
    bool        audioPause(int32_t audioID);
    bool        audioPauseAll();
    bool        audioResume(int32_t audioID);
    bool        audioResumeAll();
    bool        audioStop(int32_t audioID);
    bool        audioStopAll();
    bool        audioFree(int32_t audioID);
    bool        audioFreeAll();
    
    bool        audioSet(int32_t audioID, vec3 pos, vec3 vel, vec3 dir, float maxDistance, float minGain, bool play, bool reset);
    
    bool        audioSetPosition(int32_t audioID, vec3 pos);
    bool        audioSetup(int32_t audioID, vec3 pos, vec3 vel, vec3 dir);

    bool        listenerSet(vec3 pos, vec3 vel, vec3 ori);
    
private:
    bool        _isALerror();
    int32_t     _findLoadedBuffer(const char* filename);
    int32_t     _loadAudioFile(const char* filename);
    bool        _loadOGG(const char* filename, ALuint buffer);
    
    ALfloat     mPosition[3];
    ALfloat     mVelocity[3];
    ALfloat     mOrientation[6];
    
    // audio sources
    uint32_t    mAudioSourcesActiveCount;
    ALuint      mAudioSources[AUDIO_MAX_SOURCES];
    bool        mAudioSourceActive[AUDIO_MAX_SOURCES];
    
    // audio buffers
    uint32_t    mAudioBuffersActiveCount;
    ALuint      mAudioBuffers[AUDIO_MAX_BUFFERS];
    bool        mAudioBufferActive[AUDIO_MAX_BUFFERS];
    
    uint32_t    mFileHashes[AUDIO_MAX_BUFFERS];
    
    bool        mIsSoundOn;
    
    ALCdevice*  mDevice;
    ALCcontext* mContext;
    
private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager& other);
    AudioManager& operator= (const AudioManager& rhs);
};

HGNAMESPACE_END

#endif // HGAUDIOMANAGER_H_
