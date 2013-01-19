/**
 *  @file    HGScriptManager.h
 *  @brief   Script manager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/25
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

#ifndef HGSCRIPTMANAGER_H_
#define HGSCRIPTMANAGER_H_

#include "HGSystem.h"
#include "HGScriptDef.h"

#define HG_SCRIPT_ENTRY     "hg_init"
#define HG_SCRIPT_DEINIT    "hg_deinit"

HGNAMESPACE_START

#define SCRIPTMANAGER ScriptManager::getInstance()

class ScriptManager
{
public:
    static ScriptManager& getInstance()
    {
        static ScriptManager instance;
        return instance;
    }

    void initialize();
    void deInitialize();

    void execScript(const char* filename);
    void engineInvocation(const char* func);
    
    lua_State* getState() const { return mState; }
    
    static int scriptAtPanic(lua_State* L);

private:
    void _registerEngine();
    
    lua_State*  mState;

private:
    ScriptManager();
    ~ScriptManager();
    ScriptManager(const ScriptManager& other);
    ScriptManager& operator= (const ScriptManager& rhs);
};

HGNAMESPACE_END

#endif // HGSCRIPTMANAGER_H_
