/**
 *  @file    HGSound.h
 *  @brief   Sound header
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

#ifndef HGSOUND_H_
#define HGSOUND_H_

#include "HGAudioDef.h"

HGNAMESPACE_START

class Sound
{
public:
    Sound();
    ~Sound();
    
    bool    open(const char* file);
    void    clear();
    
    bool    play(bool reset=false);
    bool    pause();
    bool    resume();
    bool    stop();
    
    bool    update();
    
    bool    isPlaying();
    
private:
    bool    _stream(ALuint buffer);
    void    _dequeue();
    
    // sound info
    uint32_t        mFileHash;
    
    OggVorbis_File  mOggStream;
    vorbis_info*    mVorbisInfo;
    vorbis_comment* mVorbisComment;
    
    ALuint          mBuffers[2];
    ALuint          mSource;
    ALenum          mFormat;
    
private:
    Sound(const Sound& other);
    Sound& operator= (const Sound& rhs);
};

HGNAMESPACE_END

#endif // HGSOUND_H_
