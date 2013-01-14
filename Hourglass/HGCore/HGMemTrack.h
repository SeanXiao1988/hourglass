/**
 *  @file    HGMemTrack.h
 *  @brief   Memory track
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  reference: http://www.almostinfinite.com/memtrack.html
 *
 *  @internal
 *  Created:  2012/10/17
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

#ifndef HGMEMTRACK_H_
#define HGMEMTRACK_H_

#include "HGSystem.h"
#include <typeinfo>

namespace HG
{
    
typedef struct _memory_log
{
    void*   address;
    size_t  size;
    int     linenum;
    int     isChunk;    // is the memory alloc by new[] operator
    char    filename[128];
    char    type[128];
    _memory_log* next;
}memory_log_t;
        
extern void InitializeMemoryLog();
extern size_t QueryMemoryUsage(int log = 0);
extern void DeInitializeMemoryLog();
    
class MemLog
{
public:
    MemLog(const char* filename, int linenum);
    ~MemLog();
    
    int tag;
};
    
void TrackLog(const MemLog& memLog, void *p, char const *typeName);
void PrintMemoryLeak();

template <class T> inline T* operator*(const MemLog& log, T *p)
{
    TrackLog(log, p, typeid(T).name());
    return p;
}

class AutoMemoryTag
{
public:
	AutoMemoryTag() {InitializeMemoryLog();}
	~AutoMemoryTag() {PrintMemoryLeak(); DeInitializeMemoryLog();}
};
    
}

#ifdef new
#undef new
#endif

#define HG_NEW HG::MemLog(__FILE__, __LINE__) * new
#define new HG_NEW

#define MALLOC(x) malloc(x)
#define FREE(x) free(x)

#endif // HGMEMTRACK_H_
