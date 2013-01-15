/**
 *  @file    HGAlphaAnimation.h
 *  @brief   Alpha animation header
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

#ifndef HGALPHAANIMATION_H_
#define HGALPHAANIMATION_H_

#include "HGIAnimation.h"

namespace HG
{
    
class AlphaAnimation : public IAnimation
{
public:
    explicit AlphaAnimation(float alpha, float duration);
    ~AlphaAnimation() {}
    
    virtual void update(SceneNode* node, float dt);
    
    void    setAlpha(float alpha) { mAlpha = alpha; }
    void    setDuration(float duration) { mDuration = duration; }
    
private:
    float   mAlpha;
    float   mCurrent;
    float   mDuration;
};
    
}

#endif // HGALPHAANIMATION_H_
