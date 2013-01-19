/**
 *  @file    HGMutex.h
 *  @brief   Mutex class header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/01
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

#ifndef HGMUTEX_H_
#define HGMUTEX_H_

#include "HGSystem.h"

HGNAMESPACE_START

class Mutex
{
public:
    Mutex();
    ~Mutex();
    
    void lock();
    void unlock();

private:

    GLFWmutex mutex;

};
    
HGNAMESPACE_END

#endif // HGMUTEX_H_
