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

namespace HG
{
    
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
    
private:
    ALCdevice*  mDevice;
    ALCcontext* mContext;
    
private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager& other);
    AudioManager& operator= (const AudioManager& rhs);
};

}

#endif // HGAUDIOMANAGER_H_
