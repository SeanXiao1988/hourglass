/**
 *  @file    HGParamParser.cpp
 *  @brief   Parameter parser implementation
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

#include "HGParamParser.h"

namespace HG
{
    
// Type defaults
static int32_t         PARAM_DEFAULT_INT           = 0;
static uint32_t        PARAM_DEFAULT_UINT          = 0;
static float           PARAM_DEFAULT_FLOAT         = 0.0f;
static double          PARAM_DEFAULT_DOUBLE        = 0.0;
static bool            PARAM_DEFAULT_BOOL          = false;
static char*           PARAM_DEFAULT_STRING        = NULL;
static uint32_t        PARAM_DEFAULT_HASH          = 0x00000000;
static uint32_t        PARAM_DEFAULT_HEX           = 0x00000000;
static ComponentTypeID PARAM_DEFAULT_COMP_TYPE_ID  = COMP_NONE;
    
ParamParser::ParamParser(const char* address)
{
    setBasePointer(address);
}
    
ParamParser::~ParamParser()
{
}
    
ParamParser::ParamParser(const ParamParser& other)
{
    mNode = other.mNode;
    mBasePointer = other.mBasePointer;
    mLastResult = other.mLastResult;
}

ParamParser& ParamParser::operator= (const ParamParser& rhs)
{
    if (&rhs == this)
        return *this;
    
    mNode = rhs.mNode;
    mBasePointer = rhs.mBasePointer;
    mLastResult = rhs.mLastResult;
    
    return *this;
}
    
void ParamParser::setBasePointer(const char* pointer)
{
    mBasePointer = (param_node_t *)pointer;
    mNode = mBasePointer;
}
    
param_node_t* ParamParser::moveToFirstChild()
{
    param_node_t *ret = NULL;
    
    BREAK_START;
    
        if (mBasePointer == NULL)
            break;
    
        if (mNode->first_child_offset == PARAM_NULL_OFFSET)
            break;
        
        ret = (param_node_t *)(((char *)mBasePointer) + mNode->first_child_offset);
        mNode = ret;
    
    BREAK_END;
    
    return ret;
}
    
param_node_t* ParamParser::moveToNextNode()
{
    param_node_t *ret = NULL;
    
    BREAK_START;
    
    if (mBasePointer == NULL)
        break;
    
    if (mNode->next_node_offset == PARAM_NULL_OFFSET)
        break;
    
    ret = (param_node_t *)(((char *)mBasePointer) + mNode->next_node_offset);
    mNode = ret;
    
    BREAK_END;
    
    return ret;
}
    
param_node_t* ParamParser::moveToParentNode()
{
    param_node_t *ret = NULL;
    
    BREAK_START;
    
    if (mBasePointer == NULL)
        break;
    
    if (mNode->parent_node_offset == PARAM_NULL_OFFSET)
        break;
    
    ret = (param_node_t *)(((char *)mBasePointer) + mNode->parent_node_offset);
    mNode = ret;
    
    BREAK_END;
    
    return ret;
}
    
uint32_t ParamParser::getName() const
{
    if (mNode == NULL)
        return 0;
    
    return mNode->name;
}

uint32_t ParamParser::getParameterType() const
{
    if (mNode == NULL)
        return 0;
    
    return mNode->type;
}
    
ComponentTypeID ParamParser::getComponentType()
{
    if (mNode->type != TypeHashComponent)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_COMP_TYPE_ID;
    }
    
    mLastResult = PARAM_OK;
    return OBJECTMANAGER.getComponentTypeByName(mNode->name);
}

int32_t ParamParser::getInt()
{
    if (mNode->type != TypeHashInt)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_INT;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const int32_t *>(&mNode->data));
}

uint32_t ParamParser::getUInt()
{
    if (mNode->type != TypeHashUInt)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_UINT;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const uint32_t *>(&mNode->data));
}

float ParamParser::getFloat()
{
    if (mNode->type != TypeHashFloat)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_FLOAT;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const float *>(&mNode->data));
}

double ParamParser::getDouble()
{
    if (mNode->type != TypeHashDouble)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_DOUBLE;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const double *>(&mNode->data));
}

bool ParamParser::getBool()
{
    if (mNode->type != TypeHashBool)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_BOOL;
    }
    
    mLastResult = PARAM_OK;
    uint32_t boolean = *(reinterpret_cast<const uint32_t *>(&mNode->data));
    bool ret = (boolean != 0) ? true : false;
    return ret;
}

const char* ParamParser::getString()
{
    if (mNode->type != TypeHashString)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_STRING;
    }
    
    mLastResult = PARAM_OK;
    return reinterpret_cast<const char *>(&mNode->data);
}

uint32_t ParamParser::getHash()
{
    if (mNode->type != TypeHashHash)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_HASH;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const uint32_t *>(&mNode->data));
}
    
uint32_t ParamParser::getHex()
{
    if (mNode->type != TypeHashHex)
    {
        mLastResult = PARAM_INVALID_TYPE;
        return PARAM_DEFAULT_HEX;
    }
    
    mLastResult = PARAM_OK;
    return *(reinterpret_cast<const uint32_t *>(&mNode->data));
}
    
// get data from a child of this node. The parameter "name" is the name of the
// child node from which to read the data
param_node_t* ParamParser::moveToChild(uint32_t name)
{
    param_node_t *node = NULL;
    for (node = moveToFirstChild(); node != NULL; node = moveToNextNode())
    {
        if (node->name == name)
            break;
    }
    
    return node;
}

ComponentTypeID ParamParser::getComponentType(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_COMP_TYPE_ID;
    }
    
    mLastResult = PARAM_OK;
    return child.getComponentType();
}

int32_t ParamParser::getInt(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_INT;
    }
    
    mLastResult = PARAM_OK;
    return child.getInt();
}

uint32_t ParamParser::getUInt(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_UINT;
    }
    
    mLastResult = PARAM_OK;
    return child.getUInt();
}

float ParamParser::getFloat(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_FLOAT;
    }
    
    mLastResult = PARAM_OK;
    return child.getFloat();
}

double ParamParser::getDouble(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_DOUBLE;
    }
    
    mLastResult = PARAM_OK;
    return child.getDouble();
}

bool ParamParser::getBool(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_BOOL;
    }
    
    mLastResult = PARAM_OK;
    return child.getBool();
}

const char* ParamParser::getString(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_STRING;
    }
    
    mLastResult = PARAM_OK;
    return child.getString();
}

uint32_t ParamParser::getHash(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_HASH;
    }
    
    mLastResult = PARAM_OK;
    return child.getHash();
}
    
uint32_t ParamParser::getHex(uint32_t name)
{
    ParamParser child(*this);
    param_node_t *node = child.moveToChild(name);
    
    if (node == NULL)
    {
        mLastResult = PARAM_FIELD_NOT_FOUND;
        return PARAM_DEFAULT_HEX;
    }
    
    mLastResult = PARAM_OK;
    return child.getHex();
}
    
}
