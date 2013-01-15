/**
 *  @file    HGConsoleImpl.cpp
 *  @brief   Console implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/24
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

#include "HGConsoleImpl.h"
#include "HGRender.h"
namespace HG
{
    
ConsoleImpl::ConsoleImpl()
{
    mIsActive = false;
    mCommandBufferSize  = CONSOLE_COMMAND_BUFFER_SIZE;
    mTextBufferSize     = CONSOLE_TEXT_BUFFER_SIZE;
    mIsEchoOn           = true;
    mLineIndex          = 0;
    mCursorFlash        = 0.0f;
    mCursorPos          = 0;
}
    
ConsoleImpl::~ConsoleImpl()
{
}

void ConsoleImpl::initialize()
{
    setToggleKey(CONSOLE_TOGGLE_KEY);
    mAlpha          = 255;
    mColorEdit      = CONSOLE_COLOR_EDIT;
    mColorOK        = CONSOLE_COLOR_OK;
    mColorError     = CONSOLE_COLOR_ERROR;
    mColorEcho      = CONSOLE_COLOR_ECHO;
    mColorSystem    = CONSOLE_COLOR_SYSTEM;
    
    mQuad.tex       = 0;
    mQuad.blend     = BLEND_DEFAULT;
    mQuad.v[0].x    = 0.0f;
    mQuad.v[0].y    = 0.0f;
    RENDER.setVertexColor(&mQuad.v[0], CONSOLE_COLOR_UPPER);
    mQuad.v[1].x    = (float)RENDER.getWidth();
    mQuad.v[1].y    = 0.0f;
    RENDER.setVertexColor(&mQuad.v[1], CONSOLE_COLOR_UPPER);
    mQuad.v[2].x    = (float)RENDER.getWidth();
    mQuad.v[2].y    = RENDER.getHeight()/2.0f;
    RENDER.setVertexColor(&mQuad.v[2], CONSOLE_COLOR_LOWER);
    mQuad.v[3].x    = 0.0f;
    mQuad.v[3].y    = RENDER.getHeight()/2.0f;
    RENDER.setVertexColor(&mQuad.v[3], CONSOLE_COLOR_LOWER);
    
    print("Hello Developer, Welcome to Hourglass Console.", CMSG_SYSTEM_INFO);
}
    
void ConsoleImpl::deInitialze()
{
    
}

void ConsoleImpl::render(float dt)
{
    BREAK_START;
    
    if (!mIsActive)
        break;
    
    RENDER.renderQuad(&mQuad);
    
    std::list<ConsoleMsg>::iterator iter;
    
    int i = 1;
    int fontHeight = HG_DEFAULT_FONT_SIZE + 2;
    float height = RENDER.getHeight() / 2.0f;
    uint32_t textColor = 0xFFFFFFFF;
    for (iter = mTextBuffer.begin(); iter != mTextBuffer.end(); ++iter)
    {
        std::string str = iter->first;
        switch (iter->second)
        {
            case CMSG_ECHO:
                textColor = mColorEcho;
                break;
            case CMSG_FUNCTION_OK:
                textColor = mColorOK;
                break;
            case CMSG_ERROR:
                str = "<ERROR> "+str;
                textColor = mColorError;
                break;
            case CMSG_SYSTEM_INFO:
                textColor = mColorSystem;
                break;
            default:
                textColor = mColorEdit;
                break;
        }
        
        RENDER.renderText(CONSOLE_TEXT_OFFSET_X, height-fontHeight*2-fontHeight*(mTextBuffer.size() - i), textColor, str.c_str());
        i++;
    }
    
    mCursorLine.clear();
    for (uint32_t i = 0; i <= mCursorPos; i++)
        mCursorLine.push_back(' ');
    
    mCursorFlash += dt;
    if (mCursorFlash < CONSOLE_CURSOR_BLINK_TIME)
        RENDER.renderText(CONSOLE_TEXT_OFFSET_X, height-fontHeight-1, mColorEdit, "%s_", mCursorLine.c_str());
    else if (mCursorFlash > CONSOLE_CURSOR_BLINK_TIME * 2)
        mCursorFlash = 0.0f;
    
    RENDER.renderText(CONSOLE_TEXT_OFFSET_X, height-fontHeight-1, mColorEdit, ">%s", mCommandLine.c_str());
    
    BREAK_END;
}
    
bool ConsoleImpl::handleInput(int key, int action)
{
    bool interception = false;
    
    BREAK_START;
    
    mCursorFlash = 0.0f;
    if (key == mToggleKey)
    {
        toggleActive();
        break;
    }
    
    if (!mIsActive)
        break;
    
    interception = true;

    if (' ' <= key && key <= '~')
    {
        passKey((char)key);
    }
    
    BREAK_END;
    
    return interception;
}
    
void ConsoleImpl::handleControl(int key, int action)
{
    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_BACKSPACE:
                passBackspace();
                break;
            case GLFW_KEY_ENTER:
                passEnter();
                break;
            case GLFW_KEY_UP:
                historyTrack(-1);
                break;
            case GLFW_KEY_DOWN:
                historyTrack(1);
                break;
            case GLFW_KEY_LEFT:
                passCursorLocation(-1);
                break;
            case GLFW_KEY_RIGHT:
                passCursorLocation(1);
                break;
            case GLFW_KEY_DEL:
                passDelete();
                break;
            default:
                break;
        }
    }
}

void ConsoleImpl::setToggleKey(char key)
{
    if (key < ' ' || key >'~')
        return;
    
    mToggleKey = key;
}
    
void ConsoleImpl::toggleActive()
{
    mIsActive = !mIsActive;
}
    
void ConsoleImpl::close()
{
    mIsActive = false;
}
    
}