/**
 *  @file    HGAnimationManager.h
 *  @brief   Animation Manager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/03/02
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

#ifndef HGANIMATIONMANAGER_H_
#define HGANIMATIONMANAGER_H_

#include "HGIAnimation.h"

#define ANIMATIONMANAGER AnimationManager::getInstance()

HGNAMESPACE_START

typedef std::list<IAnimation*> IAnimationList;

class AnimationManager
{
public:
    static AnimationManager& getInstance()
    {
        static AnimationManager instance;
        return instance;
    }
    
    void        initialize();
    void        deInitialize();
    
private:
    friend class IAnimation;
    
    void        addAnimation(IAnimation* anim);
    void        removeAnimation(IAnimation* anim);
    void        removeAll();
    
    bool        isRemoveAll;    // TODO: this is bullshit!
    IAnimationList  mAnimationList;
    
private:
    AnimationManager() {}
    ~AnimationManager() {}
    AnimationManager(const AnimationManager& other);
    AnimationManager& operator= (const AnimationManager& rhs);
};

HGNAMESPACE_END

#endif // HGANIMATIONMANAGER_H_
