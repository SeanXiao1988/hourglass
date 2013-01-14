/**
 *  @file    HGAnimationDef.h
 *  @brief   Animation definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/02
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2012 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGANIMATIONDEF_H_
#define HGANIMATIONDEF_H_

#include "HGSystem.h"

namespace HG
{
    
typedef enum
{
    ANIMATION_ID_INVALID = 0,
    ANIMATION_ID_TRANSLATE,
    ANIMATION_ID_ROTATE,
    ANIMATION_ID_SCALE,
    ANIMATION_ID_TRANSPARENT,
    ANIMATION_ID_TEXTURE,
    ANIMATION_ID_COUNT
}AnimationTypeID;

typedef enum 
{
    ANIMATION_TYPE_DEFAULT  = 0,
    ANIMATION_TYPE_RESET,       // 123123123
    ANIMATION_TYPE_PINGPONG,    // 123212321

    ANIMATION_TYPE_COUNT,
}ANIMATION_TYPE;

typedef enum
{
    ANIMATION_STAGE_FORWARD = 0,
    ANIMATION_STAGE_REVERSE,
}ANIMATION_STAGE;

typedef struct _sprite_animation_file_header_
{
    uint32_t desc;      // File description, hash
    uint32_t size;      // File size
    uint32_t number;    // Animation number
}sprite_animation_file_header_t;

typedef struct _sprite_frame_node_
{
    float sourceX;
    float sourceY;
    float z;
    float width;
    float height;
    float offsetX;
    float offsetY;
    float delta;
    struct _sprite_frame_node_* next;
    struct _sprite_frame_node_* prev;
}sprite_frame_node_t;

typedef struct _sprite_frames_header_
{
    uint32_t name;
    uint32_t type;
    uint32_t fileNameHash;
    sprite_frame_node_t* pFrames;

    struct _sprite_frames_header_* next;
}sprite_frames_header_t;

}

#endif // HGANIMATIONDEF_H_
