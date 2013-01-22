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

HGNAMESPACE_START

typedef struct _ogg_stream_t_
{
    OggVorbis_File  oggFile;
    vorbis_info     oggInfo;
    vorbis_comment  comment;
    
    ALuint          buffers[2];
    ALuint          source;
    ALenum          format;
}ogg_stream_t;

HGNAMESPACE_END

#endif // HGAUDIODEF_H_
