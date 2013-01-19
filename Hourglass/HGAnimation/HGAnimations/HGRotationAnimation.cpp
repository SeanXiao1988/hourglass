/**
 *  @file    HGRotationAnimation.cpp
 *  @brief   Rotation animation implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/14
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

#include "HGRotationAnimation.h"
#include "HGSceneNode.h"

HGNAMESPACE_START

RotationAnimation::RotationAnimation(float rotation, float duration)
    : mDuration(duration)
    , mRotation(rotation)
    , mCurrentRot(0.0f)
{
    typeID = ANIMATION_ID_ROTATE;
    type = ANIMATION_TYPE_DEFAULT;
    name = 0;
}
    
RotationAnimation::~RotationAnimation()
{
    
}
    
void RotationAnimation::setRotation(float rot)
{
    mRotation = rot;
}
    
void RotationAnimation::update(SceneNode *node, float dt)
{
    if (node == NULL || !isAnimating || isFinished)
        return;
    
    if (mDuration == 0.0f)
    {
        isFinished = true;
        return;
    }
    
    float step = mRotation / mDuration * dt;
    
    if (fabsf(mCurrentRot + step) >= fabsf(mRotation))
    {
        step = mRotation - mCurrentRot;
        isFinished = true;
    }
    
    mCurrentRot += step;
    
    float rot = node->getRotation() + step;
    
    node->setRotation(rot);
    
    if (isFinished)
    {
        if (type == ANIMATION_TYPE_RESET)
        {
            float r = node->getRotation() - mRotation;
            node->setRotation(r);
            mCurrentRot = 0.0f;
            isFinished = false;
        }
        else if (type == ANIMATION_TYPE_PINGPONG)
        {
            mCurrentRot = 0.0f;
            mRotation = -mRotation;
            isFinished = false;
        }
    }
}

HGNAMESPACE_END