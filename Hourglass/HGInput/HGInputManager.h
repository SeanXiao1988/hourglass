/**
 *  @file    HGInputManager.h
 *  @brief   Input manager header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/07
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

#ifndef HGINPUTMANAGER_H_
#define HGINPUTMANAGER_H_

#include "HGInputDef.h"

HGNAMESPACE_START

#define INPUTMANAGER InputManager::getInstance()
    
class InputManager
{
public:
    static InputManager& getInstance()
    {
        static InputManager instance;
        return instance;
    }
    
    void initialize();
    void deInitialize();
    
    static void charkeysCallback(int key, int action);
    static void keyboardCallback(int key, int action);
    static void mousePosCallback(int x, int y);
    static void mouseBtnCallback(int btn, int action);
    
private:
    int    mKeyMap[GLFW_KEY_LAST];
    
private:
    InputManager();
    ~InputManager();
    InputManager(const InputManager& other);
    InputManager& operator= (const InputManager& rhs);
};

HGNAMESPACE_END

#endif // HGINPUTMANAGER_H_
