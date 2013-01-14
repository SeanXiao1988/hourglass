/**
 *  @file    HGThreadManager.cpp
 *  @brief   Thread Manager implementation
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

#include "HGSystem.h"
#include "HGThreadManager.h"
#include "HGAssert.h"
#include "HGMemory.h"

using namespace HG;

const int THREAD_SLEEP_DURATION = 50;   // milliseconds of sleep between status checks

#ifdef WIN32
#include <process.h>

const LocalDataKeyType ThreadManager::smInvalidLocalDataKey = TLS_OUT_OF_INDEXES;
const ThreadType ThreadManager::smInvalidThread = NULL;
#elif defined(__LINUX__) || (__APPLE__)
#include <pthread.h>
#include <unistd.h>

const LocalDataKeyType ThreadManager::smInvalidLocalDataKey = 0;
const ThreadType ThreadManager::smInvalidThread = 0;
#endif

Mutex ThreadManager::smSingletonMutex;
ThreadManager* ThreadManager::smInstance = NULL;

//------------------------------------------------------------------------------------------
ThreadManager& ThreadManager::getInstance(void)
{
    if (smInstance == NULL)
    {
        smSingletonMutex.enter();
        if(smInstance == NULL)
        {
            smInstance = new ThreadManager();
        }

        smSingletonMutex.leave();
    }

    return *smInstance;
}

//------------------------------------------------------------------------------------------
// Call yield() to cause the current thread to offer control to any waiting threads on the same processor.
// If no other threads are waiting, control will continue with the current thread.
void ThreadManager::yield(void)
{
#ifdef WIN32
    Sleep(0);
#elif defined (__LINUX__) || defined (__APPLE__)
    sched_yield();
#endif
}

//------------------------------------------------------------------------------------------
// Call sleep() to cause the current thread to block for a period of time. When blocked, other threads waiting on the same processor will be allowed to take control.
void ThreadManager::sleep(int milliseconds)
{
    assertion(milliseconds > 0, "invalid sleep time");

#ifdef WIN32
    Sleep(milliseconds);
#elif defined(__LINUX__) || defined(__APPLE__)
    usleep(milliseconds * 1000);
#endif
}

//------------------------------------------------------------------------------------------
bool ThreadManager::startup(int threadPoolSize, void* primaryThreadLocalData)
{
    bool ret = false;

    mInternalDataMutex.enter();

    assertion(!mInitialized, "ThreadMgr already initialized");
    assertion(threadPoolSize > 0, "Invalid parameter, pool size must larger than 0");

    bool successful = true;

    // allocate a new thread pool
    mThreadPool = new1<threadInfo>(threadPoolSize);
    if (mThreadPool == NULL)
    {
        successful = false;
    }
    else
    {
        mThreadPoolSize = threadPoolSize;

        for (int i = 0; i < threadPoolSize; i++)
        {
            // populate the threadInfo structure
            mThreadPool[i].id = i;
            mThreadPool[i].status = THREAD_STATUS_UNASSIGNED;
            mThreadPool[i].threadFunc = NULL;
            mThreadPool[i].calls = 0;
            mThreadPool[i].thread = smInvalidThread;
            mThreadPool[i].mutex = NULL;
            mThreadPool[i].localData = NULL;

            // initialize the critical section
            mThreadPool[i].mutex = new0 Mutex();
            if (mThreadPool[i].mutex == NULL)
            {
                mThreadPoolSize = i;
                break;
            }

#ifdef WIN32
            mThreadPool[i].thread = (ThreadType)_beginthreadex(NULL, 0, ThreadManager::threadCallback, &(mThreadPool[i].id), 0, NULL);

            if (mThreadPool[i].thread == smInvalidThread)
            {
                successful = false;
                mThreadPoolSize = i;
                break;
            }
#elif defined(__LINUX__) || defined (__APPLE__)
            if (pthread_create(&(mThreadPool[i].thread), NULL, ThreadManager::threadCallback, &(mThreadPool[i].id)) != 0)
            {
                successful = false;
                mThreadPoolSize = i;
                break;
            }
#endif // WIN32
        }
    }

    if (successful)
    {
        // create the local data key and seed it with our primaryThreadLocalData
#ifdef WIN32
        mLocalDataKey = TlsAlloc();
        if (mLocalDataKey == smInvalidLocalDataKey)
        {
            successful = false;
        }
        else
        {
			successful = TlsSetValue(mLocalDataKey, this)?true:false;
        }
#elif defined(__LINUX__) || defined(__APPLE__)
        if (pthread_key_create(&mLocalDataKey, NULL) != 0)
        {
            successful = false;
        }
        else
        {
            if (pthread_setspecific(mLocalDataKey, this) != 0)
            {
                successful = false;
            }
        }
#endif

        // store the primaryThreadLocalData for later
        mPrimaryThreadLocalData = primaryThreadLocalData;
    }

    if (!successful)
    {
        cleanup(false);
    }
    else
    {
        mInitialized = true;
        ret = true;
    }

    mInternalDataMutex.leave();
    return ret;
}

//------------------------------------------------------------------------------------------
void ThreadManager::shutdown()
{
    mInternalDataMutex.enter();

    if (mInitialized)
    {
        assertion(isPrimaryThread(), "Shutdown can only be call in primary thread");

        // call cleanup() and ensure the threads get terminated before returning
        cleanup(true);
        mInitialized = false;
    }

    mInternalDataMutex.leave();
}

//------------------------------------------------------------------------------------------
int ThreadManager::assignThreadToFunction(ThreadFunction func, ThreadFinalizeFunc finalizeFunc, void* localData)
{
    assertion(mInitialized, "ThreadMgr must be initialized before assigning any threads");
    assertion(isPrimaryThread(), "Thread assignation must be call in primary thread.");

    int id = INVALID_THREAD_ID;

    // enter major critical section
    mInternalDataMutex.enter();

    // find the next available thread id
    for (int i = 0; i < mThreadPoolSize; i++)
    {
        if (mThreadPool[i].status == THREAD_STATUS_UNASSIGNED)
        {
            id = i;
            break;
        }
    }

    if (id != INVALID_THREAD_ID)
    {
        mThreadPool[id].status = THREAD_STATUS_STOPPED;
        
        // populate the threadInfo record
        mThreadPool[id].threadFunc = func;
        mThreadPool[id].finalizeFunc = finalizeFunc;
        mThreadPool[id].calls = 0;
        mThreadPool[id].localData = localData;
    }

    mInternalDataMutex.leave();

    return id;
}

//------------------------------------------------------------------------------------------
bool ThreadManager::unassignThread(int id)
{
	assertion(isPrimaryThread(), "unassignation must be call in main thread.");

	int status = testAndSetThreadStatus(id, ~THREAD_STATUS_UNASSIGNED, THREAD_STATUS_UNASSIGN_REQUEST);

	return ((status == THREAD_STATUS_UNASSIGN_REQUEST) || (status == THREAD_STATUS_UNASSIGNED));
}

//------------------------------------------------------------------------------------------
bool ThreadManager::startThread(int id)
{
	assertion(isPrimaryThread(), "startThread must be call in main thread.");

	int status = testAndSetThreadStatus(id, THREAD_STATUS_STOPPED, THREAD_STATUS_RUNNING_REQUEST);

	return ( (status == THREAD_STATUS_RUNNING_REQUEST) || (status == THREAD_STATUS_RUNNING) );
}

//------------------------------------------------------------------------------------------
bool ThreadManager::stopThread(int id)
{
	assertion(isPrimaryThread(), "stopThread must be call in main thread.");

	int status = testAndSetThreadStatus(id, THREAD_STATUS_RUNNING | THREAD_STATUS_RUNNING_REQUEST, THREAD_STATUS_STOPPED);

	return (status == THREAD_STATUS_STOPPED);
}

//------------------------------------------------------------------------------------------
int ThreadManager::getThreadPoolSize() const
{
	return mThreadPoolSize;
}

//------------------------------------------------------------------------------------------
void* ThreadManager::getLocalData() const
{
	void* output = getLocalDataRaw();
	if (output == this)
	{
		output = mPrimaryThreadLocalData;
	}

	return output;
}

//------------------------------------------------------------------------------------------
bool ThreadManager::isPrimaryThread() const
{
	return (getLocalDataRaw() == this);
}

//------------------------------------------------------------------------------------------
int ThreadManager::getAndResetThreadCalls(int id)
{
	assertion(mInitialized, "ThreadMgr has not initialized yet.");
	assertion( (id >= 0) && (id < mThreadPoolSize), "invalid thread");

	int output = 0;

	lockThread(id);
	output = mThreadPool[id].calls;
	mThreadPool[id].calls = 0;
	unlockThread(id);

	return output;
}

//------------------------------------------------------------------------------------------
ThreadManager::ThreadManager()
{
	mInitialized = false;
	mThreadPool = NULL;
	mThreadPoolSize = 0;
	mPrimaryThreadLocalData = NULL;
	mLocalDataKey = smInvalidLocalDataKey;
}

//------------------------------------------------------------------------------------------
ThreadManager::~ThreadManager()
{
}

//------------------------------------------------------------------------------------------
void ThreadManager::cleanup(bool waitForThreadTermination)
{
	for (int i = 0; i < mThreadPoolSize; i++)
	{
		testAndSetThreadStatus(i, THREAD_STATUS_ALL, THREAD_STATUS_TERMINATE_REQUEST);
	}

#if defined(__LINUX__) || defined(__APPLE__)
		timespec joinDelay;
		joinDelay.tv_sec = 1;
#endif

	for (int i = 0; i < mThreadPoolSize; i++)
	{
#ifdef WIN32
		if (waitForThreadTermination)
		{
			WaitForSingleObject(mThreadPool[i].thread, INFINITE);
		}
		else
		{
			WaitForSingleObject(mThreadPool[i].thread, 1000);
		}
		CloseHandle((HANDLE)mThreadPool[i].thread);
#elif defined (__LINUX__)
		if (waitForThreadTermination)
		{
			pthread_join(mThreadPool[i].thread, NULL);
		}
		else
		{
			pthread_timedjoin_np(mThreadPool[i].thread, NULL, &joinDelay);
		}
#elif defined (__APPLE__)
        // no pthread_timedjoin_np for BSD
        pthread_join(mThreadPool[i].thread, NULL);
        
		pthread_detach(mThreadPool[i].thread);
#endif

		delete0(mThreadPool[i].mutex);
		mThreadPool[i].mutex = NULL;
	}

	delete1(mThreadPool);
	mThreadPool = NULL;
	mThreadPoolSize = 0;

	if (mLocalDataKey != smInvalidLocalDataKey)
	{
#ifdef WIN32
		TlsFree(mLocalDataKey);
#elif defined(__LINUX__) || defined(__APPLE__)
		pthread_key_delete(mLocalDataKey);
#endif
	}

	mLocalDataKey = smInvalidLocalDataKey;
}

//------------------------------------------------------------------------------------------
void ThreadManager::lockThread(int id)
{
	assertion((id >= 0) && (id < mThreadPoolSize), "invalid thread id.");
	assertion(mThreadPool[id].mutex != NULL, "null mutex");

	mThreadPool[id].mutex->enter();
}

//------------------------------------------------------------------------------------------
void ThreadManager::unlockThread(int id)
{
	assertion( (id >= 0) && (id < mThreadPoolSize), "invalid thread id.");
	assertion(mThreadPool[id].mutex != NULL, "null mutex");

	mThreadPool[id].mutex->leave();
}

//------------------------------------------------------------------------------------------
int ThreadManager::getThreadStatus(int id) const
{
	assertion( (id >= 0) && (id < mThreadPoolSize), "invalid id");

	return mThreadPool[id].status;
}

//------------------------------------------------------------------------------------------
int ThreadManager::testAndSetThreadStatus(int id, int condition, int value)
{
	assertion((id >= 0) && (id < mThreadPoolSize), "invalid thread id");

	lockThread(id);

	int output = mThreadPool[id].status;

	if ((output & condition) != 0)
	{
		mThreadPool[id].status = value;
		output = value;
	}

	unlockThread(id);

	return output;
}

//------------------------------------------------------------------------------------------
void ThreadManager::associateLocalDataWithThread(int id)
{
	assertion(mInitialized, "ThreadMgr not yet initialized");
	assertion((id >= 0) && (id < mThreadPoolSize), "invalid thread id.");

#ifdef WIN32
	TlsSetValue(mLocalDataKey, mThreadPool[id].localData);
#elif defined(__LINUX__) || defined(__APPLE__)
	pthread_setspecific(mLocalDataKey, mThreadPool[id].localData);
#endif
}

//------------------------------------------------------------------------------------------
void* ThreadManager::getLocalDataRaw() const
{
	assertion(mInitialized, "ThreadMgr not yet initialized.");

	void* output = NULL;

#ifdef WIN32
	output = TlsGetValue(mLocalDataKey);
#elif defined(__LINUX__) || defined(__APPLE__)
	output = pthread_getspecific(mLocalDataKey);
#endif

	return output;
}

//------------------------------------------------------------------------------------------
void ThreadManager::callFunctionForThread(int id)
{
	assertion(mInitialized, "ThreadMgr not yet initialized.");
	assertion((id >= 0) && (id < mThreadPoolSize), "invalid thread id");

	ThreadFunction func = mThreadPool[id].threadFunc;
	if (func != NULL)
	{
		(*func)();
#ifdef WIN32
		InterlockedIncrement(&mThreadPool[id].calls);
#else
		lockThread(id);
		mThreadPool[id].calls++;
		unlockThread(id);
#endif
	}
}

//------------------------------------------------------------------------------------------
void ThreadManager::callFinalizeFunctionForThread(int id)
{
	assertion(mInitialized, "ThreadMgr not yet initialized.");
	assertion((id >= 0) && (id < mThreadPoolSize), "invalid id");

	ThreadFinalizeFunc func = mThreadPool[id].finalizeFunc;
	if( (func != NULL) && (mThreadPool[id].calls > 0))
	{
		(*func)();
	}
}

//------------------------------------------------------------------------------------------
void ThreadManager::callbackHandleStatus(int id, int status)
{
	int statusResult = THREAD_STATUS_UNASSIGNED;

	switch (status)
	{
		case THREAD_STATUS_RUNNING_REQUEST:
			statusResult = testAndSetThreadStatus(id, THREAD_STATUS_RUNNING_REQUEST, THREAD_STATUS_RUNNING);
			if (statusResult != THREAD_STATUS_RUNNING)
			{
				sleep(THREAD_SLEEP_DURATION);
				break;
			}

			associateLocalDataWithThread(id);

		case THREAD_STATUS_RUNNING:
			callFunctionForThread(id);
			yield();
			break;

		case THREAD_STATUS_UNASSIGN_REQUEST:
			callFinalizeFunctionForThread(id);
			testAndSetThreadStatus(id, THREAD_STATUS_UNASSIGN_REQUEST, THREAD_STATUS_UNASSIGNED);

		default:
			sleep(THREAD_SLEEP_DURATION);
			break;
	}
}

#ifdef WIN32
unsigned int __stdcall ThreadManager::threadCallback(void* param)
#elif defined(__LINUX__) || defined(__APPLE__)
void* ThreadManager::threadCallback(void* param)
#endif
{
	assertion(param != NULL, "null param");

	int id = *(int *)param;

	ThreadManager &mgr = ThreadManager::getInstance();

	int status = mgr.getThreadStatus(id);
	while (status != THREAD_STATUS_TERMINATE_REQUEST)
	{
		mgr.callbackHandleStatus(id, status);

		status = mgr.getThreadStatus(id);
	}

#ifdef WIN32
	_endthreadex(0);
#endif

	return 0;
}