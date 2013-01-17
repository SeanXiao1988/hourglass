/**
 *  @file    HGInputManager.cpp
 *  @brief   Input manager implementation
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

#include "HGInputManager.h"
#include "HGObjectManager.h"
#include "HGConsoleImpl.h"

namespace HG
{
    
InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::initialize()
{
    glfwSetCharCallback(&InputManager::charkeysCallback);
    glfwEnable(GLFW_KEY_REPEAT);
    
    glfwSetKeyCallback(&InputManager::keyboardCallback);
    glfwSetMousePosCallback(&InputManager::mousePosCallback);
    glfwSetMouseButtonCallback(&InputManager::mouseBtnCallback);
    
    memset(mKeyMap, GLFW_PRESS, sizeof(int) * GLFW_KEY_LAST);
}

void InputManager::deInitialize()
{
    glDisable(GLFW_KEY_REPEAT);
    glfwSetCharCallback(NULL);
    glfwSetKeyCallback(NULL);
    glfwSetMousePosCallback(NULL);
    glfwSetMouseButtonCallback(NULL);
}

void InputManager::charkeysCallback(int key, int action)
{
    CONSOLE.handleInput(key, action);
}
    
void InputManager::keyboardCallback(int key, int action)
{
    InputInfo keyInfo;
    keyInfo.key = key;
    
    if (CONSOLE.isActive())
    {
        CONSOLE.handleControl(key, action);
        memset(getInstance().mKeyMap, 0, GLFW_KEY_LAST);
        return;
    }
    
	if (action == GLFW_RELEASE)
	{
		if (getInstance().mKeyMap[key] == GLFW_PRESS)
			// triggered
            keyInfo.action = INPUT_TRIGGER;
		else
            keyInfo.action = INPUT_RELEASE;
	}
    else
    {
        keyInfo.action = INPUT_PRESS;
    }
	
    getInstance().mKeyMap[key] = action;

    //
    EventKeyboard keyEvent(key, action);
    OBJECTMANAGER.broadcastEvent(keyEvent);
}
    
void InputManager::mousePosCallback(int x, int y)
{
    InputInfo posInfo;
    posInfo.x = x;
    posInfo.y = y;
    posInfo.action = INPUT_MOUSE_MOVE;
    
//    Event mouseEvent(EVENT_INPUT, &posInfo);
//    EVENTMANAGER.broadcastEvent(mouseEvent);
}
    
void InputManager::mouseBtnCallback(int btn, int action)
{
    InputInfo btnInfo;
    
    switch (btn)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            btnInfo.botton = INPUT_MOUSE_LEFT;
            break;
            
        case GLFW_MOUSE_BUTTON_RIGHT:
            btnInfo.botton = INPUT_MOUSE_RIGHT;
            break;
            
        default:
            return;
    }
    
    btnInfo.action = (action == GLFW_PRESS) ? INPUT_PRESS : INPUT_RELEASE;
    
//    Event clickEvent(EVENT_INPUT, &btnInfo);
//    EVENTMANAGER.broadcastEvent(clickEvent);
    
}
    
}