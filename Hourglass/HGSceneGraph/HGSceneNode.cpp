/**
 *  @file    HGSceneNode.cpp
 *  @brief   Scene node implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/06/15
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

#include "HGSystem.h"

#include "HGHash.h"
#include "HGSceneNode.h"
#include "HGIAnimation.h"
#include "HGISceneEntity.h"
#include "HGObjectManager.h"
#include "HGSceneManager.h"

#include "HGRender.h"

namespace HG
{
    
void SceneNode::RegisterComponentType()
{

}
    
void SceneNode::deInitialize()
{

}

EventResult SceneNode::handleEvent(const Event& event)
{
    return EVENT_RESULT_IGNORED;
}
    
uint32_t SceneNode::getComponentName()
{
    return Hash("SceneNode");
}
    
// Class
SceneNode::SceneNode()
    : mAlpha(255.0f)
    , mRenderAlpha(0)
    , mZ(0.0f)
    , mX(0.0f)
    , mY(0.0f)
    , mScaleX(1.0f)
    , mScaleY(1.0f)
    , mRotation(0.0f)
    , mVisibility(true)
    , mWorldX(0.0f)
    , mWorldY(0.0f)
    , mIsWorldXNeedSet(false)
    , mIsWorldYNeedSet(false)
    , mDestWorldX(0.0f)
    , mDestWorldY(0.0f)
    , mParent(NULL)
    , mEntity(NULL)
{
}
    
SceneNode::~SceneNode()
{
	if (mParent != NULL)
	{
		mParent->removeChild(this);
	}

	removeAll();
    
    removeAllAnimations();
}
    
void SceneNode::render()
{
    if (!mVisibility)
        return;
    
    glm::mat4 mat(1.0f);
    mRenderAlpha = mAlpha;
    
    if (mParent != NULL)
    {
        mat *= mParent->getMatrix();
        mRenderAlpha = mRenderAlpha * (mParent->getAlpha()/255.0f);
    }
    
    mat *= glm::translate(glm::mat4(1.0f), glm::vec3(mX, mY, mZ));
    mat *= glm::rotate(glm::mat4(1.0f), mRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    mat *= glm::scale(glm::mat4(1.0f), glm::vec3(mScaleX, mScaleY, 1.0f));

    glm::mat4 skew(1.0f);
    skew[1][0] = tanf(0.5);
    //skew[0][1] = tanf(0.5);
    
    mat *= skew;
    
    // Magic number...
    // extract the x, y coordinates
    mWorldX = mat[3][0];
    mWorldY = mat[3][1];
    
    // set world coordinates
	if (mIsWorldXNeedSet)
	{
		mWorldX += mDestWorldX - mWorldX;
		mIsWorldXNeedSet = false;
		mat[3][0] = mWorldX;
	}
    
	if (mIsWorldYNeedSet)
	{
		mWorldY += mDestWorldY - mWorldY;
		mIsWorldYNeedSet = false;
		mat[3][1] = mWorldY;
	}
    
    // update matrix
    mMatrix = mat;
    
    if (mParent != NULL)
    {
        RENDER.renderLine(mWorldX, mWorldY, mParent->getWorldX(), mParent->getWorldY(), 0xFF0000FF, 1.0f);
    }
    
    // pass "this" to render entity
    if (mEntity != NULL)
    {
        mEntity->render();
    }
    
    // Do render here
    if (!mChildren.empty())
    {
        for (SceneNodeList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
        {
            (*iter)->render();
        }
    }
}

void SceneNode::update(const float dt)
{
    updateAnimations(dt);
    
	// pass "this" to render entity
	if (mEntity != NULL)
		mEntity->update(dt);

	if (!mChildren.empty())
	{
		for (SceneNodeList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			(*iter)->update(dt);
		}
	}
}

void SceneNode::setWorldX(float x)
{
	mDestWorldX = x;
	mIsWorldXNeedSet = true;
}

void SceneNode::setWorldY(float y)
{
	mDestWorldY = y;
	mIsWorldYNeedSet = true;
}
    
void SceneNode::setWorldXY(float x, float y)
{
    mDestWorldX = x;
    mDestWorldY = y;
    mIsWorldXNeedSet = true;
    mIsWorldYNeedSet = true;
}
    
// Animation
void SceneNode::updateAnimations(const float dt)
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
            (*iter)->update(this, dt);
            ++iter;
        }
    }
    
    BREAK_END;
}

void SceneNode::addAnimation(IAnimation* anim, uint32_t name)
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

IAnimation* SceneNode::getAnimation(uint32_t name)
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

void SceneNode::removeAnimation(uint32_t name)
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

void SceneNode::removeAllAnimations()
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

// Scene Graph
SceneNode* SceneNode::getParent()
{
	return mParent;
}

void SceneNode::setParent(SceneNode* node)
{
    if (mParent == node)
        return;
    
	if (mParent != NULL && node != NULL)
	{
		mParent->removeChild(this);
        mParent = NULL;
        node->addChild(this);
	}

	mParent = node;
}

void SceneNode::addChild(SceneNode* node)
{
	if (node == NULL)
		return;

	// avoid add a child twice
	for (SceneNodeList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
	{
		if (*iter == node)
		{
			return;
		}
	}

	node->setParent(this);
	mChildren.push_back(node);
}

void SceneNode::removeChild(SceneNode* node)
{
	if (node == NULL || mChildren.empty())
		return;

	for (SceneNodeList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
	{
		if (*iter == node)
		{
			(*iter)->setParent(NULL);
			mChildren.erase(iter);
			break;
		}
	}
}

void SceneNode::removeAll()
{
	if (mChildren.empty())
		return;

	for (SceneNodeList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
	{
		(*iter)->setParent(NULL);
	}

	mChildren.clear();
}

SceneNode* SceneNode::getChild(const std::string& name)
{
	uint32_t hashName = Hash(name);
	return getChild(hashName);
}
    
SceneNode* SceneNode::getChild(uint32_t hashName)
{
    SceneNode* ret = NULL;
    
	if (!mChildren.empty())
	{
		for (size_t i = 0; i < mChildren.size(); ++i)
		{
			if (hashName == mChildren[i]->mHashName)
			{
				ret = mChildren[i];
				break;
			}
		}
	}
    
	return ret;
}

void SceneNode::attachEntity(ISceneEntity* entity)
{
	if (mEntity != NULL)
	{
		mEntity->detachFromSceneNode();
	}

	entity->mSceneNode = this;
	mEntity = entity;
}

void SceneNode::detachEntity()
{
	if (mEntity != NULL)
	{
		mEntity->mSceneNode = NULL;
		mEntity = NULL;
	}
}

}
