/**
 *  @file    HGParameterCompiler.cpp
 *  @brief   Parameter compiler implementation
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

#include "HGParameterCompiler.h"
#include "HGParamParser.h"
#include "HGHash.h"
#include "HGData.h"

using namespace rapidxml;

namespace HG
{
    
ParameterCompiler::ParameterCompiler()
{
}

ParameterCompiler::~ParameterCompiler()
{
}

void ParameterCompiler::initialize()
{
	resetPointers();
	clear();
}

void ParameterCompiler::deInitialize()
{
	clear();
}
    
HG_ERROR ParameterCompiler::dumpXML(const char* srcFile, const char* dstFile)
{
    HG_ERROR result = HG_OK;
    Data* data = parseXML(srcFile, &result);
    
    if (data != NULL)
        data->writeToFile(dstFile, true);
    
    delete data;
    
    return result;
}

Data* ParameterCompiler::parseXML(const char* srcFile, HG_ERROR *error)
{
    Data* data = NULL;
    HG_ERROR result = HG_OK;
    
    BREAK_START;
    
        Data* xmlData = Data::CreateFromFileContent(srcFile);
        if (xmlData == NULL)
        {
            result = HG_ERROR_FILE_NOT_EXIST;
            break;
        }
        
        resetPointers();
        
        xml_document<> doc;
        std::vector<char> buffer;
        try
        {
            buffer.insert(buffer.end(), xmlData->getData(), xmlData->getData() + xmlData->getLength());
            buffer.push_back('\0');
            doc.parse<0>(&buffer[0]);
        }
        catch (const parse_error &e)
        {
            HGLog("%s\n", e.what());
            result = HG_ERROR_FILE_CORRUPT;
            break;
        }
        
        _parseXML(doc.first_node());
        
        data = _writeMemory();
        
        doc.clear();
        
        clear();
        
        delete xmlData;
        
    BREAK_END;
    
    if (error != NULL)
        *error = result;
    
    return data;
}

Data* ParameterCompiler::compileMemory()
{
    return _writeMemory();
}


// clean all nodes, free memory
void ParameterCompiler::clear()
{
    if (mpObjectList != NULL)
    {
        // traverse object list
        for (object_node_t *node = mpObjectList; node != NULL; )
        {
            object_node_t *tempNode = node;
            node = node->next;
            
            // traverse component list
            for (component_node_t *compNode = tempNode->component_list; compNode != NULL; )
            {
                component_node_t *tempComp = compNode;
                compNode = compNode->next;
                
                // traverse parameter list
                for (parameter_node_t * paraNode = tempComp->parameter_list; paraNode != NULL; )
                {
                    parameter_node_t *tempPara = paraNode;
                    paraNode = paraNode->next;
                    
                    if (tempPara->data != NULL)
                    {
                        delete[] tempPara->data;
                    }
                    delete tempPara;
                } // parameter
                delete tempComp;
            }// component
            delete tempNode;
        }// object
    }
    
    resetPointers();
}

void ParameterCompiler::_addObject(uint32_t name)
{
    // create new object node
    object_node_t *temp = new object_node_t;
    memset(temp, 0, sizeof(object_node_t));
    temp->basic_info.first_child_offset = PARAM_NULL_OFFSET;
    temp->basic_info.next_node_offset   = PARAM_NULL_OFFSET;
    temp->basic_info.parent_node_offset = PARAM_NULL_OFFSET;
    temp->basic_info.type               = TypeHashObject;
    temp->basic_info.name               = name;
    temp->next                          = NULL;
    temp->component_list                = NULL;
    // create done
    
    // save the start offset of the current object
    mCurObjOffset = mVirtualFilePointer;
    
    // insert object node
    if (mpCurObject == NULL)
    {
        // point object list to first object node
        mpObjectList = temp;
        mpCurObject = temp;
        mCurObjOffset = 0;
    }
    else
    {
        // save the next offset of the current object
        mpCurObject->basic_info.next_node_offset = mVirtualFilePointer;
        
        // insert the newly created node
        mpCurObject->next = temp;
        mpCurObject = temp;
        
        // clear the current component list (new object -> new components)
        // if current component has no parameter node, set its first_child_offset
        // as the new object's offset
        if (mpCurComponent->parameter_list == NULL)
        {
            mpCurComponent->basic_info.first_child_offset = mVirtualFilePointer;
        }
        
        mpCurComponent = NULL;
    }
    
    // simulate file writing
    mVirtualFilePointer += BASIC_NODE_SIZE;
}

void ParameterCompiler::_addComponent(uint32_t name)
{
    // no current object, can't add component
    if (mpCurObject == NULL)
        return;
    
    // create new component node
    component_node_t *temp = new component_node_t;
    memset(temp, 0, sizeof(component_node_t));
    temp->basic_info.first_child_offset = PARAM_NULL_OFFSET;
    temp->basic_info.next_node_offset   = PARAM_NULL_OFFSET;
    temp->basic_info.parent_node_offset = mCurObjOffset;
    temp->basic_info.type               = TypeHashComponent;
    temp->basic_info.name               = name;
    
    temp->parameter_list                = NULL;
    temp->next                          = NULL;
    // create complete
    
    // save current component's offset
    mCurCompOffset = mVirtualFilePointer;
    
    // insert new component node
    if (mpCurComponent == NULL)
    {
        mpCurComponent = temp;
        mpCurObject->component_list = temp;
        
        // save first_child_offset of current object node
        if (mpCurObject != NULL)
        {
            mpCurObject->basic_info.first_child_offset = mVirtualFilePointer;
        }
    }
    else
    {
        // save current component node's next_node_offset
        mpCurComponent->basic_info.next_node_offset = mVirtualFilePointer;
        mpCurComponent->next = temp;
        
        if (mpCurComponent->parameter_list == NULL)
        {
            mpCurComponent->basic_info.first_child_offset = mVirtualFilePointer;
        }
        
        mpCurComponent = temp;
    }
    
    // clean the current parameter list
    if (mpCurParameterNode != NULL)
    {
        mpCurParameterNode->basic_info.next_node_offset = PARAM_NULL_OFFSET;
    }
    
    mpCurParameterNode = NULL;
    
    // simulate the file writing
    mVirtualFilePointer += BASIC_NODE_SIZE;
}

void ParameterCompiler::_addParameterNode(uint32_t type, uint32_t name)
{
    // no current component, can't add parameter
    if (mpCurComponent == NULL)
        return;
    
    // create new parameter node
    parameter_node_t *temp = new parameter_node_t;
    memset(temp, 0, sizeof(parameter_node_t));
    temp->basic_info.first_child_offset = PARAM_NULL_OFFSET;
    temp->basic_info.next_node_offset   = PARAM_NULL_OFFSET;
    temp->basic_info.parent_node_offset = mCurCompOffset;
    temp->basic_info.type               = type;
    temp->basic_info.name               = name;
    
    temp->data_size                     = 0;
    temp->data                          = NULL;
    temp->next                          = NULL;
    // create done
    
    // insert
    if (mpCurParameterNode == NULL)
    {
        mpCurParameterNode = temp;
        mpCurComponent->parameter_list = temp;
        
        mpCurComponent->basic_info.first_child_offset = mVirtualFilePointer;
    }
    else
    {
        mpCurParameterNode->next = temp;
        mpCurParameterNode->basic_info.next_node_offset = mVirtualFilePointer;
        mpCurParameterNode = temp;
    }
    
    mVirtualFilePointer += BASIC_NODE_SIZE;
}

// ParameterNode should always have some data to append
void ParameterCompiler::_appendDataForParameterNode(char* data, uint32_t dataSize)
{
    if (mpCurParameterNode == NULL || dataSize == 0)
        return;
    
    if (mpCurParameterNode->data != NULL)
    {
        delete[] mpCurParameterNode->data;
        
        // virtual file pointer roll back
        mVirtualFilePointer -= mpCurParameterNode->data_size;
        mpCurParameterNode->data = NULL;
        mpCurParameterNode->data_size = 0;
    }
    
    mpCurParameterNode->data_size = dataSize;
    mpCurParameterNode->data = new char[dataSize];
    memcpy(mpCurParameterNode->data, data, dataSize);
    
    mVirtualFilePointer += dataSize;
}


void ParameterCompiler::resetPointers()
{
    mpObjectList        = NULL;
    mpCurObject         = NULL;
    mpCurComponent      = NULL;
    mpCurParameterNode  = NULL;
    mLastNodeType       = INVALID_TYPE;
    mVirtualFilePointer = 0;
    mCurObjOffset       = 0;
    mCurCompOffset      = 0;
}


void ParameterCompiler::_parseXML(xml_node<> *pParent)
{
    if (!pParent)
        return;
    
    xml_node<> *pChild = NULL;
    
    for (pChild = pParent->first_node(); pChild != NULL; pChild = pChild->next_sibling())
    {
        if (pChild->type() == node_comment)
            continue;
        
        // dump object
        std::string strValue = pChild->name();
        if (Hash(strValue) == TypeHashObject)
        {
            xml_attribute<> *attrib = pChild->first_attribute();
            if (attrib != NULL)
			{
                _addObject(Hash(attrib->value()));
			}
        }
        
        // dump component
        if (Hash(strValue) == TypeHashComponent)
        {
            xml_attribute<> *attrib = pChild->first_attribute();
            if (attrib != NULL)
                _addComponent(Hash(attrib->value()));
        }
        
        // dump component parameters
        _parseParamTypes(pChild);
        
        // dump data according to last parameter type
        if (pChild->type() == node_data)
            _parseParameters(pChild);
        
        _parseXML(pChild);
    }
}


void ParameterCompiler::_parseParamTypes(xml_node<> *pNode)
{
    if (pNode == NULL)
        return;
    
    std::string strValue = pNode->name();
    
    // hash value
    if (Hash(strValue) == TypeHashHash)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = HASH;
            _addParameterNode(TypeHashHash, Hash(attrib->value()));
            
            // hash node might missing string data, fill with default in case
            uint32_t hashValue = Hash("");
            _appendDataForParameterNode((char *)&hashValue, sizeof(uint32_t));
        }
    }
    
    // boolean value
    if (Hash(strValue) == TypeHashBool)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = BOOLEAN;
            
            _addParameterNode(TypeHashBool, Hash(attrib->value()));
            
            uint32_t booleanValue = 0;
            _appendDataForParameterNode((char *)&booleanValue, sizeof(uint32_t));
        }
    }
    
    // string value
    if (Hash(strValue) == TypeHashString)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = STRING;
            
            _addParameterNode(TypeHashString, Hash(attrib->value()));
            
            // default string value
            char stringData[4];
            stringData[0] = (char)0x00;
            stringData[1] = (char)0xF0;
            stringData[2] = (char)0xFA;
            stringData[3] = (char)0x12;
            _appendDataForParameterNode(stringData, 4);
        }
    }
    
    // integer value
    if (Hash(strValue) == TypeHashInt)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = INTEGER;
            
            _addParameterNode(TypeHashInt, Hash(attrib->value()));
            
            // default integer value
            int32_t intValue = 0;
            _appendDataForParameterNode((char *)&intValue, sizeof(int32_t));
        }
    }
    
    // unsigned integer value
    if (Hash(strValue) == TypeHashUInt)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = UINTEGER;
            
            _addParameterNode(TypeHashUInt, Hash(attrib->value()));
            
            // default unsigned integer value
            uint32_t uintValue = 0;
            _appendDataForParameterNode((char *)&uintValue, sizeof(uint32_t));
        }
    }
    
    // float value
    if (Hash(strValue) == TypeHashFloat)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = FLOAT;
            
            _addParameterNode(TypeHashFloat, Hash(attrib->value()));
            
            // default float value
            float floatValue = 0;
            _appendDataForParameterNode((char *)&floatValue, sizeof(float));
        }
    }
    
    // double value
    if (Hash(strValue) == TypeHashDouble)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = DOUBLE;
            
            _addParameterNode(TypeHashDouble, Hash(attrib->value()));
            
            // default float value
            double doubleValue = 0;
            _appendDataForParameterNode((char *)&doubleValue, sizeof(float));
        }
    }
    
    // hex value
    if (Hash(strValue) == TypeHashHex)
    {
        xml_attribute<> *attrib = pNode->first_attribute();
        if (attrib != NULL)
        {
            mLastNodeType = HEX;
            
            _addParameterNode(TypeHashHex, Hash(attrib->value()));
            
            uint32_t hexValue = 0;
            _appendDataForParameterNode((char *)&hexValue, sizeof(uint32_t));
        }
    }
}

void ParameterCompiler::_parseParameters(xml_node<> *pNode)
{
    if (pNode == NULL)
        return;
    
    std::string text = pNode->value();
    if (text.length() == 0)
        return;
    
    switch (mLastNodeType)
    {
        case STRING:
        {
            char *data = NULL;
            uint32_t len = (uint32_t)text.length() + 1;
            uint32_t padLen = (len % 4) == 0 ? 0 : (4 - len % 4);
            char padData[3];
            padData[0] = (char)0xF0;
            padData[1] = (char)0xFA;
            padData[2] = (char)0x12;
            data = new char[len + padLen];
            memcpy(data, text.c_str(), len);
            if (padLen != 0)
            {
                memcpy(data+len, padData, padLen);
                len += padLen;
            }
            _appendDataForParameterNode(data, len);
            delete[] data;
            break;
        }
            
        case HASH:
        {
            uint32_t hashValue = Hash(text);
            _appendDataForParameterNode((char *)&hashValue, sizeof(uint32_t));
            break;
        }
            
        case BOOLEAN:
        {
            uint32_t boolValue = (Hash(text) == Hash("true")) ? 1 : 0;
            _appendDataForParameterNode((char *)&boolValue, sizeof(uint32_t));
            break;
        }
            
        case INTEGER:
        {
            int32_t intValue = atoi(text.c_str());
            _appendDataForParameterNode((char *)&intValue, sizeof(int32_t));
            break;
        }
        
        case UINTEGER:
        {
            uint32_t uintValue = (uint32_t)atol(text.c_str());
            _appendDataForParameterNode((char *)&uintValue, sizeof(uint32_t));
            break;
        }
            
        case FLOAT:
        {
            float floatValue = (float)atof(text.c_str());
            _appendDataForParameterNode((char *)&floatValue, sizeof(float));
            break;
        }
            
        case DOUBLE:
        {
            double doubleValue = atof(text.c_str());
            _appendDataForParameterNode((char *)&doubleValue, sizeof(double));
            break;
        }
            
        case HEX:
        {
            uint32_t hexValue = (uint32_t)atoi(text.c_str());
            _appendDataForParameterNode((char *)&hexValue, sizeof(uint32_t));
        }
        
        default:
            break;
    }
}

Data* ParameterCompiler::_writeMemory()
{
    uint32_t pointer = 0;
    
    Data *data = Data::CreateEmpty();
    
    for (object_node_t * node = mpObjectList; node != NULL; node = node->next)
    {
        data->appendData((char *)&node->basic_info, BASIC_NODE_SIZE);
        //memcpy(dst + pointer, node, BASIC_NODE_SIZE);
        pointer += BASIC_NODE_SIZE;
        
        for (component_node_t * compNode = node->component_list; compNode != NULL; compNode = compNode->next)
        {
            data->appendData((char *)&compNode->basic_info, BASIC_NODE_SIZE);
            //memcpy(dst + pointer, compNode, BASIC_NODE_SIZE);
            pointer += BASIC_NODE_SIZE;
            
            // traverse parameter list
            for (parameter_node_t * paraNode = compNode->parameter_list; paraNode != NULL; paraNode = paraNode->next)
            {
                data->appendData((char *)&paraNode->basic_info, BASIC_NODE_SIZE);
                //memcpy(dst + pointer, paraNode, BASIC_NODE_SIZE);
                pointer += BASIC_NODE_SIZE;
                
                if (paraNode->data != NULL)
                {
                    data->appendData(paraNode->data, sizeof(char) * paraNode->data_size);
                    //memcpy(dst + pointer, paraNode->data, paraNode->data_size);
                    pointer += paraNode->data_size;
                }
            }// parameter
        }// component
    }// object
    
    return data;
}

// auxiliary methods
    
void ParameterCompiler::addObject(const char* name)
{
    if (name != NULL)
    {
        _addObject(Hash(name));
    }
}

void ParameterCompiler::addComponent(const char* name)
{
    if (name != NULL)
    {
        _addComponent(Hash(name));
    }
}

void ParameterCompiler::addParamInt(const char* name, const int32_t data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashInt, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(int32_t));
    }
}

void ParameterCompiler::addParamUInt(const char* name, const uint32_t data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashUInt, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(uint32_t));
    }
}

void ParameterCompiler::addParamFloat(const char* name, const float data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashFloat, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(float));
    }
}

void ParameterCompiler::addParamDouble(const char* name, const double data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashDouble, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(double));
    }
}

void ParameterCompiler::addParamBool(const char* name, const bool data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashBool, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(uint32_t));
    }
}

void ParameterCompiler::addParamString(const char* name, const char* data)
{
    if (name != NULL)
    {
        std::string str(data);
        uint32_t len = (uint32_t)str.length() + 1;
        uint32_t padLen = (len % 4) == 0 ? 0 : (4 - len % 4);
        char padData[3];
        padData[0] = (char)0xF0;
        padData[1] = (char)0xFA;
        padData[2] = (char)0x12;
        char *temp = new char[len + padLen];
        memcpy(temp, str.c_str(), len);
        if (padLen != 0)
        {
            memcpy(temp + len, padData, padLen);
            len += padLen;
        }
        
        _addParameterNode(TypeHashString, Hash(name));
        _appendDataForParameterNode(temp, len);
        delete[] temp;
    }
}

void ParameterCompiler::addParamHex(const char* name, const uint32_t data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashHex, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(uint32_t));
    }
}

void ParameterCompiler::addParamHash(const char *name, const uint32_t data)
{
    if (name != NULL)
    {
        _addParameterNode(TypeHashHash, Hash(name));
        _appendDataForParameterNode((char *)&data, sizeof(uint32_t));
    }
}

}

