/**
 *  @file    HGMemTrack.cpp
 *  @brief   Memory track implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
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

#include "HGMemTrack.h"
#include <new>
#undef new    // IMPORTANT!

// step 1. MemLog()
// step 2. new
// step 3. operator *
// step 4. TrackLog

#define HG_MEMTRACK_MAGIC 0xDEADBEEF
#define HG_MEM_ALLOC_CHUNK      1
#define HG_MEM_ALLOC_NOT_CHUNK  0

namespace HG
{
    
static memory_log_t MEMORYLOG;
    
memory_log_t* _create_log_nod(const char* filename, int linenum)
{
    memory_log_t * newlog = (memory_log_t *)malloc(sizeof(memory_log_t));
    memset(newlog, 0, sizeof(memory_log_t));
    strcpy(newlog->filename, filename);
    newlog->linenum = linenum;
    
    return newlog;
}
    
void _remove_log_node(void *p, int isChunk = HG_MEM_ALLOC_NOT_CHUNK)
{
    memory_log_t * curNode = MEMORYLOG.next;
    memory_log_t * preNode = &MEMORYLOG;
    while (curNode!= NULL)
    {
        if (curNode->address == p && curNode->isChunk == isChunk)
        {
            preNode->next = curNode->next;
            MEMORYLOG.size -= curNode->size;
            free(curNode);
            break;
        }
        else
        {
            preNode = curNode;
            curNode = curNode->next;
        }
    }
}
    
memory_log_t* _find_node(void *p)
{
    memory_log_t *node = MEMORYLOG.next;
    if (node != NULL)
    {
        if (node->address == p)
            return node;
    }
    
    return NULL;
}
    
void _delete_top_node()
{
    memory_log_t *node = MEMORYLOG.next;
    if (node != NULL)
    {
        MEMORYLOG.next = node->next;
        free(node);
    }
}
    
void _set_top_node(void *p, size_t size, int isChunk = HG_MEM_ALLOC_NOT_CHUNK)
{
    if (HG::MEMORYLOG.linenum != HG_MEMTRACK_MAGIC)
    {
        return;
    }

    HG::memory_log_t* node = HG::MEMORYLOG.next;
    if (node != NULL)
    {
        node->address = p;
        node->size = size;
        node->isChunk = isChunk;
        HG::MEMORYLOG.linenum = 0;
        HG::MEMORYLOG.size += size;
    }
}
    
MemLog::MemLog(const char* filename, int linenum)
{
    // FIXME: wtf? rapidxml, wtf!
    std::string strFile(filename);
    std::string strRapidXML("rapidxml.hpp");
    size_t found = strFile.find(strRapidXML);
    if ( found!=std::string::npos )
        return;
        
    memory_log_t* log = _create_log_nod(filename, linenum);
    log->next = MEMORYLOG.next;
    MEMORYLOG.next = log;
    MEMORYLOG.linenum = HG_MEMTRACK_MAGIC;
}

MemLog::~MemLog()
{
}
    
void InitializeMemoryLog()
{
    memset(&MEMORYLOG, 0, sizeof(memory_log_t));
    MEMORYLOG.next = NULL;
}
    
size_t QueryMemoryUsage(int log)
{
    if (log)
    {
        HGLog("------- MEMORY USAGE LOG -------\n");
        memory_log_t * log = MEMORYLOG.next;
        while (log != NULL)
        {
            HGLog("file:%s | line: %d | size: %ld @[0x%08zx]\n", log->filename, log->linenum, log->size, (size_t)log->address);
            log = log->next;
        }
        
        HGLog("Total Lived bytes: %zd\n", MEMORYLOG.size);
    }
    
    return MEMORYLOG.size;
}
    
void DeInitializeMemoryLog()
{
    memory_log_t * log = MEMORYLOG.next;
    while (log != NULL)
    {
        MEMORYLOG.next = log->next;
        free(log);
        log = MEMORYLOG.next;
    }
}
    
void TrackLog(const MemLog& memLog, void *p, char const *typeName)
{
    memory_log_t* node = _find_node(p);
    if (node != NULL)
    {
        strcpy(node->type, typeName);
    }
}
    
void PrintMemoryLeak()
{
    HGLog("------- MEMORY LEAK LOG -------\n");
    memory_log_t * log = MEMORYLOG.next;
    while (log != NULL)
    {
        // FIXME: size == 0 ?? WTF ???
        if (log->size != 0)
        {
            HGLog("file:%s | line: %d | size: %ld @[0x%08zx]\n", log->filename, log->linenum, log->size, (size_t)log->address);
        }
        log = log->next;
    }
}
    
} // namespace HG

void *operator new(size_t size)
{
    void *p = malloc(size);
    if (p == NULL) throw std::bad_alloc();
    
    HG::_set_top_node(p, size);
    
    return p;
}

/* ---------------------------------------- operator delete */

void operator delete(void *p)
{
    if (p == NULL)
        return;
    
    HG::_remove_log_node(p);
    
    free(p);
}

/* ---------------------------------------- operator new[] */

void *operator new[](size_t size)
{
    void *p = malloc(size);
    if (p == NULL) throw std::bad_alloc();
    
    HG::_set_top_node(p, size, HG_MEM_ALLOC_CHUNK);
    
    return p;
}

/* ---------------------------------------- operator delete[] */

void operator delete[](void *p)
{
    if (p == NULL)
        return;
    
    HG::_remove_log_node(p, HG_MEM_ALLOC_CHUNK);
    
    free(p);
}
