/**
 *  @file    HGThreadType.h
 *  @brief   Cross platform definitions for thread
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

#ifndef HGTHREADTYPE_H_
#define HGTHREADTYPE_H_

#include "HGSystem.h"

namespace HG
{

//------------------------------------------------------------------------------------------
#if defined(WIN32)

typedef void* ThreadType;
typedef DWORD LocalDataKeyType;

//------------------------------------------------------------------------------------------
#elif defined(__LINUX__) || (__APPLE__)
//------------------------------------------------------------------------------------------

typedef pthread_t ThreadType;
typedef pthread_key_t LocalDataKeyType;

//------------------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------------------
#error Other platforms not yet implemented.
#endif // WIN32

}

#endif // HGTHREADTYPE_H_
