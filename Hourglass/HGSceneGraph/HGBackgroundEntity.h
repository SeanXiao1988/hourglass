/**
 *  @file    HGBackgroundEntity.h
 *  @brief   Background entity header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/07
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2013 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGBACKGROUNDENTITY_H_
#define HGBACKGROUNDENTITY_H_

#include "HGISceneEntity.h"
#include "HGGraphicsDef.h"

HGNAMESPACE_START

class SceneNode;

typedef enum
{
    BACKGROUND_DEFAULT = 0,
    BACKGROUND_STATIC,
    BACKGROUND_REPEAT
}BackgroundType;

class BackgroundEntity : public ISceneEntity
{
public:
    BackgroundEntity();
    ~BackgroundEntity();
    
    virtual void    update(const float dt);
    virtual void    render();
    
    // methods
    void            setType(BackgroundType type);
    void            setWidth(float w) { mWidth = w; }
    void            setHeight(float h) { mHeight = h; }
    const float&    getWidth() const { return mWidth; }
    const float&    getHeight() const { return mHeight; }
    void            setTexture(GLuint tex);
    void            setTextureRect(float x, float y, float w, float h);
    void            setVertexAlpha(uint8_t alpha, int i=-1);
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_BACKGROUND_ENTITY; };
    virtual uint32_t        getComponentName();
    
private:
    BackgroundType  mType;
    Quad            mQuad;
    float           mWidth;
    float           mHeight;
    float           mTexWidth;
    float           mTexHeight;
};

HGNAMESPACE_END

#endif // HGBACKGROUNDENTITY_H_
