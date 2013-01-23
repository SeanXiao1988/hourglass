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
    if (ob == NULL)
        return;
    
    alDeleteBuffers(1, &ob->buffer);
    CHK_AL_ERROR;
}

void ogg_buffer_destory(ogg_buffer_t* ob)
{
    ogg_buffer_release(ob);
    delete ob;
}


// ogg source

ogg_source_t* ogg_source_create()
{
    ogg_source_t* ret = new ogg_source_t;
    memset(ret, 0, sizeof(ogg_source_t));
    
    return ret;
}

void ogg_source_bind_buffer(ogg_source_t* os, ALuint buffer)
{
    BREAK_START;
    
    if (os == NULL || buffer == 0)
        break;
    
    
    
    BREAK_END;
}

HGNAMESPACE_END