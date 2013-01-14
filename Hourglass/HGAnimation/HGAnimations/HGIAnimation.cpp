/**
 *  @file    HGIAnimation.cpp
 *  @brief   IAnimation impletementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/13
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

#include "HGIAnimation.h"

namespace HG
{

IAnimation::IAnimation()
    : isFinished(false)
    , isAnimating(true)
    , animationStage(ANIMATION_STAGE_FORWARD)
    , name(0)
    , host(NULL)
    , typeID(ANIMATION_ID_INVALID)
    , type(ANIMATION_TYPE_DEFAULT)
{
}
    
}
