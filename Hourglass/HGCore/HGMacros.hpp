/**
 *  @file    HGMacros.h
 *  @brief   Macros
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/29
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

#ifndef HGMACROS_H_
#define HGMACROS_H_

/// Utility Macros
#define BREAK_START do{
#define BREAK_END   }while(0)

#define HGNAMESPACE_START   namespace HG {
#define HGNAMESPACE_END     }

/// Log
#if PLATFORM == PLATFORM_WINDOWS
static void __cdecl debug_printf(const char *format, ...)
{
    char buf[4096], *p = buf;
    va_list args;
    va_start(args, format);
    p += _vsnprintf(p, sizeof buf - 1, format, args);
    va_end(args);
    while ( p > buf && isspace(p[-1]) )
        *--p = '\0';
    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';
    OutputDebugStringA(buf);
}

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#define HGLog(s, ...) debug_printf("[%s| %d |%s()]", __FILE__, __LINE__, __func__),debug_printf(s, ##__VA_ARGS__)

#elif PLATFORM == PLATFORM_MACOS
#define HGLog(s, ...) printf("[%s| %d |%s()]", __FILE__, __LINE__, __func__),printf(s, ##__VA_ARGS__)
#endif

#if PLATFORM == PLATFORM_WINDOWS
	// returns a random float between -1 and 1
	#define RANDOM_MINUS1_1()  ((2.0f*((float)rand()/RAND_MAX))-1.0f)
	// returns a random float between 0 and 1
	#define RANDOM_0_1() ((float)rand()/RAND_MAX)
#else
	// returns a random float between -1 and 1
	#define RANDOM_MINUS1_1() ((random() / (float)0x3fffffff )-1.0f)
	// returns a random float between 0 and 1
	#define RANDOM_0_1() ((random() / (float)0x7fffffff ))
#endif

// converts degrees to radians
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

// converts radians to degrees
#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

// max/min
template <class T>
const T& MAX(const T& a, const T& b)
{
    return (a > b ? a : b);
}

template <class T>
const T& MIN(const T& a, const T& b)
{
    return (a < b ? a : b);
}

template<class T, class U>
T CLAMP(T in, U low, U high)
{
    if(in <= low)
        return low;
    else if(in >= high)
        return high;
    else
        return in;
}

#endif // HGMACROS_H_
