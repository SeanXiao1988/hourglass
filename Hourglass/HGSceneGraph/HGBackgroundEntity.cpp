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
    : mWidth(0.0f)
    , mHeight(0.0f)
{
    quad_set_default(&mQuad);
}

BackgroundEntity::~BackgroundEntity()
{
    
}

void BackgroundEntity::update(const float dt)
{
    
}

void BackgroundEntity::render()
{
    if (mSceneNode == NULL)
		return;
    
    setVertexAlpha((uint8_t)mSceneNode->getRenderAlpha());
    
    glm::mat4& mat = mSceneNode->getMatrix();
    glm::vec4 result = mat * glm::vec4(-mWidth/2.0f, -mHeight/2.0f, 1.0f, 1.0f);
    mQuad.v[0].x = result[0];
    mQuad.v[0].y = result[1];
    //mQuad.v[0].z = result[2];
    
    result =  mat * glm::vec4(mWidth/2.0f, -mHeight/2.0f, 1.0f, 1.0f);
    mQuad.v[1].x = result[0];
    mQuad.v[1].y = result[1];
    //mQuad.v[1].z = result[2];
    
    result = mat * glm::vec4(mWidth/2.0f, mHeight/2.0f, 1.0f, 1.0f);
    mQuad.v[2].x = result[0];
    mQuad.v[2].y = result[1];
    //mQuad.v[2].z = result[2];
    
    result = mat * glm::vec4(-mWidth/2.0f, mHeight/2.0f, 1.0f, 1.0f);
    mQuad.v[3].x = result[0];
    mQuad.v[3].y = result[1];
    //mQuad.v[3].z = result[2];
    
    RENDER.renderQuad(&mQuad);
}

void BackgroundEntity::setTexture(GLuint tex)
{
    mQuad.tex = tex;
}

void BackgroundEntity::setTextureRect(float x, float y, float w, float h)
{
    
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