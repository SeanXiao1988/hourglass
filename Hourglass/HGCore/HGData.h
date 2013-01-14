/**
 *  @file    HGData.h
 *  @brief   Data class header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/09/04
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

#ifndef HGDATA_H_
#define HGDATA_H_

#include "HGSystem.h"

namespace HG
{

const uint32_t MAX_DATA_CAPCITY = 1024*1024;

class Data
{
public:
    Data(const Data& other);
    Data& operator= (const Data& rhs);
    ~Data();

    static Data* CreateFromFileContent(const char* filename);
    static Data* CreateFromBytes(const char* bytes, uint32_t length);
    static Data* CreateEmpty();

    const char*     appendData(const char* data, uint32_t length);
    const char*     getData() const     { return mBuffer; };
    const uint32_t  getLength() const   { return mLength; }
    void            purgeData();
    bool            writeToFile(const char* filename, bool overwrite = false);
    
    static bool IsFileExist(const char* filename);

private:
    Data();
    
    char*  mBuffer;
    uint32_t        mLength;
};

}

#endif // HGDATA_H_
