/**
 *  @file    HGThreadManager.h
 *  @brief   Thread Manager
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

// TODO: http://software.intel.com/en-us/articles/threaded-cross-platform-game-development/

#ifndef HGTHREADMANAGER_H_
#define HGTHREADMANAGER_H_

#include "HGSystem.h"
#include "HGThreadType.h"
#include "HGMutex.h"

namespace HG
{

class ThreadManager
{
public:
    typedef void (*ThreadFunction)();
    typedef void (*ThreadFinalizeFunc)();

    static const int INVALID_THREAD_ID = -1;

    static ThreadManager &getInstance(void);

    static void yield(void);
    static void sleep(int milliseconds);

    // non thread-safe
    bool startup(int threadPoolSize, void* primaryThreadLocalData = NULL);
    void shutdown(void);
    int assignThreadToFunction(ThreadFunction func, ThreadFinalizeFunc finalizeFunc = NULL, void* threadLocalData = NULL);
    bool unassignThread(int id);

    bool startThread(int id);
    bool stopThread(int id);

    // thread-safe
    int getThreadPoolSize(void) const;
    void* getLocalData(void) const;
    bool isPrimaryThread(void) const;
    int getAndResetThreadCalls(int id);

    
private:
    // constants for thread status
    enum {
        THREAD_STATUS_ALL               = ~0,
        THREAD_STATUS_UNASSIGNED        = 1 << 0,
        THREAD_STATUS_STOPPED           = 1 << 1,
        THREAD_STATUS_RUNNING           = 1 << 2,
        THREAD_STATUS_RUNNING_REQUEST   = 1 << 3,
        THREAD_STATUS_UNASSIGN_REQUEST  = 1 << 4,
        THREAD_STATUS_TERMINATE_REQUEST = 1 << 5,
        THREAD_STATUS_MAX               = 1 << 5
    }THREAD_STATUS;

    static const LocalDataKeyType   smInvalidLocalDataKey;
    static const ThreadType         smInvalidThread;
	static Mutex              smSingletonMutex;
    static ThreadManager*           smInstance;

    ThreadManager(void);
    ~ThreadManager(void);

    void cleanup(bool waitForThreadTermination);

    void lockThread(int id);
    void unlockThread(int id);

    int getThreadStatus(int id) const;
    int testAndSetThreadStatus(int id, int condition, int value);

	void associateLocalDataWithThread(int id);
	void* getLocalDataRaw(void) const;

    void callFunctionForThread(int id);
    void callFinalizeFunctionForThread(int id);
    void callbackHandleStatus(int id, int status);

#ifdef WIN32
    // thread pool callback
    static unsigned int __stdcall threadCallback(void *param);
#elif defined(__LINUX__) || defined(__APPLE__)
    // thread pool callback
    static void *threadCallback(void *param);
#else
#error Platform not yet supported.
#endif

    // threadInfo structure keeps track of each thread in the pool
    typedef struct {
        int                 id;
        int                 status;
        ThreadFunction      threadFunc;
        ThreadFinalizeFunc  finalizeFunc;
        long                calls;
        ThreadType          thread;
        Mutex*              mutex;
        void*               localData;
    }threadInfo;

    bool mInitialized;

    // thread pool
    threadInfo*         mThreadPool;
    int                 mThreadPoolSize;
    void*               mPrimaryThreadLocalData;
    LocalDataKeyType    mLocalDataKey;
    Mutex               mInternalDataMutex;
};

}

#endif // HGTHREADMANAGER_H_
