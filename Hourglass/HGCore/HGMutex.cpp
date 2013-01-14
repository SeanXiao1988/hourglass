/**
 *  @file    HGMutex.cpp
 *  @brief   Mutex implementation
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

#include "HGMutex.h"

namespace HG
{
    
Mutex::Mutex()
{
    mutex = glfwCreateMutex();
}
    
Mutex::~Mutex()
{
    if (mutex)
    {
        glfwDestroyMutex(mutex);
    }
    
    mutex = NULL;
}
    
void Mutex::lock()
{
    glfwLockMutex(mutex);
}

void Mutex::unlock()
{
    glfwUnlockMutex(mutex);
}
    
}