/**
 *  @file    HGScaleAnimation.h
 *  @brief   Scale animation header
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

#ifndef HGSCALEANIMATION_H_
#define HGSCALEANIMATION_H_

#include "HGIAnimation.h"

namespace HG
{

class ScaleAnimation : public IAnimation
{
public:
    explicit ScaleAnimation(float sx, float sy, float duration);
    ~ScaleAnimation() {}
    
    virtual void update(SceneNode* node, float dt);
    
    void setScale(float sx, float sy);
    void setDuration(float duration) { mDuration = duration; }
    void setType(ANIMATION_TYPE t) { type = t; }
    
private:
    float   mScaleX;
    float   mScaleY;
    float   mCurrentX;
    float   mCurrentY;
    float   mDuration;
    bool    mIsFinishX;
    bool    mIsFinishY;
};

}

#endif // HGSCALEANIMATION_H_
