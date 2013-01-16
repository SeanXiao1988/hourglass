/**
 *  @file    HGRotationAnimation.h
 *  @brief   Rotation animation header
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

#ifndef HGROTATIONANIMATION_H_
#define HGROTATIONANIMATION_H_

#include "HGIAnimation.h"

namespace HG
{
    
class RotationAnimation : public IAnimation
{
public:
    explicit RotationAnimation(float rotation, float duration);
    ~RotationAnimation();
    
    virtual void update(SceneNode* node, float dt);
    
    void setRotation(float rot);
    void setDuration(float duration) { mDuration = duration; }
    
private:
    float   mRotation;
    float   mCurrentRot;
    float   mDuration;
};
    
}

#endif // HGROTATIONANIMATION_H_
