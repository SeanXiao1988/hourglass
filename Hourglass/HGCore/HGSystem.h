/**
 *  @file    HGSystem.h
 *  @brief   Standard Library Headers
 *
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/04/27
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

#ifndef HGSYSTEM_H_
#define HGSYSTEM_H_

/// Common standard library headers
#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

/// Common STL headers
#include <algorithm>
#include <deque>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

// Platform define

#define PLATFORM_UNSUPPORTED    0
#define PLATFORM_WINDOWS        1
#define PLATFORM_UNIX           2
#define PLATFORM_MACOS          3
#define PLATFORM_IPHONE         4
#define PLATFORM_IOS_SIMULATOR  5

#if defined(WIN32) || defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define PLATFORM PLATFORM_IPHONE
    #elif TARGET_IPHONE_SIMULATOR
        #define PLATFORM PLATFORM_IOS_SIMULATOR
    #elif TARGET_OS_MAC
        #define PLATFORM PLATFORM_MACOS
    #else
        #define PLATFORM PLATFORM_UNSUPPORTED
    #endif
#elif __linux
    #define PLATFORM PLATFORM_UNIX
#elif __unix // all unices not caught above
    #define PLATFORM PLATFORM_UNIX
//#elif __posix
// POSIX
#else
    #define PLATFORM PLATFORM_UNSUPPORTED
#endif

#if PLATFORM == PLATFORM_WINDOWS

    #pragma warning(disable : 4081)
    #pragma warning(disable : 4706)
    #pragma warning(disable : 4819)
	#pragma warning(disable : 4996)

    #define WIN32_MEAN_AND_LEAN
    #include <windows.h>

    #define M_PI 3.14159265358979323846264338327950288
    #include "stdint.h"

    #pragma comment(lib, "glew32s.lib")
    #pragma comment(lib, "glfw.lib")
    #pragma comment(lib, "opengl32.lib")

    #define GLEW_STATIC

#endif

#include "glew.h"
#include "glfw.h"

/// GLM
#include "glm.hpp"
#include "matrix_transform.hpp"

/// Lode png
#include "lodepng.h"

/// FreeType
#define FTGL_LIBRARY_STATIC
#include <FTGL/ftgl.h>

#if PLATFORM == PLATFORM_WINDOWS
    #pragma comment(lib, "ftgl_static.lib")
#endif

/// Audio
#if PLATFORM == PLATFORM_MACOS
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#elif PLATFORM == PLATFORM_WINDOWS
	#include "al.h"
	#include "alc.h"
	#pragma comment(lib, "OpenAL32.lib")
	#pragma comment(lib, "libvorbis_static.lib")
	#pragma comment(lib, "libvorbisfile_static.lib")
	#pragma comment(lib, "libogg_static.lib")
#endif

#include "vorbisfile.h"

#include "HGMacros.hpp"

#include "HGMemTrack.h"

#endif // HGSYSTEM_H_
