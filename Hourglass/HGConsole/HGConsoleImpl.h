/**
 *  @file    HGConsoleImpl.h
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

#ifndef HGCONSOLEIMPL_H_
#define HGCONSOLEIMPL_H_

#include "HGConsole.h"
#include "HGGraphicsDef.h"

namespace HG
{
    
#define CONSOLE ConsoleImpl::getInstance()

#define CONSOLE_TEXT_OFFSET_X       5
#define CONSOLE_CURSOR_BLINK_TIME   0.5f
    
#define CONSOLE_COMMAND_BUFFER_SIZE 20
#define CONSOLE_TEXT_BUFFER_SIZE    50
#define CONSOLE_TOGGLE_KEY          '`'
    
#define CONSOLE_COLOR_EDIT          0x000000FF
#define CONSOLE_COLOR_OK            0x008000FF
#define CONSOLE_COLOR_ERROR         0xFF0000FF
#define CONSOLE_COLOR_ECHO          0x404040FF
#define CONSOLE_COLOR_SYSTEM        0x000080FF
#define CONSOLE_COLOR_UPPER         0xFFFFFF80
#define CONSOLE_COLOR_LOWER         0xFFFFFFFF
    
class ConsoleImpl : public Console
{
public:
    static ConsoleImpl& getInstance()
    {
        static ConsoleImpl instance;
        return instance;
    }
    
    ~ConsoleImpl();

    void        initialize();
    void        deInitialze();
    
    void        setToggleKey(char key);
    void        toggleActive();
    
    void        render(float dt);
    
    bool        handleInput(int key, int action);
    void        handleControl(int key, int action);
    void        close();
    
    bool        isActive() { return mIsActive; }
    

private:
    int         mAlpha;
    bool        mIsActive;
    char        mToggleKey;
    float       mCursorFlash;
    uint32_t    mColorEdit;
    uint32_t    mColorOK;
    uint32_t    mColorError;
    uint32_t    mColorEcho;
    uint32_t    mColorSystem;
    Quad        mQuad;

private:
    ConsoleImpl();
    ConsoleImpl(const ConsoleImpl& other);
    ConsoleImpl& operator= (const ConsoleImpl& rhs);
};

}

#endif // HGCONSOLEIMPL_H_
