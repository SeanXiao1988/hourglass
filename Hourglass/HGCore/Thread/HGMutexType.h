/**
 *  @file    HGMutexType.h
 *  @brief   cross platform definitions for HGMutex
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

#ifndef HGMUTEXTYPE_H_
#define HGMUTEXTYPE_H_

#include "HGSystem.h"

#if defined(WIN32)
// Use typecast to avoid include <Windows.h>
namespace HG
{

typedef void* MutexType;

}
//------------------------------------------------------------------------------------------
#elif defined(__LINUX__) || defined(__APPLE__)
#include <pthread.h>
namespace HG
{

typedef struct
{
    pthread_mutexattr_t Attribute;
    pthread_mutex_t     Mutex;
}MutexType;

}
#else
//------------------------------------------------------------------------------------------
// TODO: support for other platforms
//------------------------------------------------------------------------------------------
#error Other platforms not yet implemented.
#endif // WIN32

#endif // HGMUTEXTYPE_H_
