/**
 *  @file    HGTranslateAnimation.h
 *  @brief   Translate animation header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/12
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

#ifndef HGTRANSLATEANIMATION_H_
#define HGTRANSLATEANIMATION_H_

#include "HGIAnimation.h"

HGNAMESPACE_START

class TranslateAnimation : public IAnimation
{
public:
    explicit TranslateAnimation(float x, float y, float duration);
    ~TranslateAnimation();
    
    virtual void update(SceneNode* node, float dt);

    void setTranslate(float x, float y);
    void setDuration(float duration) { mDuration = duration; }
    
private:
    float   mTranslateX;
    float   mTranslateY;
    float   mCurrentX;
    float   mCurrentY;
    float   mDuration;
    bool    mIsFinishX;
    bool    mIsFinishY;
};

HGNAMESPACE_END

#endif // HGTRANSLATEANIMATION_H_