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
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

HGNAMESPACE_END