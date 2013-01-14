/**
 *  @file    HGConsole.h
 *  @brief   Console header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/13
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


#ifndef HGCONSOLE_H_
#define HGCONSOLE_H_

#include "HGSystem.h"

namespace HG
{

const uint8_t CONSOLE_CHAR_SPACE = 32;
const int CON_MAX_TEXT_LENGTH = 256;

typedef enum _console_item_type
{
    CTYPE_UCHAR = 0,
    CTYPE_CHAR,
    CTYPE_UINT,
    CTYPE_INT,
    CTYPE_FLOAT,
    CTYPE_STRING,
    CTYPE_BOOL,
    CTYPE_FUNC,

    CTYPE_COUNT
}ConsoleItemType;

// message type and function results
typedef enum _console_msg_type
{
    CMSG_FUNCTION_OK = 0,
    CMSG_SYSTEM_INFO,
    CMSG_ERROR,
    CMSG_ECHO,

    CMSG_COUNT
}ConsoleMsgType;

// [return type] [virtual func pointer] [parameter is a reference to std::vector<std::string>]
typedef int (*ConsoleFunc)(const std::vector<std::string> &);

// console item
typedef struct _console_item
{
    // item name
    std::string name;
    // item type
    ConsoleItemType type;
    union
    {
        ConsoleFunc func;
        void* variablePointer;
    };
}console_item_t;

typedef std::list<console_item_t> ItemList;

// output text
typedef std::pair<std::string, ConsoleMsgType> ConsoleMsg;

class Console
{
public:
    Console();
    virtual ~Console();

    void        addItem(const std::string& name, void* pointer, ConsoleItemType type);
    void        deleteItem(const std::string& name);
    void        print(const std::string& text, const ConsoleMsgType type = CMSG_FUNCTION_OK);
    void        print(const ConsoleMsgType type, const char* fmt, ...);
    void        historyTrack(int direction);

    void        passKey(char key);
    void        passBackspace();
    void        passDelete();
    void        passCursorLocation(int direction);
    void        passEnter();
    // void     render();
    
    void                setCommandBufferSize(int size);
    const uint32_t&     getCommandBufferSize() const { return mCommandBufferSize; }
    void                clearCommandBuffer() { mCommandBuffer.clear(); }
    
    void                setOutputBufferSize(int size);
    const uint32_t&     getOutputBufferSize() const { return mTextBufferSize; }
    void                clearOutputBuffer() { mTextBuffer.clear(); }

protected:
    bool        _parseCommandLine();
    void        _defaultFunc(std::vector<std::string> args);
    bool        _isParameterValid(const std::string& p);

    std::list<ConsoleMsg>       mTextBuffer;
    std::vector<std::string>    mCommandBuffer;
    // holds the command line
    std::string                 mCommandLine;
    std::string                 mCursorLine;

    uint32_t                    mCursorPos;
    bool                        mIsEchoOn;
    uint32_t                    mLineIndex;
    uint32_t                    mCommandBufferSize;
    uint32_t                    mTextBufferSize;

    ItemList                    mItemList;
};

}

#endif // HGCONSOLE_H_
