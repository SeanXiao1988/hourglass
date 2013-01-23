/**
 *  @file    HGAudioDef.h
 *  @brief   Audio definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/18
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

#ifndef HGAUDIODEF_H_
#define HGAUDIODEF_H_

#include "HGSystem.h"

#define HG_SOUND_MAX_BUFFERS    64
#define HG_SOUND_MAX_SOURCES    16
#define HG_SOUND_BUFFER_SIZE    32768

#define CHK_AL_ERROR alIsError(__FILE__, __LINE__, __func__)

HGNAMESPACE_START

static bool alIsError(const char* file, int line, const char* func)
{
    bool ret = false;
    ALenum err = alGetError();
    
    BREAK_START;
    
    if (err == AL_NO_ERROR)
        break;
    
    HGLog("Error: %s @ %s|%d|%s()\n", alGetString(err), file, line, func);
    ret = true;
    
    BREAK_END;
    
    return ret;
}

HGNAMESPACE_END

#endif // HGAUDIODEF_H_
