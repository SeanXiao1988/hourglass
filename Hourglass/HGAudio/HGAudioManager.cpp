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
#include "HGHash.h"

HGNAMESPACE_START
    
AudioManager::AudioManager()
    : mDevice(NULL)
    , mContext(NULL)
    , mAudioSourcesActiveCount(0)
    , mAudioBuffersActiveCount(0)
    , mIsSoundOn(false)
{
    memset(mAudioSources, 0, AUDIO_MAX_SOURCES * sizeof(ALuint));
    memset(mAudioBuffers, 0, AUDIO_MAX_BUFFERS * sizeof(ALuint));
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

int32_t AudioManager::audioLoad(const char* filename, bool loop)
{
    int32_t bufferID = 0;
    int32_t sourceID = -1;
    BREAK_START;
    
    if (filename == NULL)
        break;
    
    if (mAudioBuffersActiveCount == AUDIO_MAX_BUFFERS)
        break;
    
    bufferID = _findLoadedBuffer(filename);
    if (bufferID < 0)
        bufferID = _loadAudioFile(filename);
    
    if (bufferID < 0)
        break;
    
    // audio source
    sourceID = 0;
    while (mAudioSourceActive[sourceID])
        sourceID++;
    
    if (sourceID >= AUDIO_MAX_SOURCES)
    {
        sourceID = -1;
        break;
    }
    
    mAudioSourceActive[sourceID] = true;
    mAudioSourcesActiveCount++;
    
    // attach sound to buffer
    alSourcei(mAudioSources[sourceID], AL_BUFFER, mAudioBuffers[bufferID]);
    // loop
    alSourcei(mAudioSources[sourceID], AL_LOOPING, loop);
    
    _isALerror();
    
    BREAK_END;
    
    return sourceID;
}

bool AudioManager::audioPlay(int32_t audioID, bool reset)
{
    bool ret = false;
    
    BREAK_START;

    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    int sourceState = 0;
    
    alGetSourcei(mAudioSources[audioID], AL_SOURCE_STATE, &sourceState);
    
    if (sourceState == AL_PLAYING)
    {
        if (reset)
        {
            // stop(audio);
        }
        else
        {
            ret = true;
            break;
        }
    }
    
    alSourcePlay(mAudioSources[audioID]);
    ret = true;
    _isALerror();
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioPause(int32_t audioID)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    alSourcePause(mAudioSources[audioID]);
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioPauseAll()
{
    bool ret = false;
    
    BREAK_START;
    
    if (mAudioSourcesActiveCount >= AUDIO_MAX_SOURCES)
        break;
    
    alSourcePausev(mAudioSourcesActiveCount, mAudioSources);
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioResume(int32_t audioID)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    alSourcePlay(mAudioSources[audioID]);
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioResumeAll()
{
    bool ret = false;
    
    BREAK_START;
    
    if (mAudioSourcesActiveCount >= AUDIO_MAX_SOURCES)
        break;
    
    int sourceState = 0;
    for (uint32_t i = 0; i < mAudioSourcesActiveCount; i++)
    {
        alGetSourcei(mAudioSources[i], AL_SOURCE_STATE, &sourceState);
        
        if (sourceState == AL_PAUSED)
        {
            audioResume(i);
        }
    }
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioStop(int32_t audioID)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    alSourceStop(mAudioSources[audioID]);
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioStopAll()
{
    bool ret = false;
    
    BREAK_START;
    
    if (mAudioSourcesActiveCount >= AUDIO_MAX_SOURCES)
        break;
    
    for (uint32_t i = 0; i < mAudioSourcesActiveCount; i++)
    {
        audioStop(i);
    }
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioFree(int32_t audioID)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES)
        break;
    
    alSourceStop(mAudioSources[audioID]);
    mAudioSourceActive[audioID] = false;
    mAudioSourcesActiveCount--;
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioFreeAll()
{
    bool ret = false;
    
    BREAK_START;
    
    if (mAudioSourcesActiveCount >= AUDIO_MAX_SOURCES)
        break;
    
    int count = mAudioSourcesActiveCount;
    
    for (int i = 0; i < count; i++)
    {
        audioFree(i);
    }
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioSet(int32_t audioID, vec3 pos, vec3 vel, vec3 dir, float maxDistance, float minGain, bool play, bool reset)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    audioSetup(audioID, pos, vel, dir);
    
    ALuint sid = mAudioSources[audioID];
    
    alSourcef(sid, AL_MAX_DISTANCE, maxDistance);
    alSourcef(sid, AL_MIN_GAIN, minGain);
    alSourcef(sid, AL_MAX_GAIN, 1.0f);
    alSourcef(sid, AL_ROLLOFF_FACTOR, 1.0f);
    
    if (play)
    {
        audioPlay(audioID, reset);
    }
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioSetPosition(int32_t audioID, vec3 pos)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;

    ALfloat position[] = { pos.x, pos.y, pos.z };
    
    alSourcefv(mAudioSources[audioID], AL_POSITION, position);
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::audioSetup(int32_t audioID, vec3 pos, vec3 vel, vec3 dir)
{
    bool ret = false;
    
    BREAK_START;
    
    if (audioID >= AUDIO_MAX_SOURCES || !mAudioSourceActive[audioID])
        break;
    
    ALuint sid = mAudioSources[audioID];
    
    // set position
    ALfloat position[] = { pos.x, pos.y, pos.z };
    alSourcefv(sid, AL_POSITION, position);
    
    ALfloat velocity[] = { vel.x, vel.y, vel.z };
    alSourcefv(sid, AL_VELOCITY, velocity);
    
    ALfloat direction[] = { dir.x, dir.y, dir.z };
    alSourcefv(sid, AL_DIRECTION, direction);
    
    ret = !_isALerror();
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::listenerSet(vec3 pos, vec3 vel, vec3 ori)
{
    bool ret = false;
    
    ALfloat position[] = { pos.x, pos.y, pos.z };
    alListenerfv(AL_POSITION, position);
    
    ALfloat velocity[] = { vel.x, vel.y, vel.z };
    alListenerfv(AL_VELOCITY, velocity);
    
    vec3 axis = vec3(0.0f);
    axis.x = ori.y;
    axis.y = ori.p;
    axis.z = ori.r;
    
    ALfloat direction[] = { axis.x, axis.y, axis.z };
    alListenerfv(AL_ORIENTATION, direction);
    
    alListenerf( AL_MAX_DISTANCE, 10000.0f );
    alListenerf( AL_MIN_GAIN, 0.0f );
    alListenerf( AL_MAX_GAIN, 1.0f );
    alListenerf( AL_GAIN, 1.0f );
    ret = !_isALerror();
    
    return ret;
}

int32_t AudioManager::_findLoadedBuffer(const char *filename)
{
    int32_t bufferID = -1;
    
    for (int i = 0; i < AUDIO_MAX_BUFFERS; i++)
    {
        if (mFileHashes[i] == Hash(filename))
        {
            bufferID = i;
            break;
        }
    }
    
    return bufferID;
}

int32_t AudioManager::_loadAudioFile(const char *filename)
{
    int32_t bufferID = 0;
    
    BREAK_START;
    
    if (filename == NULL)
        break;
    
    if (mAudioBuffersActiveCount == AUDIO_MAX_BUFFERS)
        break;
    
    while (mAudioBufferActive[bufferID])
        bufferID++;
    
    std::string name = filename;
    if (name.find(".ogg", 0) != std::string::npos)
    {
        if (!_loadOgg(filename, mAudioBuffers[bufferID]))
        {
            bufferID = -1;
            break;
        }
        
        mAudioBufferActive[bufferID] = true;
        mFileHashes[bufferID] = Hash(filename);
        mAudioBuffersActiveCount++;
    }
    
    BREAK_END;
    
    return bufferID;
}

bool AudioManager::_loadOgg(const char *filename, ALuint buffer)
{
    bool ret = false;
    
    OggVorbis_File oggFile;
    
    BREAK_START;
    
    if (ov_fopen(filename, &oggFile))
        break;
    
    vorbis_info* info = ov_info(&oggFile, -1);
    
    ALenum format;
    switch (info->channels)
    {
        case 1:
            format = AL_FORMAT_MONO16;
            break;
        case 2:
            format = AL_FORMAT_STEREO16;
            break;
        case 4:
            format = alGetEnumValue("AL_FORMAT_QUAD16");
            break;
        case 6:
            format = alGetEnumValue("AL_FORMAT_51CHN16");
            break;
        case 7:
            format = alGetEnumValue("AL_FORMAT_61CHN16");
            break;
        case 8:
            format = alGetEnumValue("AL_FORMAT_71CHN16");
            break;
        default:
            format = 0;
            break;
    }
    
    std::vector<int16_t> samples;
    char tempbuf[4096];
    int section = 0;
    bool firstrun = true;
    while (1)
    {
        long result = ov_read(&oggFile, tempbuf, 4096, 0, 2, 1, &section);
        if (result > 0)
        {
            firstrun = false;
            samples.insert(samples.end(), tempbuf, tempbuf+(result));
        }
        else if (result < 0)
        {
            ov_clear(&oggFile);
            return false;
        }
        else
        {
            if (firstrun)
                return false;
            else
                break;
        }
    }
    
    alBufferData(buffer, format, &samples[0], (int)ov_pcm_total(&oggFile, -1), (int)info->rate);
    
    ret = true;
    
    BREAK_END;
    
    return ret;
}

bool AudioManager::_isALerror()
{
    bool ret = false;
    ALenum err = alGetError();
    
    BREAK_START;
    
    if (err == AL_NO_ERROR)
        break;
    
    HGLog("Error: %s\n", alGetString(err));
    
    BREAK_END;
    
    return ret;
}
    
HGNAMESPACE_END