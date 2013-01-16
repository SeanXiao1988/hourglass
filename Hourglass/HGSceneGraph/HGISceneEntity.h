/**
 *  @file    HGISceneEntity.h
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


#ifndef HGISCENEENTITY_H_
#define HGISCENEENTITY_H_

#include "HGSystem.h"
#include "HGEvent.h"
#include "HGIComponent.h"

namespace HG
{

class SceneNode;

class ISceneEntity : public IComponent
{
public:
	ISceneEntity();
	~ISceneEntity();

	virtual void render() = 0;
    virtual void update(const float dt) = 0;
    
    // Composite
    virtual EventResult     handleEvent(const Event& event) = 0;
    virtual ComponentTypeID getComponentTypeID() = 0;
    virtual uint32_t        getComponentName() = 0;

protected:
	void	detachFromSceneNode();
	SceneNode*	mSceneNode;
    
	friend class SceneNode;
};

}

#endif // HGISCENEENTITY_H_
