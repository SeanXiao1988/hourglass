/**
 *  @file    HGAutoMutexScope.h
 *  @brief   Automatic Mutex Scope
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/05/03
 *  Company:  BackFire co.,ltd
 *  (C) Copyright 2012 BackFire co.,ltd All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of BackFire co.,ltd
 * The contents may be used and/or copied only with the written permission of
 * BackFire co.,ltd or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGAUTOMUTEXSCOPE_H_
#define HGAUTOMUTEXSCOPE_H_

#include "HGSystem.h"
#include "HGMutex.h"

namespace HG
{

class AutoMutexScope
{
public:
    AutoMutexScope(Mutex* mutex);
    ~AutoMutexScope();

private:
    Mutex* mMutex;
};

}

#endif // HGAUTOMUTEXSCOPE_H_
