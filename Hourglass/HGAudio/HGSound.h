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

typedef struct _ogg_buffer_t_
{
    uint32_t        nameHash;
    ALuint          buffer;
    ALenum          format;
    
    OggVorbis_File  oggFile;
    vorbis_info*    oggInfo;
    vorbis_comment* comment;
}ogg_buffer_t;

extern ogg_buffer_t* ogg_buffer_create();
extern bool ogg_buffer_loadfile(ogg_buffer_t* ob, const char* filename);
extern void ogg_buffer_release(ogg_buffer_t* ob);
extern void ogg_buffer_destory(ogg_buffer_t* ob);

typedef struct _ogg_source_t_
{
    ALuint          source;
    ALuint          buffer; // the binding buffer
    
    ALint           loop;
    ALfloat         pitch;
    ALfloat         gain;
    ALfloat         pos[3];
    ALfloat         vel[3];
    ALfloat         dir[3];
}ogg_source_t;

extern ogg_source_t* ogg_source_create();
extern void ogg_source_bind_buffer(ogg_source_t* os, ALuint buffer);
extern void ogg_source_setup(ogg_source_t* os);
extern void ogg_source_set_pitch(ogg_source_t* os, ALfloat pitch);
extern void ogg_source_set_gain(ogg_source_t* os, ALfloat gain);
extern void ogg_source_set_loop(ogg_source_t* os, ALint loop);
extern void ogg_source_set_position(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z);
extern void ogg_source_set_velocity(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z);
extern void ogg_source_set_direction(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z);
extern void ogg_source_release(ogg_source_t* os);
extern void ogg_source_destory(ogg_source_t* os);

HGNAMESPACE_END

#endif // HGSOUND_H_
