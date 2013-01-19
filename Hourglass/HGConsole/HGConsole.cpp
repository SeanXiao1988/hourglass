/**
 *  @file    HGConsole.cpp
 *  @brief   Console implementation
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


#include "HGConsole.h"

HGNAMESPACE_START

Console::Console()
    : mCursorPos(0)
    , mIsEchoOn(true)
    , mLineIndex(0)
    , mCommandBufferSize(0)
    , mTextBufferSize(0)
{
}

Console::~Console()
{
}

void Console::addItem(const std::string& name, void *pointer, ConsoleItemType type)
{
    console_item_t it;

    it.name = name;
    it.type = type;

    if (type != CTYPE_FUNC)
    {
        it.variablePointer = pointer;
    }
    else
    {
        it.func = (ConsoleFunc)pointer;
    }

    mItemList.push_back(it);
}

void Console::deleteItem(const std::string& name)
{
    ItemList::iterator iter;

    for (iter = mItemList.begin(); iter != mItemList.end(); ++iter)
    {
        if (iter->name == name)
        {
            mItemList.erase(iter);
            break;
        }
    }
}

void Console::print(const std::string& text, const ConsoleMsgType type)
{
    mTextBuffer.push_back(std::make_pair(text, type));
    
    if (mTextBuffer.size() > mTextBufferSize)
        mTextBuffer.pop_front();
}

void Console::print(const ConsoleMsgType type, const char* fmt, ...)
{
    va_list ar;

    char buffer[CON_MAX_TEXT_LENGTH];
    char *pBuffer = buffer;

    memset(pBuffer, 0, CON_MAX_TEXT_LENGTH);

    va_start(ar, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ar);
    va_end(ar);

    std::string strText(buffer);

    print(strText, type);
}

void Console::setCommandBufferSize(int size)
{
    mCommandBufferSize = size;

    if (mCommandBuffer.size() > mCommandBufferSize)
    {
        std::vector<std::string>::iterator it;
        it = mCommandBuffer.end() - mCommandBufferSize;
        mCommandBuffer.erase(mCommandBuffer.begin(), it);
    }
}

void Console::setOutputBufferSize(int size)
{
    mTextBufferSize = size;

    if (mTextBuffer.size() > mTextBufferSize)
    {
        std::list<ConsoleMsg>::iterator textIter;

        textIter = mTextBuffer.end();

        int reverse = mTextBufferSize;
        std::advance(textIter, -reverse);
        mTextBuffer.erase(mTextBuffer.begin(), textIter);
    }
}

void Console::historyTrack(int direction)
{
    if (mLineIndex == 0 && direction < 0)
    {
        mLineIndex = 0;
    }
    else if ((mLineIndex + direction) >= mCommandBuffer.size())
    {
        mLineIndex = (uint32_t)mCommandBuffer.size() - 1;
    }
    else
    {
        mLineIndex += direction;
    }

    if (!mCommandBuffer.empty())
    {
        mCommandLine = mCommandBuffer[mLineIndex];
        mCursorPos = (uint32_t)mCommandLine.size();
    }
}

void Console::passKey(char key)
{
    if (mCursorPos > CON_MAX_TEXT_LENGTH)
        return;

    if (key >= CONSOLE_CHAR_SPACE)
    {
        mCommandLine.insert(mCommandLine.begin() + mCursorPos, key);
        mCursorPos++;
    }
}

void Console::passBackspace()
{
    if (mCommandLine.length() > 0 && mCursorPos > 0)
    {
        mCommandLine.erase(mCommandLine.begin() + mCursorPos - 1);
        mCursorPos--;
    }
}

void Console::passDelete()
{
    if (mCommandLine.length() > 0)
    {
        mCommandLine.erase(mCommandLine.begin() + mCursorPos);
    }
}

void Console::passCursorLocation(int direction)
{
    switch (direction)
    {
        case 1:
            {
                if (mCursorPos >= mCommandLine.size())
                    break;

                mCursorPos++;
                break;
            }
        case -1:
            {
                if (mCursorPos == 0)
                    break;

                mCursorPos--;
                break;
            }
        default:
            break;
    }
}

void Console::passEnter()
{
    if (mCommandLine.length() > 0)
    {
        _parseCommandLine();
        mCommandLine.clear();
        mCursorPos = 0;
        mLineIndex = (uint32_t)mCommandBuffer.size();
    }
}

bool Console::_parseCommandLine()
{
    std::ostringstream out;
    std::string::size_type index = 0;
    std::vector<std::string> arguments;
    std::list<console_item_t>::const_iterator iter;
    bool ret = false;

    // add to text buffer
    if (mIsEchoOn)
        print(mCommandLine, CMSG_ECHO);

    // add to commandline buffer
    mCommandBuffer.push_back(mCommandLine);

    if (mCommandBuffer.size() > mCommandBufferSize)
        mCommandBuffer.erase(mCommandBuffer.begin());

    // tokenize
    while (index != std::string::npos)
    {
        // push word
        std::string::size_type nextSpace = mCommandLine.find(' ', index);
        arguments.push_back(mCommandLine.substr(index, nextSpace));

        // increase index
        if (nextSpace != std::string::npos)
            index = nextSpace + 1;
        else
            break;
    }

    // execute
    for (iter = mItemList.begin(); iter != mItemList.end(); ++iter)
    {
        if (iter->name == arguments[0])
        {
            switch(iter->type)
            {
                case CTYPE_UCHAR:
                    {
                        if (arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_UCHAR>", CMSG_ERROR);
                            ret = false;
                        }
                        else if (arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_UCHAR> " << (*iter).name << " = " << *((unsigned char *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        else if (arguments.size() == 2)
                        {
                            if (!_isParameterValid(arguments[1]))
                            {
                                print("Invalid parameters", CMSG_ERROR);
                                ret = false;
                            }
                            else
                            {
                                *((unsigned char *)(*iter).variablePointer) = (unsigned char)atoi(arguments[1].c_str());
                                ret = true;
                            }
                        }
                        break;
                    }

                case CTYPE_CHAR:
                    {
                        if (arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_CHAR>", CMSG_ERROR);
                            ret = false;
                        }
                        else if (arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_CHAR> " << (*iter).name << " = " << *((char *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        else if (arguments.size() == 2)
                        {
                            if (!_isParameterValid(arguments[1]))
                            {
                                print("Invalid parameters", CMSG_ERROR);
                                ret = false;
                            }
                            else
                            {
                                *((char *)(*iter).variablePointer) = (char)atoi(arguments[1].c_str());
                                ret = true;
                            }
                        }
                        break;
                    }
                
                case CTYPE_UINT:
                    {
                        if (arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_UINT>", CMSG_ERROR);
                            ret = false;
                        }
                        else if (arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_UINT> " << (*iter).name << " = " << *((unsigned int *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        else if (arguments.size() == 2)
                        {
                            if (!_isParameterValid(arguments[1]))
                            {
                                print("Invalid parameters", CMSG_ERROR);
                                ret = false;
                            }
                            else
                            {
                                *((unsigned int *)(*iter).variablePointer) = (unsigned int)atoi(arguments[1].c_str());
                                ret = true;
                            }
                        }
                        break;
                    }
            
                case CTYPE_INT:
                    {
                        if (arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_INT>", CMSG_ERROR);
                            ret = false;
                        }
                        else if (arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_INT> " << (*iter).name << " = " << *((int *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        else if (arguments.size() == 2)
                        {
                            if (!_isParameterValid(arguments[1]))
                            {
                                print("Invalid parameters", CMSG_ERROR);
                                ret = false;
                            }
                            else
                            {
                                *((int *)(*iter).variablePointer) = (int)atoi(arguments[1].c_str());
                                ret = true;
                            }
                        }
                        break;
                    }
                    
                case CTYPE_BOOL:
                    {
                        if(arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_BOOL>", CMSG_ERROR);
                            ret = false;
                        }
                        else if(arguments.size() == 1)
                        {
                            out.str("");
                            out<< " <CTYPE_BOOL> "<<(*iter).name << "=" << *((bool *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        if(arguments.size() == 2)
                        {
                            if(_isParameterValid(arguments[1]))
                            {
                                int ibtemp;
                                ibtemp = (int) atoi(arguments[1].c_str());
                                if(ibtemp != 0)
                                    *((bool *)(*iter).variablePointer) = true;
                                else 
                                    *((bool *)(*iter).variablePointer) = false;
                                ret = true;
                            }
                            else
                            {
                                print("Invalid parameters", CMSG_ERROR);
                                ret = false;
                            }
                        }
                        break;
                    }
                    
                case CTYPE_FLOAT:
                    {
                        if(arguments.size() > 2)
                        {
                            print("Too many arguments for <CTYPE_FLOAT>", CMSG_ERROR);
                            ret = false;
                        }
                        else if(arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_FLOAT> "<< (*iter).name << " = " << *((float *)(*iter).variablePointer);
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        if(arguments.size() == 2)
                        {
                            *((float *)(*iter).variablePointer) = (float)atof(arguments[1].c_str());
                            ret = true;
                        }
                        break;
                    }
                    
                case CTYPE_STRING:
                    {
                        if(arguments.size() == 1)
                        {
                            out.str("");
                            out << " <CTYPE_STRING> "<< (*iter).name << " = " << *(std::string *)(*iter).variablePointer;
                            print(out.str(), CMSG_FUNCTION_OK);
                            ret = true;
                        }
                        else if(arguments.size() > 1)
                        {
                            // reset variable
                            *((std::string *)(*iter).variablePointer) = "";
                            
                            // add new string
                            for(unsigned int i = 1; i < arguments.size(); ++i)
                            {
                                *((std::string *)(*iter).variablePointer) += arguments[i];
                            }
                            ret = true;
                        }
                        break;
                    }
                    
                case CTYPE_FUNC:
                    {
                        if( ((*iter).func(arguments)) == 0)
                        {
                            print("Command Execute Failed, check your input please.", CMSG_ERROR);
                            ret = false;
                        }
                        else
                        {
                            ret = true;
                        }
                        break;
                    }
                default:
                    _defaultFunc(arguments);
                    ret = false;
                    
                    break;
            }
        }
    }
    
    return ret;
}

void Console::_defaultFunc(std::vector<std::string> args)
{
    print("< " + args[0] + " > invalid command.", CMSG_ERROR);
}

bool Console::_isParameterValid(const std::string &s)
{
    if (s.empty())
        return false;

    for (unsigned int i = 0; i < s.size(); ++i)
    {
        if (!std::isdigit(s[i]))
            return false;
    }

    return true;
}

HGNAMESPACE_END
