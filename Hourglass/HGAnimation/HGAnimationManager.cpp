/**
 *  @file    HGAnimationManager.cpp
 *  @brief   Animation Manager implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/03/02
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

#include "HGAnimationManager.h"

HGNAMESPACE_START

void AnimationManager::initialize()
{
    isRemoveAll = false;
}

void AnimationManager::deInitialize()
{
    removeAll();
}

void AnimationManager::addAnimation(IAnimation* anim)
{
    mAnimationList.push_back(anim);
}

void AnimationManager::removeAnimation(IAnimation *anim)
{
    if (isRemoveAll)
        return;
    
    IAnimationList::iterator iter = mAnimationList.begin();
    for (; iter != mAnimationList.end(); ++iter)
    {
        if (*iter == anim)
        {
            mAnimationList.remove(*iter);
            return;
        }
    }
}

void AnimationManager::removeAll()
{
    isRemoveAll = true;
    
    IAnimationList::iterator iter = mAnimationList.begin();
    for (; iter != mAnimationList.end(); ++iter)
    {
        delete *iter;
    }
    
    mAnimationList.clear();
    
    isRemoveAll = false;
}

HGNAMESPACE_END