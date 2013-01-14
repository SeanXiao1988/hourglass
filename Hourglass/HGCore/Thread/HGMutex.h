/**
 *  @file    HGMutex.h
 *  @brief   Mutex for multi-threading lock
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/05/02
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

#ifndef HGMUTEX_H_
#define HGMUTEX_H_

#include "HGSystem.h"
#include "HGMutexType.h"

namespace HG
{

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void enter();
    void leave();

private:
    MutexType mMutex;
};

}

#endif // HGMUTEX_H_
