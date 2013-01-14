/**
 *  @file    HGMutex.cpp
 *  @brief   Mutex implementation
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

#include "HGMutex.h"
#include "HGAssert.h"
using namespace HG;

#if defined(WIN32)
//------------------------------------------------------------------------------------------
Mutex::Mutex()
{
#ifdef HG_USE_CRITICAL_SECTIONS
    mMutex = new CRITICAL_SECTION;
    BOOL success = InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)mMutex, 4096);
    assertion(success == TRUE, "Failed to initialize critical section.\n");
    HG_UNUSED(success);
#else // HG_USE_CRITICAL_SECTIONS
    mMutex = CreateMutex(NULL, FALSE, NULL);
    assertion(mMutex != NULL, "Failed to create mutex.\n");
#endif // HG_USE_CRITICAL_SECTIONS
}
//------------------------------------------------------------------------------------------
Mutex::~Mutex()
{
#ifdef HG_USE_CRITICAL_SECTIONS
    DeleteCriticalSection((CRITICAL_SECTION *)mMutex);
    delete mMutex;
#else // HG_USE_CRITICAL_SECTIONS
    BOOL closed = CloseHandle((HANDLE)mMutex);
    assertion(closed == TRUE, "Failed to destroy mutex.\n");
    HG_UNUSED(closed);
#endif // HG_USE_CRITICAL_SECTIONS
}
//------------------------------------------------------------------------------------------
void Mutex::enter()
{
#ifdef HG_USE_CRITICAL_SECTIONS
    EnterCriticalSection((CRITICAL_SECTION *)mMutex);
#else // HG_USE_CRITICAL_SECTIONS
    DWORD result = WaitForSingleObject((HANDLE)mMutex, INFINITE);
    HG_UNUSED(result);
    // result:
    // WAIT_ABANDONED (0x00000080)
    // WAIT_OBJECT_0  (0x00000000), signaled
    // WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
    // WAIT_FAILED    (0xFFFFFFFF), not signaled
#endif // HG_USE_CRITICAL_SECTIONS
}
//------------------------------------------------------------------------------------------
void Mutex::leave()
{
#ifdef HG_USE_CRITICAL_SECTIONS
    LeaveCriticalSection((CRITICAL_SECTION *)mMutex);
#else // HG_USE_CRITICAL_SECTIONS
    BOOL released = ReleaseMutex((HANDLE)mMutex);
    HG_UNUSED(released);
#endif // HG_USE_CRITICAL_SECTIONS
}
//------------------------------------------------------------------------------------------
#elif defined(__LINUX__) || defined(__APPLE__)
//------------------------------------------------------------------------------------------
Mutex::Mutex()
{
    int result;
    HG_UNUSED(result);

    result = pthread_mutexattr_init(&mMutex.Attribute);
    // successful = 0
    // error = ENOMEM
    
    result = pthread_mutexattr_settype(&mMutex.Attribute, PTHREAD_MUTEX_RECURSIVE);
    // successful = 0
    
    result = pthread_mutex_init(&mMutex.Mutex, &mMutex.Attribute);
    // successful = 0
    // error = EAGAIN, ENOMEM, EPERM, EBUSY, EINVAL
}
//------------------------------------------------------------------------------------------
Mutex::~Mutex()
{
    int result;
    HG_UNUSED(result);

    result = pthread_mutex_destroy(&mMutex.Mutex);
    // successful = 0
    // errors = EINVAL

    result = pthread_mutexattr_destroy(&mMutex.Attribute);
    // successful = 0
    // errors = EBUSY, EINVAL
}
//------------------------------------------------------------------------------------------
void Mutex::enter()
{
    int result = pthread_mutex_lock(&mMutex.Mutex);
    HG_UNUSED(result);
    // successful = 0
    // errors = EINVAL, EDEADLK
}
//------------------------------------------------------------------------------------------
void Mutex::leave()
{
    int result = pthread_mutex_unlock(&mMutex.Mutex);
    HG_UNUSED(result);
    // successful = 0
    // errors = EINVAL, EPERM
}
//------------------------------------------------------------------------------------------
#else
#error Other platforms not yet implementation.
#endif // Platform
//------------------------------------------------------------------------------------------
