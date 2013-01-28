/**
 *  @file    HGSceneManager.h
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

#include "HGSceneManager.h"
#include "HGSceneNode.h"
#include "HGHash.h"

#include "HGSystem.h"

HGNAMESPACE_START

SceneManager::SceneManager()
	: mRootNode(NULL)
{
}

SceneManager::~SceneManager()
{
	deInitialize();
}

void SceneManager::initialize()
{
	if (mRootNode != NULL)
	{
		delete mRootNode;
		mRootNode = NULL;
	}

	mRootNode = new SceneNode();
    mRootNode->mHashName = Hash("root");
	mRootNode->setXY(0, 0);
	mRootNode->setScale(1.0f, 1.0f);
	mRootNode->setZ(0);
	mRootNode->setVisibility(true);
	mRootNode->setRotation(0.0f);
}

void SceneManager::deInitialize()
{
	if (mRootNode != NULL)
	{
		delete mRootNode;
		mRootNode = NULL;
	}

	// deleteAll();
}

SceneNode* SceneManager::getRoot()
{
	return mRootNode;
}

void SceneManager::removeAll()
{
	if (mRootNode != NULL)
	{
		mRootNode->removeAll();
	}
}

void SceneManager::update(const float dt)
{
    mRootNode->update(dt);
}


void SceneManager::render()
{
	mRootNode->render();
}
    
void SceneManager::run(const float dt)
{
    update(dt);
    render();
}
    
void SceneManager::addNode(SceneNode *node)
{
    mNodeMap.insert(SceneNodeMap::value_type(node->getHashName(), node));
}
    
SceneNode* SceneManager::createNode(const std::string& name)
{
	SceneNode *node = new SceneNode();
    node->mHashName = Hash(name.c_str());
	mNodeMap.insert(SceneNodeMap::value_type(Hash(name.c_str()), node));

	return node;
}
    
SceneNode* SceneManager::getNode(const uint32_t hashedName)
{
    SceneNode *ret = NULL;
    
    BREAK_START;
    
    if (hashedName == Hash("root"))
    {
        ret = mRootNode;
        break;
    }

    if (mNodeMap.empty())
        break;
    
    SceneNodeMap::iterator sceneNodeMapIter = mNodeMap.find(hashedName);
    if (sceneNodeMapIter != mNodeMap.end())
        ret = (*sceneNodeMapIter).second;
    
    BREAK_END;
    
    return ret;
}

SceneNode* SceneManager::getNode(const std::string& name)
{
    return getNode(Hash(name));
}
/*
void SceneManager::deleteNode(const std::string& name)
{
    deleteNode(Hash(name));
}

void SceneManager::deleteNode(const uint32_t hashname)
{
   	SceneNodeMap::iterator iter = mNodeMap.find(hashname);
	if (iter != mNodeMap.end())
	{
		delete iter->second;
		mNodeMap.erase(iter);
	}
}
    
void SceneManager::deleteNode(SceneNode* node)
{
	if (node == NULL)
		return;

	SceneNodeMap::iterator iter = mNodeMap.find(node->getHashName());
	if (iter != mNodeMap.end())
	{
		delete iter->second;
		mNodeMap.erase(iter);
	}
}

void SceneManager::deleteAll()
{
	if (mNodeMap.empty())
		return;

	SceneNodeMap::iterator iter = mNodeMap.begin();
	for (; iter != mNodeMap.end(); ++iter)
	{
		delete iter->second;
	}

	mNodeMap.clear();
}
*/
HGNAMESPACE_END