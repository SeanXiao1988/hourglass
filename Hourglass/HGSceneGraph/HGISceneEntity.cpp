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

HGNAMESPACE_START

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

}

void ISceneEntity::detachFromSceneNode()
{
	if (mSceneNode == NULL)
		return;

	mSceneNode->detachEntity();
	mSceneNode = NULL;
}
    
HGNAMESPACE_END