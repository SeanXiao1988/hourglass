/**
 *  @file    HGApplication.cpp
 *  @brief   Application wrapper implementation
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

#include "HGApplication.h"
#include "HG.h"

HGNAMESPACE_START
    
void Application::initialize()
{
    RENDER.initialize(800, 600);
    OBJECTMANAGER.initialize();
    SPRITEANIMATIONCOMPILER.initialize();
    SCENEMANAGER.initialize();
    INPUTMANAGER.initialize();
    CONSOLE.initialize();
    //AUDIOMANAGER.initialize();
    
    SCRIPTMANAGER.initialize();
    SCRIPTMANAGER.execScript(MAIN_SCRIPT_FILE);
    SCRIPTMANAGER.engineInvocation(HG_SCRIPT_ENTRY);
    
    mMainLoopFunc = NULL;
}
    
void Application::deInitialize()
{
    SCRIPTMANAGER.engineInvocation(HG_SCRIPT_DEINIT);
    
    //AUDIOMANAGER.deInitialize();
    CONSOLE.deInitialze();
    INPUTMANAGER.deInitialize();
    SCENEMANAGER.deInitialize();
    SPRITEANIMATIONCOMPILER.deInitialize();
    OBJECTMANAGER.deInitialize();
    RENDER.deInitialize();
    
    SCRIPTMANAGER.deInitialize();
}
 
void Application::setMainLoopFunction(mainloop_func func)
{
    mMainLoopFunc = func;
}
    
int Application::run()
{
    if (mMainLoopFunc != NULL)
        mMainLoopFunc();
    
    deInitialize();
    
    return 0;
}
    
void Application::setAppTitle(const char *title)
{
    glfwSetWindowTitle(title);
}
    
HGNAMESPACE_END