/**
 *  @file    HGSceneNode.h
 *  @brief   Scene node
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

#ifndef HGSCENENODE_H_
#define HGSCENENODE_H_

#include "HGSystem.h"
#include "HGEvent.h"
#include "HGIComponent.h"

HGNAMESPACE_START

const GLfloat Z_INCREMENT = 0.1f;

class SceneNode;
typedef std::vector<SceneNode *>SceneNodeList;

class IAnimation;
typedef std::list<IAnimation* > AnimationList;
typedef std::set<uint32_t> AnimationSet;
    
class ISceneEntity;

class SceneNode : public IComponent
{
public:
    ~SceneNode();
    
    // Component
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_SCENE_NODE; }
    virtual uint32_t        getComponentName();
    
    void            update(const float dt);
    void            render();

    void            setAlpha(float alpha) { mAlpha = alpha; }
    void            setX(float x) { mX = x; }
    void            setY(float y) { mY = y; }
    void            setXY(float x, float y) { mX = x; mY = y; }
    void            setScaleX(float sx) { mScaleX = sx; }
    void            setScaleY(float sy) { mScaleY = sy; }
    void            setScale(float sx, float sy) { mScaleX = sx; mScaleY = sy; }
    void            setRotation(float rot) { mRotation = rot; }
    void            setVisibility(bool visible) { mVisibility = visible; }

	void            setWorldX(float x);
	void            setWorldY(float y);
    void            setWorldXY(float x, float y);

    const float&    getAlpha() const { return mAlpha; }
    const float&    getX() const { return mX; }
    const float&    getY() const { return mY; }
    const float&    getScaleX() const { return mScaleX; }
    const float&    getScaleY() const { return mScaleY; }
    const float&    getRotation() const { return mRotation; }
    const bool&     getVisibility() const { return mVisibility; }

    const float&    getWorldX() const { return mWorldX; }
    const float&    getWorldY() const { return mWorldY; }
    
    int             getLayer();
    void            increaseLayer();
    void            decreaseLayer();
    void            bringToTop();
    void            sendToBottom();

    glm::mat4&      getMatrix() { return mMatrix; };
    
    const float&    getRenderAlpha() const { return mRenderAlpha; }
    
    // Animation
    void            updateAnimations(const float dt);
    
    void            addAnimation(IAnimation* anim, uint32_t name = 0);
    IAnimation*     getAnimation(uint32_t name);
    void            removeAnimation(uint32_t name);
    void            removeAllAnimations();
    
    // Scene Graph
    SceneNode*      getParent();
    void            setParent(SceneNode* node);
    void            addChild(SceneNode* node);
    void            removeChild(SceneNode* node);
    void            removeChildByName(const std::string& name);
    void            removeAll();

    const uint32_t  getHashName() const { return mHashName; }
    SceneNode*      getChild(const std::string& name);
    SceneNode*      getChild(uint32_t hashname);
    
    void            attachEntity(ISceneEntity* entity);
    void            detachEntity();
    ISceneEntity*   getEntity() { return mEntity; }
    
private:
    explicit SceneNode();
    SceneNode(const SceneNode& other);
    SceneNode& operator = (const SceneNode& rhs);

    // graphical members
    float   mAlpha;
    float   mRenderAlpha;
    float   mX;
    float   mY;
    float   mScaleX;
    float   mScaleY;
    float   mRotation;
    bool    mVisibility;
    
    float   mWorldX;
    float   mWorldY;

	bool	mIsWorldXNeedSet;
	bool	mIsWorldYNeedSet;
	float	mDestWorldX;
	float	mDestWorldY;
    
    glm::mat4 mMatrix;
    
    // animation
    AnimationList   mAnimations;
    AnimationSet    mAnimationSet;

    // scene graph
    SceneNode*      mParent;
    uint32_t        mHashName;
    SceneNodeList   mChildren;

    ISceneEntity*   mEntity;

    friend class SceneManager;
};

HGNAMESPACE_END

#endif // HGSCENENODE_H_
