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
#include "HGError.h"

namespace HG
{

#define RENDER Render::getInstance()
    
class Render
{
public:
    static Render& getInstance()
    {
        static Render instance;
        return instance;
    }
    
    // might need more param later
    HG_ERROR        initialize(int width, int height);
    void		    deInitialize();
    
    void		    setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void		    setClearColor(GLuint color);
    void            setClipping(int x=0, int y=0, int w=0, int h=0);
    void		    clear();
    HG_ERROR	    beginScene(HANDLE_RTARGET target = 0);
    void		    endScene();
    
    Vertex*		    startBatch(int primType, HANDLE_TEXTURE tex, int blend, int *maxPrim);
    void		    finishBatch(int nprim);
    
    HANDLE_TEXTURE  textureCreate(int width, int height);
    void            textureFree(HANDLE_TEXTURE texture, bool force = false);
    int             textureGetWidth(HANDLE_TEXTURE texture);
    int             textureGetHeight(HANDLE_TEXTURE texture);
    unsigned char*  textureLock(HANDLE_TEXTURE texture, bool bReadOnly = true, int left = 0, int top = 0, int width = 0, int height = 0);
    void            textureUnlock(HANDLE_TEXTURE texture);
    HANDLE_TEXTURE	textureLoad(const char* filename);
    HANDLE_TEXTURE  textureFind(uint32_t name);
    
    HANDLE_RTARGET  rtargetCreate(int width, int height, bool zbuff = false);
    void            rtargetFree(HANDLE_RTARGET target);
    HANDLE_TEXTURE  rtargetGetTexture(HANDLE_RTARGET target);

	HANDLE_SHADER	shaderLoad(const char* filename, bool activate = false);
	void			shaderFree(HANDLE_SHADER shader);
	void			shaderActive(HANDLE_SHADER shader);
	void			shaderDeActive();
    void            shaderPush();
    void            shaderPop();
    HANDLE_SHADER   shaderGetCurrent() { return (HANDLE_SHADER)mpCurShader; }
	// TODO: pass parameters to shader
    
    void            renderLine(float x1, float y1, float x2, float y2, uint32_t color, float z);
    void            renderRect(float x, float y, float w, float h, uint32_t color, float z);
    void            renderBox(float x, float y, float w, float h, uint32_t color, float z);
    void            renderQuad(const Quad *quad);
    void            renderTriple(const Triple *triple);
    void            renderVertexList(const VertexList* list);
    
    static void		setVertexColor(Vertex *v, int rgba);
    
    double          calculateFPS();
    const double    getFPS() const { return mFPS; }
    
    void			setDebug(bool debug = true);
    const bool		isDebug() const { return mIsDebug; }
    
private:
    void			_renderBatch(bool bEndScene = false);
    void			_setupPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void			_setupOrtho(int width, int height, bool target=false);
    void            _setupProjection(int w, int h, bool flip);
    HG_ERROR		_initOpenGL();
    HG_ERROR		_initBuffer();
    
    TextureList*    _findLoadedTexture(const char* filename);
    
    void            _bindTexture(HANDLE_TEXTURE texture);
    void            _setBlendMode(int mode);
    
    
    static inline GLuint _nextPow2(int raw);
    static inline bool	 _isPow2(int raw);
    
    int         mWidth;
    int         mHeight;
    
    // graphics
    Vertex*     mpVB;
    GLushort*   mpIB;
    Vertex*     mpVertices;
    GLuint      mIndexBufferObject;
    
    TextureList*        mpTextures;
    HANDLE_TEXTURE      mpCurTexture;
    
    RTargetList*        mpTargets;
    RTargetList*        mpCurTarget;

    typedef std::stack<ShaderList *> ShaderStack;
	ShaderList*			mpShaders;
	ShaderList*			mpCurShader;
    ShaderStack         mShaderStack;
    
    int                 mPrimitive;
    int                 mCurPrimitiveType;
    int                 mCurBlenMode;
    GLenum              mLastCustomPrimitive;
    const VertexList*   mCurVertexList;
    
    unsigned char*		mBuffer;
    uint32_t			mBufferSize;
    
    // FPS counter
    int                 mFrames;
    double              mFPS;
    double              mTime0;
    double              mTime1;
    
    bool				mIsDebug;
    int                 mDebugVertexUsed;
    
private:
    Render();
    ~Render();
    Render(const Render& other);
    Render& operator= (const Render& rhs);
};
    
}



#endif // HGRENDER_H_
