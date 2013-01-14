/**
 *  @file    HGSpriteAnimationCompiler.h
 *  @brief   Sprite Animation xml file compiler
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

#ifndef HGSPRITEANIMATIONCOMPILER_H_
#define HGSPRITEANIMATIONCOMPILER_H_

#include "HGSystem.h"
#include "HGAnimationDef.h"
#include "HGError.h"
#include "rapidxml.hpp"

namespace HG
{
    
#define SPRITEANIMATIONCOMPILER SpriteAnimationCompiler::getInstance()
    
class Data;

class SpriteAnimationCompiler
{
public:
    static SpriteAnimationCompiler& getInstance()
    {
        static SpriteAnimationCompiler instance;
        return instance;
    }
    
    void    initialize();
    void    deInitialize();
    
    void    clear();
    
    sprite_frames_header_t* parseXML(const char* srcFile, HG_ERROR *error = NULL);
    sprite_frames_header_t* getAnimation() { return mAnimationList; }
        
private:
    void    _parseXML(rapidxml::xml_node<> *pNode);
    void    _addAnimation(const char* name, uint32_t texHash, ANIMATION_TYPE type);
    bool    _addFrame();
    
    void    _parseAnimations(rapidxml::xml_node<> *pNode);
    void    _parseParamTypes(rapidxml::xml_node<> *pNode);
    void    _parseParameters(rapidxml::xml_node<> *pNode);
    
    sprite_frames_header_t* mAnimationList;
    sprite_frames_header_t* mCurrentAnimation;
    sprite_frame_node_t*	mCurrentFrame;
    
    enum VAR_TYPE
    {
        INVALID_TYPE = 0,
        SOURCE_X,
        SOURCE_Y,
        Z,
        WIDTH,
        HEIGHT,
        OFFSET_X,
        OFFSET_Y,
        DELTA
    };
    
    VAR_TYPE    mLastNodeType;
    
private:
    SpriteAnimationCompiler();
    ~SpriteAnimationCompiler();
    SpriteAnimationCompiler(const SpriteAnimationCompiler& other);
    SpriteAnimationCompiler& operator= (const SpriteAnimationCompiler& rhs);
};
    
}

#endif // HGSPRITEANIMATIONCOMPILER_H_
