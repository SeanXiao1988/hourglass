/**
 *  @file    HGQuadEntity.h
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

#ifndef HGQUADENTITY_H_

#include "HGISceneEntity.h"
#include "HGGraphicsDef.h"
#include "HGAnimationDef.h"

HGNAMESPACE_START

class QuadEntity : public ISceneEntity
{
public:
	QuadEntity();
	~QuadEntity();

    virtual void    update(const float dt);
	virtual void    render();

	Quad*           getQuad()	{ return &mQuad; }
	void            setTexture(GLuint tex);
	void            setTextureRect(float x, float y, float w, float h, bool adjust = true);
	void            setColor(uint32_t col, int i = -1);
	void            setVertexAlpha(uint8_t alpha, int i = -1);
	void            setBlendMode(int blend = BLEND_DEFAULT) { mQuad.blend = blend; }
	void            setCenterPoint(float x, float y) { mCenterX = x; mCenterY = y; }
    void            setWidth(float width) { mWidth = width; }
	void            setHeight(float height) { mHeight = height; }

	const GLuint&	getTexture() const { return mQuad.tex; }
	void			getTextureRect(float* x, float* y, float* w, float* h) const;
	const uint32_t  getColor(int i = 0) const;
	const uint8_t   getVertexAlpha(int i = 0) const { return mQuad.v[i].color[3]; }
	const int       getBlendMode() const { return mQuad.blend; }
	void			getCenterPoint(float* x, float* y) const { *x = mCenterX; *y = mCenterY; }
    
    // sprite animation
    bool            initSpriteAnimation(sprite_frames_header_t *anim);
    void            deInitSpriteAnimation();
    
    bool            playSpriteAnimation(uint32_t name);
    void            setFrame(sprite_frame_node_t *frame);
    
    void            updateSpriteAnimation(float delta);
    void            nextFrame();
    void            prevFrame();
    
    void            stopSpriteAnimation(bool reset = true);
    void            pauseSpriteAnimation();
    void            resumeSpriteAnimation();
    bool            isSpriteAnimating() { return mIsSpriteAnimating; }
    sprite_frame_node_t*   getCurrentFrame();
    sprite_frames_header_t* getCurrentSpriteAnimation();
    sprite_frames_header_t* getSpriteAnimationList();
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_QUAD_ENTITY; };
    virtual uint32_t        getComponentName();

private:
    Quad    mQuad;

	float	mU;
	float	mV;
	float	mWidth;
	float	mHeight;
	float	mTexWidth;
	float	mTexHeight;
    float   mCenterX;
    float   mCenterY;
    
    // sprite animation
    sprite_frames_header_t* mSpriteAnimationList;
    sprite_frames_header_t* mCurrentSpriteAnimation;
    sprite_frame_node_t*	mCurrentFrame;
    bool					mIsSpriteAnimating;
    float					mCurFrameTimer;
    ANIMATION_STAGE			mSpriteAnimationStage;
};

HGNAMESPACE_END

#endif // HGQUADENTITY_H_
