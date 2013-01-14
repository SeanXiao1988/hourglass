/**
 *  @file    HGApplication.h
 *  @brief   Application wrapper header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/09
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

#ifndef HGAPPLICATION_H_
#define HGAPPLICATION_H_

#include "HGSystem.h"

#define MAIN_SCRIPT_FILE "hg_main.lua"

namespace HG
{
    
typedef void (* mainloop_func)(void);

#define APPLICATION Application::getInstance()
    
class Application
{
public:
    static Application& getInstance()
    {
        static Application instance;
        return instance;
    }
    
    void            initialize();
    int             run();
    void            setMainLoopFunction(mainloop_func func);
    
    void            setAppTitle(const char* title);
private:
    mainloop_func   mMainLoopFunc;
    void            deInitialize();
    AutoMemoryTag   mMemLog;
    
private:
    Application() {}
    ~Application() {}
    Application(const Application& other);
    Application& operator= (const Application& rhs);
};

}

#endif // HGAPPLICATION_H_
