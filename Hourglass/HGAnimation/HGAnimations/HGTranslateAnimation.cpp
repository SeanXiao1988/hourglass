/**
 *  @file    HGTranslateAnimation.cpp
 *  @brief   Translate animation implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/13
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

#include "HGTranslateAnimation.h"
#include "HGSceneNode.h"

namespace HG
{
    
TranslateAnimation::TranslateAnimation(float x, float y, float duration)
    : mDuration(duration)
    , mTranslateX(x)
    , mTranslateY(y)
    , mCurrentX(0.0f)
    , mCurrentY(0.0f)
    , mIsFinishX(false)
    , mIsFinishY(false)
{
    typeID = ANIMATION_ID_TRANSLATE;
    type = ANIMATION_TYPE_DEFAULT;
    name = 0;
    host = NULL;
}
    
TranslateAnimation::~TranslateAnimation()
{
    
}
    
void TranslateAnimation::setTranslate(float x, float y)
{
    mTranslateX = x;
    mTranslateY = y;
}
    
void TranslateAnimation::update(SceneNode *node, float dt)
{
    if (node == NULL || !isAnimating || isFinished)
        return;
    
    if (mDuration == 0.0f)
    {
        isFinished = true;
        return;
    }
    
    float stepX = mTranslateX / mDuration * dt;
    float stepY = mTranslateY / mDuration * dt;
    
    if (fabsf(mCurrentX + stepX) >= fabsf(mTranslateX))
    {
        stepX = mTranslateX - mCurrentX;
        mIsFinishX = true;
    }
    
    if (fabsf(mCurrentY + stepY) >= fabsf(mTranslateY))
    {
        stepX = mTranslateY - mCurrentY;
        mIsFinishY = true;
    }
    
    mCurrentX += stepX;
    mCurrentY += stepY;
    
    float x = node->getX() + stepX;
    float y = node->getY() + stepY;

    node->setX(x);
    node->setY(y);
    
    if (mIsFinishX && mIsFinishY)
    {
        if (type == ANIMATION_TYPE_RESET)
        {
            float rx = node->getX() - mTranslateX;
            float ry = node->getY() - mTranslateY;
            node->setX(rx);
            node->setY(ry);
            mCurrentX = 0.0f;
            mCurrentY = 0.0f;
            mIsFinishX = false;
            mIsFinishY = false;
        }
        else if (type == ANIMATION_TYPE_PINGPONG)
        {
            mCurrentX = 0.0f;
            mCurrentY = 0.0f;
            mTranslateX = -mTranslateX;
            mTranslateY = -mTranslateY;
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
