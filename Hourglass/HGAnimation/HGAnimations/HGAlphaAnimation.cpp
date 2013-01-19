/**
 *  @file    HGAlphaAnimation.cpp
 *  @brief   Alpha animation implementation
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

#include "HGAlphaAnimation.h"
#include "HGSceneNode.h"

HGNAMESPACE_START
    
AlphaAnimation::AlphaAnimation(float alpha, float duration)
    : mDuration(duration)
    , mAlpha(alpha)
    , mCurrent(0.0f)
{
    typeID = ANIMATION_ID_ALPHA;
    type = ANIMATION_TYPE_DEFAULT;
    name = 0;
}
    
void AlphaAnimation::update(SceneNode *node, float dt)
{
    if (node == NULL || !isAnimating || isFinished)
        return;
    
    if (mDuration == 0.0f)
    {
        isFinished = true;
        return;
    }
    
    float step = mAlpha / mDuration * dt;
    
    if (fabsf(mCurrent + step) >= fabsf(mAlpha))
    {
        step = mAlpha - mCurrent;
        isFinished = true;
    }
    
    mCurrent += step;
    
    float alpha = node->getAlpha() + step;
    node->setAlpha(alpha);
    
    if (isFinished)
    {
        if (type == ANIMATION_TYPE_RESET)
        {
            float a = node->getAlpha() - mAlpha;
            node->setAlpha(a);
            mCurrent = 0.0f;
            isFinished = false;
        }
        else if (type == ANIMATION_TYPE_PINGPONG)
        {
            mCurrent = 0.0f;
            mAlpha = -mAlpha;
            isFinished = false;
        }
    }
}
    
HGNAMESPACE_END
