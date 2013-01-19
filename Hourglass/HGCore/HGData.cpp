/**
 *  @file    HGData.cpp
 *  @brief   Data class implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/09/05
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

#include "HGData.h"

#ifdef WIN32
#define FILE_OPEN_BEGIN std::locale::global(std::locale(""))
#define FILE_OPEN_END   std::locale::global(std::locale("C"))
#else
#define FILE_OPEN_BEGIN
#define FILE_OPEN_END
#endif

HGNAMESPACE_START

Data::Data()
    : mBuffer(NULL)
    , mLength(0)
{
}
    
Data::Data(const Data& other)
    : mBuffer(NULL)
    , mLength(0)
{
    mBuffer = new char[MAX_DATA_CAPCITY];
    mLength = other.mLength;
    memcpy(mBuffer, other.mBuffer, mLength);
}
    
Data& Data::operator=(const Data &rhs)
{
    if (&rhs == this)
        return *this;
    
    mLength = rhs.mLength;
    memcpy(mBuffer, rhs.mBuffer, mLength);

    return *this;
}

Data::~Data()
{
    if (mBuffer != NULL)
    {
        delete [] mBuffer;
        mBuffer = NULL;
    }

    mLength = 0;
}

Data* Data::CreateFromBytes(const char* bytes, uint32_t length)
{
    Data* retval = NULL;

    BREAK_START;
    
        if (bytes == NULL || length == 0)
            break;

        retval = new Data();

        retval->mBuffer = new char[MAX_DATA_CAPCITY];
        retval->mLength = length;
        memcpy(retval->mBuffer, bytes, length);
    
    BREAK_END;

    return retval;
}

bool Data::IsFileExist(const char* filename)
{
    bool ret = false;

    BREAK_START;
    
        if (filename == NULL)
            break;

        std::fstream file;
        FILE_OPEN_BEGIN;
        file.open(filename, std::ios::in | std::ios::binary);
        FILE_OPEN_END;

        if (file.fail())
            break;

        file.close();
        ret = true;
        
    BREAK_END;

    return ret;
}

Data* Data::CreateFromFileContent(const char* filename)
{
    Data * retval = NULL;

    BREAK_START;
    
        if (!IsFileExist(filename))
            break;

        std::ifstream file;
        FILE_OPEN_BEGIN;
        file.open(filename, std::ios::in | std::ios::binary);
        FILE_OPEN_END;

        if (file.fail())
            break;

        file.seekg(0, std::ios::end);
        std::streamoff filesize = file.tellg();

        uint32_t copy_length = (filesize >= MAX_DATA_CAPCITY) ? MAX_DATA_CAPCITY : (uint32_t)filesize;

        file.seekg(0, std::ios::beg);

        retval = new Data();
        retval->mBuffer = new char[MAX_DATA_CAPCITY];
		memset(retval->mBuffer, 0, MAX_DATA_CAPCITY);

        if (retval->mBuffer == NULL)
        {
            file.close();
            break;
        }

        file.read((char *)retval->mBuffer, copy_length);

        retval->mLength = copy_length;
        
    BREAK_END;
    
    return retval;
}

Data* Data::CreateEmpty()
{
    Data * retval = new Data();
    retval->mBuffer = new char[MAX_DATA_CAPCITY];
    return retval;
}

const char* Data::appendData(const char* data, uint32_t length)
{

    BREAK_START;
    
        if (data == NULL || length == 0)
            break;

        if ((mLength + length) >= MAX_DATA_CAPCITY)
            break;

        memcpy(mBuffer + mLength, data, length);
        mLength += length;

    BREAK_END;

    return data;
}

void Data::purgeData()
{
    mLength = 0;
}

bool Data::writeToFile(const char* filename, bool overwrite)
{
    bool retval = false;
    
    BREAK_START;
    
        // file exist and not overwrite it
        if ((IsFileExist(filename)) && (!overwrite))
            break;
        
        std::ofstream file;
        FILE_OPEN_BEGIN;
        file.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
        FILE_OPEN_END;
        
        if (file.fail())
            break;
        
        file.write((char *)mBuffer, mLength);
        file.close();
        
        retval = true;
        
    BREAK_END;
    
    return retval;
}

HGNAMESPACE_END