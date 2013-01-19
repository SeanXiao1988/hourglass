/**
 *  @file    HGQuadEntity.cpp
 *  @brief   
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/06/15
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

#include "HGQuadEntity.h"
#include "HGSceneNode.h"

#include "HGHash.h"
#include "HGObjectManager.h"

#include "HGRender.h"

HGNAMESPACE_START

QuadEntity::QuadEntity()
	: ISceneEntity()
    , mU(0.0f)
    , mV(0.0f)
    , mWidth(0.0f)
    , mHeight(0.0f)
    , mTexWidth(0.0f)
    , mTexHeight(0.0f)
    , mCenterX(0.0f)
    , mCenterY(0.0f)
    // animation
    , mSpriteAnimationList(NULL)
    , mCurrentSpriteAnimation(NULL)
    , mCurrentFrame(NULL)
    , mIsSpriteAnimating(false)
    , mCurFrameTimer(0.0f)
    , mSpriteAnimationStage(ANIMATION_STAGE_FORWARD)
{
    memset(&mQuad, 0, sizeof(Quad));
    mQuad.tex = NULL;
    mQuad.blend = BLEND_DEFAULT;
    
    setColor(0xFFFFFFFF);
}

QuadEntity::~QuadEntity()
{
    deInitSpriteAnimation();
    
    if (mQuad.tex != 0)
    {
        RENDER.textureFree(mQuad.tex);
    }
}
    
void QuadEntity::update(const float dt)
{
    updateSpriteAnimation(dt);
}

void QuadEntity::setTexture(GLuint tex)
{
    if (tex == 0) 
    {
        mU = 0.0f;
        mV = 0.0f;
        mTexWidth = 0.0f;
        mTexHeight = 0.0f;
        
        mQuad.v[0].u = mQuad.v[0].v = 0.0f;
        mQuad.v[1].u = mQuad.v[1].v = 0.0f;
        mQuad.v[2].u = mQuad.v[2].v = 0.0f;
        mQuad.v[3].u = mQuad.v[3].v = 0.0f;
        mQuad.tex = 0;
    }
    else
    {
        mTexWidth = (float)RENDER.textureGetWidth(tex);
        mTexHeight = (float)RENDER.textureGetHeight(tex);
        mQuad.tex = tex;
    }

}

void QuadEntity::setTextureRect(float x, float y, float w, float h, bool adjust)
{
    mU = x;
    mV = y;
    
    if (mTexWidth > 0.0f && mTexHeight > 0.0f)
    {
        if (adjust)
        {
            mWidth = w;
            mHeight = h;
            mTexWidth = (float)RENDER.textureGetWidth(mQuad.tex);
            mTexHeight = (float)RENDER.textureGetHeight(mQuad.tex);
        }
        
        mQuad.v[0].u = mU / mTexWidth;
        mQuad.v[0].v = mV / mTexHeight;
        
        mQuad.v[2].u = (mU + w) / mTexWidth;
        mQuad.v[2].v = (mV + h) / mTexHeight;
        
        mQuad.v[1].u = mQuad.v[2].u;
        mQuad.v[1].v = mQuad.v[0].v;
        
        mQuad.v[3].u = mQuad.v[0].u;
        mQuad.v[3].v = mQuad.v[2].v;
    }
}
    
void QuadEntity::getTextureRect(float* x, float* y, float* w, float* h) const
{
    if (x != NULL)
        *x = mU;
    
    if (y != NULL)
        *y = mV;
    
    if (w != NULL)
        *w = mWidth;
    
    if (h != NULL)
        *h = mHeight;
}

const uint32_t QuadEntity::getColor(int i) const
{
    uint32_t ret = RGBA(mQuad.v[i].color[0], mQuad.v[i].color[1], mQuad.v[i].color[2], mQuad.v[i].color[3]);
    return ret;
}

void QuadEntity::setColor(uint32_t col, int i)
{
    if (i == -1)
    {
        for (int j = 0; j < 4; j++)
        {
            mQuad.v[j].color[0] = GETR(col);
            mQuad.v[j].color[1] = GETG(col);
            mQuad.v[j].color[2] = GETB(col);
            mQuad.v[j].color[3] = GETA(col);
        }
    }
    else
    {
        mQuad.v[i].color[0] = GETR(col);
        mQuad.v[i].color[1] = GETG(col);
        mQuad.v[i].color[2] = GETB(col);
        mQuad.v[i].color[3] = GETA(col);
    }
}

void QuadEntity::setVertexAlpha(uint8_t alpha, int i)
{
    if (i == -1)
    {
        for (int j = 0; j < 4; j++)
        {
            mQuad.v[j].color[3] = alpha;
        }
    }
    else
    {
        mQuad.v[i].color[3] = alpha;
    }
}

void QuadEntity::setVertexZ(float z, int i)
{
    if (i == -1)
    {
        for (int j = 0; j < 4; j++)
        {
            mQuad.v[j].z = z;
        }
    }
    else
    {
        mQuad.v[i].z = z;
    }
}

void QuadEntity::render()
{
	if (mSceneNode == NULL)
		return;
    
    setVertexAlpha((uint8_t)mSceneNode->getRenderAlpha());
    
    glm::mat4& mat = mSceneNode->getMatrix();
    glm::vec4 result = mat * glm::vec4(-mWidth/2.0f - mCenterX, -mHeight/2.0f - mCenterY, mQuad.v[0].z, 1.0f);
    mQuad.v[0].x = result[0];
    mQuad.v[0].y = result[1];
    mQuad.v[0].z = result[2];
    
    result =  mat * glm::vec4(mWidth/2.0f - mCenterX, -mHeight/2.0f - mCenterY, mQuad.v[1].z, 1.0f);
    mQuad.v[1].x = result[0];
    mQuad.v[1].y = result[1];
    mQuad.v[1].z = result[2];
    
    result = mat * glm::vec4(mWidth/2.0f - mCenterX, mHeight/2.0f - mCenterY, mQuad.v[2].z, 1.0f);
    mQuad.v[2].x = result[0];
    mQuad.v[2].y = result[1];
    mQuad.v[2].z = result[2];
    
    result = mat * glm::vec4(-mWidth/2.0f - mCenterX, mHeight/2.0f - mCenterY, mQuad.v[3].z, 1.0f);   
    mQuad.v[3].x = result[0];
    mQuad.v[3].y = result[1];
    mQuad.v[3].z = result[2];
    
    RENDER.renderQuad(&mQuad);
}
    
// TODO: replace c style list to stl containers
// animation
bool QuadEntity::initSpriteAnimation(sprite_frames_header_t *anim)
{
    bool ret = false;
    
    BREAK_START;
    
    if (anim == NULL)
        break;
    
    deInitSpriteAnimation();
    
    // copy animation nodes
    sprite_frames_header_t* currentHeader = NULL;
    
    for (sprite_frames_header_t* srcNode = anim; srcNode != NULL; srcNode = srcNode->next)
    {
        sprite_frames_header_t* newHeader = new sprite_frames_header_t;
        memset(newHeader, 0, sizeof(sprite_frames_header_t));
        newHeader->name = srcNode->name;
        newHeader->type = srcNode->type;
        newHeader->pFrames = NULL;
        newHeader->fileNameHash = srcNode->fileNameHash;
        newHeader->next = NULL;
        
        if (mSpriteAnimationList == NULL)
        {
            currentHeader = newHeader;
            mSpriteAnimationList = newHeader;
        }
        else
        {
            currentHeader->next = newHeader;
            currentHeader = newHeader;
        }
        
        sprite_frame_node_t* currentFrame = NULL;
        for (sprite_frame_node_t* subNode = srcNode->pFrames; subNode != NULL; subNode = subNode->next)
        {
            sprite_frame_node_t* frameNode = new sprite_frame_node_t;
            memcpy(frameNode, subNode, sizeof(sprite_frame_node_t));
            
            frameNode->next = NULL;
            frameNode->prev = NULL;
            
            if (currentHeader->pFrames == NULL)
            {
                currentHeader->pFrames = frameNode;
                currentFrame = frameNode;
                frameNode->prev = NULL;
            }
            else
            {
                currentFrame->next = frameNode;
                frameNode->prev = currentFrame;
                currentFrame = frameNode;
            }
        }
    }
    
    BREAK_END;
    
    return ret;
}
    
void QuadEntity::deInitSpriteAnimation()
{
    if (mSpriteAnimationList != NULL)
    {
        for (sprite_frames_header_t* node = mSpriteAnimationList; node != NULL; )
        {
            sprite_frames_header_t* tempNode = node;
            node = node->next;
            
            for (sprite_frame_node_t* frameNode = tempNode->pFrames; frameNode != NULL; )
            {
                sprite_frame_node_t* tempFrame = frameNode;
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
        
        mIsSpriteAnimating        = false;
        mCurFrameTimer      = 0.0f;
        mCurrentSpriteAnimation   = NULL;
        mCurrentFrame       = NULL;
        mSpriteAnimationList      = NULL;
    }
}
    
bool QuadEntity::playSpriteAnimation(uint32_t name)
{
    bool ret = false;
    BREAK_START;
    
    if (mSpriteAnimationList == NULL)
        break;

    if (mCurrentFrame != NULL)
    {
        // animationList[name] is already in playing
        if (mCurrentSpriteAnimation->name == name)
        {
            ret = true;
            break;
        }
    }
    
    // find animationList[name]
    sprite_frames_header_t* node = NULL;
    for (node = mSpriteAnimationList; node != NULL; node = node->next)
    {
        if (node->name == name)
        {
            mCurrentSpriteAnimation = node;
            mCurrentFrame = mCurrentSpriteAnimation->pFrames;
            
            setTexture(RENDER.textureLoad(node->fileNameHash));
            break;
        }
    }
    
    // animationList[name] not found
    if (node == NULL)
        break;
    
    setFrame(mCurrentFrame);
    mCurFrameTimer = 0.0f;
    mSpriteAnimationStage = ANIMATION_STAGE_FORWARD;
    mIsSpriteAnimating = true;
    
    ret = true;
    
    BREAK_END;
    
    return ret;
}
    
void QuadEntity::setFrame(sprite_frame_node_t *frame)
{
    if (frame == NULL)
        return;
    
    mWidth = frame->width;
    mHeight = frame->height;
    
    setTextureRect(frame->sourceX, frame->sourceY, frame->width, frame->height, false);
    
    mCenterX = frame->offsetX;
    mCenterY = frame->offsetY;
}
    
void QuadEntity::stopSpriteAnimation(bool reset)
{
    mIsSpriteAnimating = false;
    
    if (reset && mCurrentSpriteAnimation != NULL)
        mCurrentFrame = mCurrentSpriteAnimation->pFrames;
    
    mCurFrameTimer = 0.0f;
}
    
void QuadEntity::pauseSpriteAnimation()
{
    mIsSpriteAnimating = false;
}
    
void QuadEntity::resumeSpriteAnimation()
{
    mIsSpriteAnimating = true;
}
    
void QuadEntity::updateSpriteAnimation(float delta)
{
    BREAK_START;
    
    if (!mIsSpriteAnimating)
        break;
    
    if (mCurrentSpriteAnimation == NULL || mCurrentFrame == NULL)
        break;
    
    mCurFrameTimer += delta;
    if (mCurFrameTimer < mCurrentFrame->delta)
        break;
    
    mCurFrameTimer = 0.0f;
    nextFrame();
    
    BREAK_END;
}
    
void QuadEntity::nextFrame()
{
    // forwarding
    if (mSpriteAnimationStage == ANIMATION_STAGE_FORWARD)
    {
        // still can move forward
        if (mCurrentFrame->next != NULL)
        {
            mCurrentFrame = mCurrentFrame->next;
        }
        // animation sequence ended
        else if (mCurrentSpriteAnimation->type == ANIMATION_TYPE_RESET)
        {
            mSpriteAnimationStage = ANIMATION_STAGE_FORWARD;
            mCurrentFrame = mCurrentSpriteAnimation->pFrames;
        }
        else if (mCurrentSpriteAnimation->type == ANIMATION_TYPE_PINGPONG)
        {
            mSpriteAnimationStage = ANIMATION_STAGE_REVERSE;
            mCurrentFrame = (mCurrentFrame->prev == NULL) ? mCurrentFrame : mCurrentFrame->prev;
        }
    }
    else if (mSpriteAnimationStage == ANIMATION_STAGE_REVERSE)
    {
        // still can move backward
        if (mCurrentFrame->prev != NULL)
        {
            mCurrentFrame = mCurrentFrame->prev;
        }
        else
        {
            mSpriteAnimationStage = ANIMATION_STAGE_FORWARD;
            mCurrentFrame = (mCurrentFrame->next == NULL) ? mCurrentFrame : mCurrentFrame->next;
        }
    }
    
    setFrame(mCurrentFrame);
}
    
void QuadEntity::prevFrame()
{
    // forwarding
    if (mSpriteAnimationStage == ANIMATION_STAGE_FORWARD)
    {
        if (mCurrentFrame->prev != NULL)
        {
            mCurrentFrame = mCurrentFrame->prev;
        }
        else if (mCurrentSpriteAnimation->type == ANIMATION_TYPE_RESET)
        {
            mSpriteAnimationStage = ANIMATION_STAGE_FORWARD;
            mCurrentFrame = mCurrentSpriteAnimation->pFrames;
            while (mCurrentFrame->next != NULL)
                mCurrentFrame = mCurrentFrame->next;
        }
        else if (mCurrentSpriteAnimation->type == ANIMATION_TYPE_PINGPONG)
        {
            mSpriteAnimationStage = ANIMATION_STAGE_REVERSE;
            mCurrentFrame = (mCurrentFrame->next == NULL) ? mCurrentFrame :mCurrentFrame->next;
        }
    }
    else if (mSpriteAnimationStage == ANIMATION_STAGE_REVERSE)
    {
        if (mCurrentFrame->next != NULL)
        {
            mCurrentFrame = mCurrentFrame->next;
        }
        else
        {
            mSpriteAnimationStage = ANIMATION_STAGE_FORWARD;
            mCurrentFrame = (mCurrentFrame->prev == NULL) ? mCurrentFrame : mCurrentFrame->prev;
        }
    }
    
    setFrame(mCurrentFrame);
}
    
void QuadEntity::RegisterComponentType(void)
{

}
    
void QuadEntity::deInitialize()
{
    
}
    
EventResult QuadEntity::handleEvent(const Event &event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t QuadEntity::getComponentName()
{
    return Hash("QuadEntity");
}
    
HGNAMESPACE_END