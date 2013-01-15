/**
 *  @file    HGScaleAnimation.cpp
 *  @brief   Scale animation implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/15
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

#include "HGScaleAnimation.h"
#include "HGSceneNode.h"

namespace HG
{
    
ScaleAnimation::ScaleAnimation(float sx, float sy, float duration)
    : mDuration(duration)
    , mScaleX(sx)
    , mScaleY(sy)
    , mCurrentX(0.0)
    , mCurrentY(0.0)
    , mIsFinishX(false)
    , mIsFinishY(false)
{
    typeID = ANIMATION_ID_SCALE;
    type = ANIMATION_TYPE_DEFAULT;
    name = 0;
    host = NULL;
}
    
void ScaleAnimation::setScale(float sx, float sy)
{
    mScaleX = sx;
    mScaleY = sy;
}
    
void ScaleAnimation::update(SceneNode *node, float dt)
{
    if (node == NULL || !isAnimating || isFinished)
        return;
    
    if (mDuration == 0.0f)
    {
        isFinished = true;
        return;
    }
    
    float stepX = mScaleX / mDuration * dt;
    float stepY = mScaleY / mDuration * dt;
    
    if (fabsf(mCurrentX + stepX) >= fabsf(mScaleX))
    {
        stepX = mScaleX - mCurrentX;
        mIsFinishX = true;
    }
    
    if (fabsf(mCurrentY + stepY) >= fabsf(mScaleY))
    {
        stepY = mScaleY - mCurrentY;
        mIsFinishY = true;
    }
    
    mCurrentX += stepX;
    mCurrentY += stepY;
    
    float sx = node->getScaleX() + stepX;
    float sy = node->getScaleY() + stepY;
    
    node->setScale(sx, sy);
    
    if (mIsFinishX && mIsFinishY)
    {
        if (type == ANIMATION_TYPE_RESET)
        {
            float rx = node->getScaleX() - mScaleX;
            float ry = node->getScaleY() - mScaleY;
            node->setScale(rx, ry);
            mCurrentX = 0.0f;
            mCurrentY = 0.0f;
            mIsFinishX = false;
            mIsFinishY = false;
        }
        else if (type == ANIMATION_TYPE_PINGPONG)
        {
            mCurrentX = 0.0f;
            mCurrentY = 0.0f;
            mScaleX = -mScaleX;
            mScaleY = -mScaleY;
            mIsFinishX = false;
            mIsFinishY = false;
        }
        else
        {
            isFinished = true;
        }
    }
}
    
}