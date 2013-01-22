/**
 *  @file    HGSound.cpp
 *  @brief   Sound implementation
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

#include "HGSound.h"
#include "HGHash.h"

HGNAMESPACE_START

Sound::Sound()
    : mFileHash(0)
    , mVorbisInfo(NULL)
    , mVorbisComment(NULL)
    , mSource(0)
    , mFormat(0)
{
    mBuffers[0] = 0;
    mBuffers[1] = 0;
}

Sound::~Sound()
{
    clear();
}

bool Sound::open(const char *file)
{
    bool ret = false;
    
    BREAK_START;
    
    if (file == NULL)
        break;
    
    // open ogg file
    if (ov_fopen(file, &mOggStream))
        break;
    
    mFileHash = Hash(file);
    
    // get ogg file info
    mVorbisInfo = ov_info(&mOggStream, -1);
    mVorbisComment = ov_comment(&mOggStream, -1);
    
    switch (mVorbisInfo->channels)
    {
        case 1:
            mFormat = AL_FORMAT_MONO16;
            break;
        case 2:
            mFormat = AL_FORMAT_STEREO16;
            break;
        case 4:
            mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
            break;
        case 6:
            mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
            break;
        case 7:
            mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
            break;
        case 8:
            mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
            break;
        default:
            mFormat = 0;
            break;
    }
    
    // create buffer
    alGenBuffers(2, mBuffers);
    if (alIsError())
        break;
    
    alGenSources(1, &mSource);
    if (alIsError())
        break;
    
    alSource3f(mSource, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(mSource, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(mSource, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (mSource, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (mSource, AL_SOURCE_RELATIVE, AL_TRUE      );

    ret = true;
    
    BREAK_END;
    
    if (!ret) {
        clear();
    }
    
    return ret;
}

void Sound::clear()
{
    if (mSource != 0)
    {
        alSourceStop(mSource);
        alDeleteSources(1, &mSource);
    }
    
    _dequeue();
    
    alDeleteBuffers(2, mBuffers);
    
    ov_clear(&mOggStream);
}

bool Sound::play(bool reset)
{
    bool ret = false;
    
    BREAK_START;
    
    if (!reset && isPlaying())
    {
        ret = true;
        break;
    }
    
    if (!_stream(mBuffers[0]))
    {
        ret = false;
        break;
    }
    
    if (!_stream(mBuffers[1]))
    {
        ret = false;
        break;
    }
    
    alSourceQueueBuffers(mSource, 2, mBuffers);
    alSourcePlay(mSource);

    ret = true;
    
    BREAK_END;
    
    return ret;
}

bool Sound::isPlaying()
{
    ALenum state;
    
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}

bool Sound::update()
{
    int processed = 0;
    bool active = false;
    
    alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
    
    while (processed--)
    {
        ALuint buffer = 0;
        
        alSourceUnqueueBuffers(mSource, 1, &buffer);
        if (alIsError())
            break;
        
        _stream(buffer);
        
        alSourceQueueBuffers(mSource, 1, &buffer);
        
        active = !alIsError();
    }
    
    return active;
}

bool Sound::_stream(ALuint buffer)
{
    char pcm[HG_SOUND_BUFFER_SIZE];
    int size = 0;
    int section = 0;
    long result = 0;
    
    while (size < HG_SOUND_BUFFER_SIZE)
    {
        result = ov_read(&mOggStream, pcm + size, HG_SOUND_BUFFER_SIZE - size, 0, 2, 1, &section);
        
        if (result > 0)
        {
            size += result;
        }
        else
        {
            if (result < 0)
                return false;
            else
                break;
        }
    }
    
    if (size == 0)
        return false;
    
    alBufferData(buffer, mFormat, pcm, size, static_cast<ALsizei>(mVorbisInfo->rate));
    return !alIsError();
}

void Sound::_dequeue()
{
    int queue;
    
    if (mSource == 0)
        return;
    
    alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queue);
    alIsError();
    while (queue--)
    {
        ALuint buffer = 0;
        alSourceUnqueueBuffers(mSource, 1, &buffer);
        if (alIsError())
            break;
    }
}

HGNAMESPACE_END