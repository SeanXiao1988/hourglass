/**
 *  @file    HGAnimationCompiler.cpp
 *  @brief   Animation compiler implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/03
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

#include "HGSpriteAnimationCompiler.h"
#include "HGData.h"
#include "HGHash.h"

using namespace rapidxml;

HGNAMESPACE_START
    
SpriteAnimationCompiler::SpriteAnimationCompiler()
    : mAnimationList(NULL)
    , mCurrentAnimation(NULL)
    , mCurrentFrame(NULL)
    , mLastNodeType(INVALID_TYPE)
{
}
    
SpriteAnimationCompiler::~SpriteAnimationCompiler()
{
    clear();
}
    
void SpriteAnimationCompiler::initialize()
{
    clear();
}
    
void SpriteAnimationCompiler::deInitialize()
{
    clear();
}
    
void SpriteAnimationCompiler::clear()
{
    if (mAnimationList != NULL)
    {
        for (sprite_frames_header_t *node = mAnimationList; node != NULL; )
        {
            sprite_frames_header_t *tempNode = node;
            node = node->next;
            
            for (sprite_frame_node_t *frameNode = tempNode->pFrames; frameNode != NULL; )
            {
                sprite_frame_node_t *tempFrame = frameNode;
                frameNode = frameNode->next;
                
                if (tempFrame != NULL)
                {
                    delete tempFrame;
                    tempFrame = NULL;
                }
            }
            
            delete tempNode;
            tempNode = NULL;
        }
    }
    
    mAnimationList = NULL;
}
    
sprite_frames_header_t* SpriteAnimationCompiler::parseXML(const char *srcFile, HG_ERROR *error)
{
    sprite_frames_header_t* ret = NULL;
    HG_ERROR result = HG_OK;
    
    BREAK_START;
    
        Data* xmlData = Data::CreateFromFileContent(srcFile);
        if (xmlData == NULL)
        {
            result = HG_ERROR_FILE_NOT_EXIST;
            break;
        }
        
        clear();
        
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
            break;
        }
        
        _parseXML(doc.first_node());
        
        ret = getAnimation();
    
        doc.clear();
        
        delete xmlData;
    
    BREAK_END;
    
    if (error != NULL)
        *error = result;
    
    return ret;
}
    
void SpriteAnimationCompiler::_parseXML(xml_node<> *pParent)
{
    if (!pParent)
        return;
    
    xml_node<> *pChild = NULL;
    
    for (pChild = pParent->first_node(); pChild != NULL; pChild = pChild->next_sibling())
    {
        if (pChild->type() == node_comment)
            continue;
        
        // dump animation header
        std::string strValue = pChild->name();
        if (Hash(strValue) == Hash("animation"))
        {
            _parseAnimations(pChild);
        }
        
        // dump frames
        if (Hash(strValue) == Hash("frame"))
        {
            _addFrame();
        }
        
        // dump parameters
        if (Hash(strValue) == Hash("float"))
        {
            _parseParamTypes(pChild);
        }
        
        // dump frame parameter
        _parseParameters(pChild);
        
        _parseXML(pChild);
    }
}
    
void SpriteAnimationCompiler::_parseAnimations(xml_node<> *pNode)
{
    if (pNode == NULL)
        return;
    
    xml_attribute<> *animationName = pNode->first_attribute();
    xml_attribute<> *animationType = NULL;
    xml_attribute<> *animationTexture = NULL;
    uint32_t textureFileHash;
    
    BREAK_START;
    
    if (animationName != NULL)
    {
        // texture
        animationTexture = animationName->next_attribute();
        if (animationTexture == NULL)
            break;
        
        std::string strName = animationTexture->name();
        if (Hash(strName) != Hash("texture"))
            break;
        
        textureFileHash = Hash(animationTexture->value());
        
        // animation type
        animationType = animationTexture->next_attribute();
        if (animationType == NULL)
        {
            _addAnimation(animationName->value(), textureFileHash, ANIMATION_TYPE_DEFAULT);
        }
        else
        {
            ANIMATION_TYPE type = ANIMATION_TYPE_DEFAULT;
            std::string strType = animationType->value();
            if (Hash(strType) == Hash("reset"))
                type = ANIMATION_TYPE_RESET;
            else if (Hash(strType) == Hash("pingpong"))
                type = ANIMATION_TYPE_PINGPONG;
            
            _addAnimation(animationName->value(), textureFileHash, type);
        }
    }
    
    BREAK_END;
}
    
void SpriteAnimationCompiler::_parseParamTypes(xml_node<> *pNode)
{
    if (pNode == NULL)
        return;
    
    xml_attribute<> *attrib = pNode->first_attribute();
    if (attrib != NULL)
    {
        std::string strParam = attrib->value();
        if (Hash(strParam) == Hash("sourcex"))
            mLastNodeType = SOURCE_X;
        else if (Hash(strParam) == Hash("sourcey"))
            mLastNodeType = SOURCE_Y;
        else if (Hash(strParam) == Hash("width"))
            mLastNodeType = WIDTH;
        else if (Hash(strParam) == Hash("height"))
            mLastNodeType = HEIGHT;
        else if (Hash(strParam) == Hash("offsetx"))
            mLastNodeType = OFFSET_X;
        else if (Hash(strParam) == Hash("offsety"))
            mLastNodeType = OFFSET_Y;
        else if (Hash(strParam) == Hash("delta"))
            mLastNodeType = DELTA;
        else
            mLastNodeType = INVALID_TYPE;
    }
}
    
void SpriteAnimationCompiler::_parseParameters(xml_node<> *pNode)
{
    if (pNode == NULL)
        return;
    
    if (pNode->type() == node_data && mCurrentFrame != NULL)
    {
        float value = 0.0f;
        std::string text = pNode->value();
        if (text.length() != 0)
        {
            value = (float)atof(text.c_str());
            switch (mLastNodeType)
            {
                case SOURCE_X:
                    mCurrentFrame->sourceX = value;
                    break;
                case SOURCE_Y:
                    mCurrentFrame->sourceY = value;
                    break;
                case WIDTH:
                    mCurrentFrame->width = value;
                    break;
                case HEIGHT:
                    mCurrentFrame->height = value;
                    break;
                case OFFSET_X:
                    mCurrentFrame->offsetX = value;
                    break;
                case OFFSET_Y:
                    mCurrentFrame->offsetY = value;
                    break;
                case DELTA:
                    mCurrentFrame->delta = value;
                    break;
                    
                default:
                    break;
            }
        }
    }
}
    
void SpriteAnimationCompiler::_addAnimation(const char *name, uint32_t texHash, ANIMATION_TYPE type)
{
    if (name == NULL)
        return;
    
    sprite_frames_header_t *newAnimation = new sprite_frames_header_t;
    memset(newAnimation, 0, sizeof(sprite_frames_header_t));
    newAnimation->name = Hash(name);
    newAnimation->fileNameHash = texHash;
    newAnimation->type = type;
    newAnimation->pFrames = NULL;
    newAnimation->next = NULL;
    
    if (mCurrentAnimation == NULL)
    {
        mAnimationList = newAnimation;
    }
    else
    {
        sprite_frames_header_t *tail = mAnimationList;
        while (tail->next != NULL)
            tail = tail->next;
        
        tail->next = newAnimation;
    }
    
    mCurrentAnimation = newAnimation;
}
    
bool SpriteAnimationCompiler::_addFrame()
{
    if (mCurrentAnimation == NULL)
        return false;
    
    sprite_frame_node_t *newFrame = new sprite_frame_node_t;
    memset(newFrame, 0, sizeof(sprite_frame_node_t));
    
    if (mCurrentAnimation->pFrames == NULL)
    {
        mCurrentAnimation->pFrames = newFrame;
    }
    else
    {
        sprite_frame_node_t *tail = mCurrentAnimation->pFrames;
        while (tail->next != NULL)
            tail = tail->next;
        
        tail->next = newFrame;
        newFrame->prev = tail;
    }
    
    mCurrentFrame = newFrame;
    
    return true;
}

HGNAMESPACE_END
