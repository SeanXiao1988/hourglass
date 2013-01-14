/**
 *  @file    HGTime.cpp
 *  @brief   Time implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/05/07
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

#include "HGSystem.h"

namespace HG
{

#ifdef __APPLE__
#include <sys/time.h>
static timeval gsInitial;
static bool gsInitializedTime = false;
#elif defined(__LINUX__)
#include <sys/timeb.h>
static long gsInitialSec = 0;
static long gsInitialUSec = 0;
static long gsInitializedTime = 0;
#elif defined(WIN32)
static bool gsIsPerformanceTimer = false;
static bool gsIsInitialized;
static LARGE_INTEGER	gsFrequency;
static LARGE_INTEGER	gsPerfInitTime;
static DWORD			gsNonPerfInitTime = 0;
#endif // __APPLE__

//------------------------------------------------------------------------------------------
int64_t GetTimeInMicroseconds()
{
#ifdef __APPLE__
    if (!gsInitializedTime)
    {
        gettimeofday(&gsInitial, 0);
        gsInitializedTime = true;
    }

    struct timeval currentTime;
    gettimeofday(&currentTime, 0);

    struct timeval deltaTime;
    timersub(&currentTime, &gsInitial, &deltaTime);

    return 1000000 * deltaTime.tv_sec + deltaTime.tv_usec;
#elif defined (__LINUX__)
    struct timeb currentTime;

    if (!gsInitializedTime)
    {
        ftime(&currentTime);
        gsInitialSec = (long)currentTime.time;
        gsInitialUSec = 1000 * currentTime.millitm;
        gsInitializedTime = true;
    }

    ftime(&currentTime);
    long currentSec = (long)currentTime.time;
    long currentUSec = 1000 * currentTime.millitm;
    long deltaSec = currentSec - gsInitialSec;
    long deltaUSec = currentUSec - gsInitialUSec;
    if (deltaUSec < 0)
    {
        deltaUSec += 1000000;
        --deltaSec;
    }

    return 1000000 * deltaSec + deltaUSec;
#elif defined(WIN32)
	if (!gsIsInitialized)
	{	
		if (QueryPerformanceFrequency(&gsFrequency))
			gsIsPerformanceTimer = true;
	}

	if (!gsIsInitialized)
	{
		if (gsIsPerformanceTimer)
		{
			QueryPerformanceFrequency(&gsFrequency);
			QueryPerformanceCounter(&gsPerfInitTime);
		}
		else
		{
			gsNonPerfInitTime = GetTickCount();
		}

		gsIsInitialized = true;
	}

	if (gsIsPerformanceTimer)
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		LONGLONG llInterval = (currentTime.QuadPart - gsPerfInitTime.QuadPart)/(gsFrequency.QuadPart);
		return (int64_t)(llInterval * 1000000);
	}
	else
	{
		DWORD currentTime;
		// TODO: GetTickCount 47days problem
		// http://msdn.microsoft.com/en-us/library/ms724408
		currentTime = GetTickCount();
		
		return (int64_t)((currentTime - gsNonPerfInitTime) * 1000);
	}

#endif // __APPLE__
}
//------------------------------------------------------------------------------------------
double GetTimeInSeconds()
{
    int64_t microseconds = GetTimeInMicroseconds();
    return 1e-06 * microseconds;
}
//------------------------------------------------------------------------------------------

}
