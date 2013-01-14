/**
 *  @file    HGParameterCompiler.h
 *  @brief   Parameter Compiler
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/12
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

#ifndef HGPARAMETERCOMPILER_H_
#define HGPARAMETERCOMPILER_H_

#include "HGSystem.h"
#include "HGError.h"
#include "rapidxml.hpp"

namespace HG
{
    
#define BASIC_NODE_SIZE sizeof(basic_node_t)
#define PARAMETERCOMPILER ParameterCompiler::getInstance()
    
typedef struct _basic_node_t_
{
    uint32_t first_child_offset;
    uint32_t next_node_offset;
    uint32_t parent_node_offset;
    uint32_t type;
    uint32_t name;
}basic_node_t;

typedef struct _parameter_node_t_
{
    basic_node_t basic_info;
    
    // auxiliary field
    char*    data;
    uint32_t data_size;
    struct _parameter_node_t_ *next;
}parameter_node_t;
    
typedef struct _component_node_t_
{
    basic_node_t basic_info;
    
    // auxiliary field
    struct _component_node_t_ *next;
    parameter_node_t *parameter_list;
}component_node_t;

typedef struct _object_node_t_
{
    basic_node_t basic_info;
    
    // auxiliary field
    struct _object_node_t_ *next;
    component_node_t *component_list;
}object_node_t;

class Data;
    
class ParameterCompiler
{
public:
    static ParameterCompiler& getInstance()
    {
        static ParameterCompiler instance;
        return instance;
    }

	void			initialize();
	void			deInitialize();
    
    HG_ERROR		dumpXML(const char* srcFile, const char* dstFile);
    Data*           parseXML(const char* srcFile, HG_ERROR *error = NULL);
    
    void            addObject(const char* name);
    void            addComponent(const char* name);
    void            addParamInt(const char* name, const int32_t data);
    void            addParamUInt(const char* name, const uint32_t data);
    void            addParamFloat(const char* name, const float data);
    void            addParamDouble(const char* name, const double data);
    void            addParamBool(const char* name, const bool data);
    void            addParamString(const char* name, const char* data);
    void            addParamHex(const char* name, const uint32_t data);
    void            addParamHash(const char* name, const uint32_t data);
    Data*           compileMemory();
    
    void            clear();

private:
    void            _addObject(uint32_t name);
    void            _addComponent(uint32_t name);
    void            _addParameterNode(uint32_t type, uint32_t name);
    void            _appendDataForParameterNode(char* data, uint32_t dataSize);
    
    void            resetPointers();
    
    void            _parseXML(rapidxml::xml_node<> *pParent);
    
    void            _parseParamTypes(rapidxml::xml_node<> *pNode);
    void            _parseParameters(rapidxml::xml_node<> *pNode);
    
    Data*           _writeMemory();
    
    uint32_t        mVirtualFilePointer;
    uint32_t        mCurObjOffset;
    uint32_t        mCurCompOffset;
    
    object_node_t       *mpObjectList;
    object_node_t       *mpCurObject;
    component_node_t    *mpCurComponent;
    parameter_node_t    *mpCurParameterNode;
    
    // parameters
    enum VAR_TYPE
    {
        INVALID_TYPE = 0,
        STRING,
        BOOLEAN,
        INTEGER,
        UINTEGER,
        FLOAT,
        DOUBLE,
        HEX,
        HASH
    };
    
    VAR_TYPE        mLastNodeType;
    
private:
    ParameterCompiler();
    ~ParameterCompiler();
    ParameterCompiler(const ParameterCompiler& other);
    ParameterCompiler& operator= (const ParameterCompiler& rhs);
};

}

#endif // HGPARAMETERCOMPILER_H_
