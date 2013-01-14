/**
 *  @file    HGISceneEntity.cpp
 *  @brief   
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/08/09
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

#include "HGISceneEntity.h"
#include "HGSceneNode.h"
#include "HGIAnimation.h"

namespace HG
{

ISceneEntity::ISceneEntity()
	: mSceneNode(NULL)
{
}

ISceneEntity::~ISceneEntity()
{
	if (mSceneNode != NULL)
	{
		mSceneNode->detachEntity();
	}
    
    removeAllAnimations();
}

void ISceneEntity::detachFromSceneNode()
{
	if (mSceneNode == NULL)
		return;

	mSceneNode->detachEntity();
	mSceneNode = NULL;
}
    
// Animation
void ISceneEntity::updateAnimations(const float dt)
{
    BREAK_START;
    
    if (mAnimations.empty())
        break;
    
    AnimationList::iterator iter = mAnimations.begin();
    for (; iter != mAnimations.end(); )
    {
        if ((*iter)->isFinished)
        {
            mAnimationSet.erase((*iter)->name);
            
            delete *iter;
            mAnimations.erase(iter++);
        }
        else
        {
            (*iter)->update(mSceneNode, dt);
            ++iter;
        }
    }
    
    BREAK_END;
}
    
void ISceneEntity::addAnimation(IAnimation* anim, uint32_t name)
{
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    name = (name == 0) ? anim->name : name;
    
    if (mAnimationSet.find(name) != mAnimationSet.end())
        break;
    
    mAnimationSet.insert(name);
    mAnimations.push_back(anim);
    
    BREAK_END;
}
    
IAnimation* ISceneEntity::getAnimation(uint32_t name)
{
    IAnimation* ret = NULL;
    
    BREAK_START;
    
    if (name == 0)
        break;
    
    AnimationList::iterator iter = mAnimations.begin();
    for (; iter != mAnimations.end(); ++iter)
    {
        if ((*iter)->name == name)
        {
            ret = *iter;
            break;
        }
    }
    
    BREAK_END;
    
    return ret;
}
    
void ISceneEntity::removeAnimation(uint32_t name)
{
    BREAK_START;
    
    if (name == 0)
        break;
    
    // find animation in set
    AnimationSet::iterator setIter = mAnimationSet.find(name);
    if (setIter == mAnimationSet.end())
        break;
    
    mAnimationSet.erase(setIter);
    
    AnimationList::iterator listIter = mAnimations.begin();
    for (; listIter != mAnimations.end(); ++listIter)
    {
        if ((*listIter)->name == name)
        {
            delete *listIter;
            mAnimations.erase(listIter);
            break;
        }
    }
    
    BREAK_END;
}
    
void ISceneEntity::removeAllAnimations()
{
    BREAK_START;
    
    if (mAnimations.empty())
        break;

    AnimationList::iterator iter = mAnimations.begin();
    for (; iter != mAnimations.end(); ++iter)
    {
        delete *iter;
    }
    
    mAnimations.clear();
    mAnimationSet.clear();
    
    BREAK_END;
}
    
/*
void ISceneEntity::_removeFinishedAnimation()
{
    BREAK_START;
    
    if (mAnimations.empty())
        break;
    
    AnimationList::iterator iter = mAnimations.begin();
    for (; iter != mAnimations.end(); )
    {
        if ((*iter)->isFinished)
        {
            delete *iter;
            mAnimations.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
    
    BREAK_END;
}
*/
    
}