/**
 *  @file    HGRender.h
 *  @brief   Render header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/18
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

#ifndef HGRENDER_H_
#define HGRENDER_H_

#include "HGSystem.h"
#include "HGGraphicsDef.h"
#include "HGGraphicsUtil.h"
#include "HGError.h"

#define HG_DEFAULT_FONT_FILE    "source_code_pro.ttf"
#define HG_DEFAULT_FONT_SIZE    14

HGNAMESPACE_START

#define RENDER Render::getInstance()
    
typedef std::map<GLuint, texture_t*> TextureMap;
typedef std::map<GLuint, rtarget_t*> RTargetMap;
typedef std::map<GLuint, shader_t*> ShaderMap;
typedef std::stack<GLuint> ShaderStack;
    
class Render
{
public:
    static Render& getInstance()
    {
        static Render instance;
        return instance;
    }
    
    // might need more param later
    HG_ERROR        initialize(int width, int height, bool fullscreen = false);
    void		    deInitialize();
    
    void		    setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void		    setClearColor(GLuint color);
    void            setClipping(int x=0, int y=0, int w=0, int h=0);
    void		    clear();
    HG_ERROR	    beginScene(GLuint target = 0);
    void		    endScene();
    
    Vertex*		    startBatch(int primType, GLuint tex, int blend, int *maxPrim);
    void		    finishBatch(int nprim);
    
    GLuint          textureCreate(int width, int height);
    void            textureFree(GLuint texture, bool force = false);
    int             textureGetWidth(GLuint texture);
    int             textureGetHeight(GLuint texture);
    char*           textureLock(GLuint texture, bool bReadOnly = true, int left = 0, int top = 0, int width = 0, int height = 0);
    void            textureUnlock(GLuint texture);
    GLuint          textureLoad(const char* filename);
    GLuint          textureLoad(uint32_t filenameHash);
    void            textureFreeAll();
    
    GLuint          rtargetCreate(int width, int height);
    void            rtargetFree(GLuint target);
    GLuint          rtargetGetTexture(GLuint target);
    rtarget_t*      rtargetFind(GLuint target);
    void            rtargetFreeAll();

	GLuint          shaderLoad(const char* filename, bool activate = false);
	void			shaderFree(GLuint shader);
	void			shaderActive(GLuint shader);
	void			shaderDeActive();
    void            shaderPush();
    void            shaderPop();
    GLuint          shaderGetCurrent() { return mCurShader; }
    void            shaderFreeAll();
	// TODO: pass parameters to shader
    
    void            renderLine(float x1, float y1, float x2, float y2, uint32_t color, float z);
    void            renderRect(float x, float y, float w, float h, uint32_t color, float z);
    void            renderBox(float x, float y, float w, float h, uint32_t color, float z);
    void            renderCircle(float cx, float cy, float r, uint32_t v, uint32_t color, float z, int fill = 0);
    void            renderQuad(const Quad *quad);
    void            renderTriple(const Triple *triple);
    void            renderVertexList(const VertexList* list);
    
    void            renderText(float x, float y, float z, uint32_t color,const char* format, ...);  // TODO, add font support
    void            setDefaultFontSize(uint32_t size);
    
    double          calculateFPS();
    const double    getFPS() const { return mFPS; }
    
    const int&      getWidth() const { return mWidth; }
    const int&      getHeight() const { return mHeight; }
	const bool&		isFullScreen() const { return mIsFullScreen; }
    
private:
    void			_renderBatch(bool bEndScene = false);
    void			_setupPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void			_setupOrtho(int width, int height, bool target=false);
    void            _setupProjection(int w, int h, bool flip);
    HG_ERROR        _initLib();
    HG_ERROR		_initOpenGL();
    HG_ERROR		_initBuffer();
    
    texture_t*      _findLoadedTexture(const char* filename);
    
    void            _bindTexture(GLuint texture);
    void            _setBlendMode(int mode);
    
    
    static inline GLuint _nextPow2(int raw);
    static inline bool	 _isPow2(int raw);
    
    int                 mWidth;
    int                 mHeight;
    bool                mIsFullScreen;
    
    // graphics
    Vertex*             mpVB;
    GLushort*           mpIB;
    Vertex*             mpVertices;
    GLuint              mIndexBufferObject;
    
    TextureMap          mTextureMap;
    GLuint              mCurTexture;
    
    RTargetMap          mRTargetMap;
    rtarget_t*          mCurRTarget;

    ShaderMap           mShaders;
    GLuint              mCurShader;
    ShaderStack         mShaderStack;
    
    int                 mPrimitive;
    int                 mCurPrimitiveType;
    int                 mCurBlenMode;
    GLenum              mLastCustomPrimitive;
    VertexList          mCurVertexList;
    
    unsigned char*		mBuffer;
    uint32_t			mBufferSize;
    
    // font
    TextureFontContext* mDefaultFont;
    
    // FPS counter
    int                 mFrames;
    double              mFPS;
    double              mTime0;
    double              mTime1;
    int                 mDebugVertexUsed;
    
private:
    Render();
    ~Render();
    Render(const Render& other);
    Render& operator= (const Render& rhs);
};
    
HGNAMESPACE_END



#endif // HGRENDER_H_
