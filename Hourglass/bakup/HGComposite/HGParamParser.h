/**
 *  @file    HGParamParser.h
 *  @brief   Parameter parser header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/20
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

#ifndef HGPARAMPARSER_H_
#define HGPARAMPARSER_H_

#include "HGObjectManager.h"

namespace HG
{
    
#define PARAM_NULL_OFFSET -1
    
const uint32_t TypeHashObject       = 0x9970C5B4;
const uint32_t TypeHashComponent    = 0xE98135FE;
const uint32_t TypeHashInt          = 0x3F38EF10;
const uint32_t TypeHashUInt         = 0x97A68D95;
const uint32_t TypeHashFloat        = 0xF8968649;
const uint32_t TypeHashDouble       = 0x79186806;
const uint32_t TypeHashBool         = 0xEBD8DB59;
const uint32_t TypeHashString       = 0x9A6C7A4E;
const uint32_t TypeHashHex          = 0x563B5248;
const uint32_t TypeHashHash         = 0x1A6B9FD5;

enum ParameterType
{
    PARAM_TYPE_OBJECT = 0,
    PARAM_TYPE_COMPONENT,
    PARAM_TYPE_INT,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_DOUBLE,
    PARAM_TYPE_BOOL,
    PARAM_TYPE_STRING,
    PARAM_TYPE_HASH,
    PARAM_TYPE_HEX,
    
    PARAM_TYPE_COUNT
};

enum ParameterResult
{
    PARAM_OK = 0,
    PARAM_INVALID_TYPE,
    PARAM_FIELD_NOT_FOUND
};
 
//#pragma pack(push)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 byte boundary */
    
typedef struct _param_node_t_
{
    uint32_t first_child_offset;
    uint32_t next_node_offset;
    uint32_t parent_node_offset;
    uint32_t type;
    uint32_t name;
    char     data[1];
}param_node_t;
    
//#pragma pack(pop)
    
class ParamParser
{
public:
    explicit ParamParser(const char* address);
    ParamParser(const ParamParser& other);
    ParamParser& operator= (const ParamParser& rhs);
    ~ParamParser();
    
    void            setBasePointer(const char* pointer);
    
    param_node_t*   getCurrentNode() { return mNode; }
    param_node_t*   moveToFirstChild();
    param_node_t*   moveToNextNode();
    param_node_t*   moveToParentNode();
    
    param_node_t*   moveToChild(uint32_t name);
    
    // accessor
    uint32_t        getName() const;
    uint32_t        getParameterType() const;
    ComponentTypeID getComponentType();
    int32_t         getInt();
    uint32_t        getUInt();
    float           getFloat();
    double          getDouble();
    bool            getBool();
    const char*     getString();
    uint32_t        getHash();
    uint32_t        getHex();
    
    // get data from a child of this node. The parameter "name" is the name of the
    // child node from which to read the data
    ComponentTypeID getComponentType(uint32_t name);
    int32_t         getInt(uint32_t name);
    uint32_t        getUInt(uint32_t name);
    float           getFloat(uint32_t name);
    double          getDouble(uint32_t name);
    bool            getBool(uint32_t name);
    const char*     getString(uint32_t name);
    uint32_t        getHash(uint32_t name);
    uint32_t        getHex(uint32_t name);
    
private:
    param_node_t*   mNode;
    param_node_t*   mBasePointer;
    ParameterResult mLastResult;
};
    
}

#endif // HGPARAMPARSER_H_
