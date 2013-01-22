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
#include "HGError.h"

#define SOUNDMANAGER SoundManager::getInstance()

HGNAMESPACE_START

class SoundManager
{
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
    }
    
    HG_ERROR    initialize();
    void        deInitialize();
    
private:
    ALCdevice*  mDevice;
    ALCcontext* mContext;

private:
    SoundManager();
    ~SoundManager();
    SoundManager(const SoundManager& other);
    SoundManager& operator= (const SoundManager& rhs);
};

HGNAMESPACE_END

#endif // HGSOUNDMANAGER_H_
