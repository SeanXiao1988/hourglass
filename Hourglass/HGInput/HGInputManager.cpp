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

HGNAMESPACE_START
    
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
    INPUT_TYPE type = INPUT_NONE;
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
            type = INPUT_TRIGGER;
		else
            type = INPUT_RELEASE;
	}
    else
    {
        type = INPUT_PRESS;
    }
	
    getInstance().mKeyMap[key] = action;

    //
    EventKeyboard keyEvent(key, (int)type);
    OBJECTMANAGER.broadcastEvent(keyEvent);
}
    
void InputManager::mousePosCallback(int x, int y)
{
    EventMouse mouseEvent(x, y, INPUT_MOUSE_MOVE);
    OBJECTMANAGER.broadcastEvent(mouseEvent);
}
    
void InputManager::mouseBtnCallback(int btn, int action)
{
    INPUT_TYPE type = INPUT_NONE;
    int x = 0;
    int y = 0;
    
    switch (btn)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS)
                type = INPUT_MOUSE_LEFT_PRESS;
            else
                type = INPUT_MOUSE_LEFT_RELEASE;
            break;
            
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS)
                type = INPUT_MOUSE_RIGHT_PRESS;
            else
                type = INPUT_MOUSE_RIGHT_RELEASE;
            break;
            
        default:
            return;
    }
    
    glfwGetMousePos(&x, &y);
    
    EventMouse event(x, y, (int)type);
    OBJECTMANAGER.broadcastEvent(event);
}
    
HGNAMESPACE_END