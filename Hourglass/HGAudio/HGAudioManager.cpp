/**
 *  @file    HGAudioManager.cpp
 *  @brief   AudioManager implementation
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

#include "HGAudioManager.h"

#include "vorbisfile.h"

HGNAMESPACE_START
    
AudioManager::AudioManager()
    : mDevice(NULL)
    , mContext(NULL)
    , mAudioSourcesActiveCount(0)
    , mAudioBuffersActiveCount(0)
    , mIsSoundOn(false)
{
    memset(mAudioSources, 0, AUDIO_MAX_SOURCES * sizeof(uint32_t));
    memset(mAudioBuffers, 0, AUDIO_MAX_BUFFERS * sizeof(uint32_t));
    memset(mAudioSourceActive, 0, AUDIO_MAX_SOURCES * sizeof(bool));
    memset(mAudioBufferActive, 0, AUDIO_MAX_BUFFERS * sizeof(bool));
    
    memset(mPosition, 0, 3 * sizeof(ALfloat));
    memset(mVelocity, 0, 3 * sizeof(ALfloat));
    memset(mOrientation, 0, 6 * sizeof(ALfloat));
    
    mOrientation[2] = -1.0f;
    
    memset(mFileHashes, 0, AUDIO_MAX_BUFFERS * sizeof(uint32_t));
}

AudioManager::~AudioManager()
{
}

HG_ERROR AudioManager::initialize()
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
    alcMakeContextCurrent(mContext);
    if (mContext == NULL)
    {
        err = HG_ERROR_AL_CONTEXT_FAILED;
        break;
    }
    
    // Create sources & buffer
    alGenSources(AUDIO_MAX_SOURCES, mAudioSources);
    alGenBuffers(AUDIO_MAX_BUFFERS, mAudioBuffers);
    
    // set up listener
    alListenerfv(AL_POSITION, mPosition);
    alListenerfv(AL_VELOCITY, mVelocity);
    alListenerfv(AL_ORIENTATION, mOrientation);
    
    alListenerf(AL_GAIN, 1.0f);
    
    // Doppler
    alDopplerFactor(1.0f);
    alDopplerVelocity(343.0f);
    
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE)
        HGLog("Audio Device: %s\n", (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    
    mIsSoundOn = true;

    BREAK_END;
    
    return err;
}

void AudioManager::deInitialize()
{
    alDeleteSources(AUDIO_MAX_SOURCES, mAudioSources);
    alDeleteBuffers(AUDIO_MAX_BUFFERS, mAudioBuffers);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

uint32_t AudioManager::audioLoad(const char* filename)
{
    return 0;
}

bool AudioManager::_isALerror()
{
    bool ret = false;
    ALenum err = alGetError();
    
    BREAK_START;
    
    if (err == AL_NO_ERROR)
        break;
    
    // alGetString(err);
    
    BREAK_END;
    
    return ret;
}
    
HGNAMESPACE_END