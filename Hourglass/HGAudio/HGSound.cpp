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

ogg_buffer_t* ogg_buffer_create()
{
    ogg_buffer_t* ret = new ogg_buffer_t;
    memset(ret, 0, sizeof(ogg_buffer_t));
    
    return ret;
}

bool ogg_buffer_loadfile(ogg_buffer_t* ob, const char* filename)
{
    bool loaded = false;
    
    BREAK_START;
    
    if (ob == NULL || filename == NULL)
        break;
    
    // clear
    memset(ob, 0, sizeof(ogg_buffer_t));
    
    // open ogg file
    OggVorbis_File *fp = &ob->oggFile;
    if (ov_fopen(filename, fp))
        break;
    
    // get ogg info
    ob->nameHash = Hash(filename);
    ob->comment = ov_comment(fp, -1);
    ob->oggInfo = ov_info(fp, -1);
    switch (ob->oggInfo->channels)
    {
        case 1:
            ob->format = AL_FORMAT_MONO16;
            break;
        case 2:
            ob->format = AL_FORMAT_STEREO16;
            break;
        case 4:
            ob->format = alGetEnumValue("AL_FORMAT_QUAD16");
            break;
        case 6:
            ob->format = alGetEnumValue("AL_FORMAT_51CHN16");
            break;
        case 7:
            ob->format = alGetEnumValue("AL_FORMAT_61CHN16");
            break;
        case 8:
            ob->format = alGetEnumValue("AL_FORMAT_71CHN16");
            break;
        default:
            ob->format = 0;
            break;
    }
    
    // read file
    std::vector<char> samples;
    char tempbuf[HG_SOUND_BUFFER_SIZE];
    int section = 0;
    bool firstrun = true;
    while (1)
    {
        long result = ov_read(fp, tempbuf, HG_SOUND_BUFFER_SIZE, 0, 2, 1, &section);
        if (result > 0)
        {
            firstrun = false;
            samples.insert(samples.end(), tempbuf, tempbuf + result);
        }
        else if (result < 0)
        {
            ov_clear(fp);
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
    
    // generate openal buffer id
    alGenBuffers(1, &ob->buffer);
    if (CHK_AL_ERROR)
        break;
    
    // buffer data
    alBufferData(ob->buffer, ob->format, &samples[0], (ALsizei)samples.size(), (int)ob->oggInfo->rate);
    ov_clear(fp);
    if (CHK_AL_ERROR)
        alDeleteBuffers(1, &ob->buffer);
    else
        loaded = true;
    
    BREAK_END;
    
    return loaded;
}

void ogg_buffer_release(ogg_buffer_t* ob)
{
    if (ob == NULL || ob->buffer == 0)
        return;
    
    alDeleteBuffers(1, &ob->buffer);
    CHK_AL_ERROR;
}

void ogg_buffer_destory(ogg_buffer_t* ob)
{
    BREAK_START;
    
    if (ob == NULL)
        break;
    
    ogg_buffer_release(ob);
    
    delete ob;
    
    BREAK_END;
}


// ogg source

ogg_source_t* ogg_source_create()
{
    ogg_source_t* ret = new ogg_source_t;
    memset(ret, 0, sizeof(ogg_source_t));
    ret->pitch = 1.0f;
    ret->gain = 1.0f;
    
    return ret;
}

void ogg_source_bind_buffer(ogg_source_t* os, ALuint buffer)
{
    BREAK_START;
    
    if (os == NULL || buffer == 0)
        break;
    
    if (os->source != 0)
        ogg_source_release(os);
    
    alGenSources(1, &os->source);
    CHK_AL_ERROR;
    if (os->source == 0)
        break;
    
    os->buffer = buffer;
    alSourcei(os->source, AL_BUFFER, buffer);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_setup(ogg_source_t* os)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    ogg_source_set_pitch(os, os->pitch);
    ogg_source_set_gain(os, os->gain);
    ogg_source_set_loop(os, os->loop);
    ogg_source_set_position(os, os->pos[0], os->pos[1], os->pos[2]);
    ogg_source_set_velocity(os, os->vel[0], os->vel[1], os->vel[2]);
    ogg_source_set_direction(os, os->dir[0], os->dir[1], os->dir[2]);
    
    BREAK_END;
}

void ogg_source_set_pitch(ogg_source_t* os, ALfloat pitch)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->pitch = pitch;
    alSourcef(os->source, AL_PITCH, os->pitch);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_set_gain(ogg_source_t* os, ALfloat gain)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->gain = gain;
    alSourcef(os->source, AL_GAIN, os->gain);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_set_loop(ogg_source_t* os, ALint loop)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->loop = loop;
    alSourcef(os->source, AL_LOOPING, os->loop);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_set_position(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->pos[0] = x;
    os->pos[1] = y;
    os->pos[2] = z;
    alSourcefv(os->source, AL_POSITION, os->pos);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_set_velocity(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->vel[0] = x;
    os->vel[1] = y;
    os->vel[2] = z;
    alSourcefv(os->source, AL_VELOCITY, os->vel);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_set_direction(ogg_source_t* os, ALfloat x, ALfloat y, ALfloat z)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    os->dir[0] = x;
    os->dir[1] = y;
    os->dir[2] = z;
    alSourcefv(os->source, AL_DIRECTION, os->dir);
    CHK_AL_ERROR;
    
    BREAK_END;
}

void ogg_source_release(ogg_source_t* os)
{
    BREAK_START;
    
    if (os == NULL || os->source == 0)
        break;
    
    alSourceStop(os->source);
    alDeleteSources(1, &os->source);
    CHK_AL_ERROR;
    
    os->source = 0;
    
    BREAK_END;
}

void ogg_source_destory(ogg_source_t* os)
{
    BREAK_START;
    
    if (os == NULL)
        break;
    
    ogg_source_release(os);
    
    delete os;
    
    BREAK_END;
}

HGNAMESPACE_END