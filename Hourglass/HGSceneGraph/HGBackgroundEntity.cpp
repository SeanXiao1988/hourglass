/**
 *  @file    HGBackgroundEntity.cpp
 *  @brief   Background entity implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/07
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

#include "HGBackgroundEntity.h"
#include "HGSceneNode.h"
#include "HGRender.h"
#include "HGHash.h"

#include "HGGraphicsUtil.h"

HGNAMESPACE_START

// TODO: add background animation, parallel scroll etc.

BackgroundEntity::BackgroundEntity()
    : mType(BACKGROUND_DEFAULT)
    , mWidth(0.0f)
    , mHeight(0.0f)
    , mTexWidth(0.0f)
    , mTexHeight(0.0f)
{
    quad_set_default(&mQuad);
    quad_set_coord(&mQuad, 0.0f, 0.0f, (float)RENDER.getWidth(), (float)RENDER.getHeight());
}

BackgroundEntity::~BackgroundEntity()
{
    RENDER.textureFree(mQuad.tex);
}

void BackgroundEntity::update(const float dt)
{
    float parentX = -mSceneNode->getX();
    float parentY = -mSceneNode->getY();
    float screenW = (float)RENDER.getWidth();
    float screenH = (float)RENDER.getHeight();
    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = 0.0f;
    float v1 = 0.0f;

    u0 = parentX / mWidth;
    v0 = parentY / mHeight;
    u1 = (parentX+screenW)/mWidth;
    v1 = (parentY + screenH)/mHeight;
    
    switch (mType)
    {
        case BACKGROUND_DEFAULT:
        {
            if (u1 > 1.0f)
            {
                u0 = (mWidth - screenW)/mWidth;
                u1 = 1.0f;
            }
            if (v1 > 1.0f)
            {
                v0 = (mHeight - screenH)/mHeight;
                v1 = 1.0f;
            }
            break;
        }
            
        case BACKGROUND_REPEAT:
            break;
            
        case BACKGROUND_STATIC:
        {
            u0 = 0.0f;
            v0 = 0.0f;
            u1 = 1.0f;
            v1 = 1.0f;
            break;
        }
        default:
            break;
    }
    
    mQuad.v[0].u = u0;
    mQuad.v[0].v = v0;
    mQuad.v[1].u = u1;
    mQuad.v[1].v = v0;
    mQuad.v[2].u = u1;
    mQuad.v[2].v = v1;
    mQuad.v[3].u = u0;
    mQuad.v[3].v = v1;
}

void BackgroundEntity::render()
{
    RENDER.renderQuad(&mQuad);
}

void BackgroundEntity::setType(BackgroundType type)
{
    mType = type;
}

void BackgroundEntity::setTexture(GLuint tex)
{
    mQuad.tex = tex;
    if (tex != 0)
    {
        mTexWidth = RENDER.textureGetWidth(tex);
        mTexHeight = RENDER.textureGetHeight(tex);
    }
}

void BackgroundEntity::setTextureRect(float x, float y, float w, float h)
{
    quad_set_texture_rect(&mQuad, x, y, w, h, mTexWidth, mTexHeight);
}

void BackgroundEntity::setVertexAlpha(uint8_t alpha, int i)
{
    if (i == -1)
        quad_set_alpha(&mQuad, alpha);
    else
        mQuad.v[i].color[3] = alpha;
}

// Composite
void BackgroundEntity::RegisterComponentType(void)
{
    
}

void BackgroundEntity::deInitialize()
{
    
}

EventResult BackgroundEntity::handleEvent(const Event& event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t BackgroundEntity::getComponentName()
{
    return Hash("BackgroundEntity");
}

HGNAMESPACE_END