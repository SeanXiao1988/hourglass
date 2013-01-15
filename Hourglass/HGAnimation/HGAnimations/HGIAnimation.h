/**
 *  @file    HGIAnimation.h
 *  @brief   Animation interface
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/11
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

#ifndef HGIANIMATION_H_
#define HGIANIMATION_H_

#include "HGAnimationDef.h"

namespace HG
{
    
class SceneNode;
class ISceneEntity;

class IAnimation
{
public:
    IAnimation();
    virtual ~IAnimation() {};
    virtual void update(SceneNode* node, float dt) = 0;
    
    bool            isFinished;
    bool            isAnimating;
    ANIMATION_STAGE animationStage;

    uint32_t        name;
    AnimationTypeID typeID;
    ANIMATION_TYPE  type;
};

}

#endif // HGIANIMATION_H_
