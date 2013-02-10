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
    
}

void BackgroundEntity::render()
{
    if (mSceneNode == NULL)
    
    RENDER.renderQuad(&mQuad);
}

void BackgroundEntity::setType(BackgroundType type)
{
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