/**
 *  @file    HGRender.cpp
 *  @brief   Render implementation
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

#include "HGSystem.h"
#include "HGRender.h"
#include "HGData.h"
#include "HGHash.h"

#define HG_VERTEX_BUFFER_SIZE 80000
#define HG_MAX_IMAGE_SIZE 1024

//#define NO_BUFFER
// TODO generate mid map

namespace HG
{
    
Render::Render()
	: mpVB(NULL)
	, mpIB(NULL)
	, mpVertices(NULL)
	, mIndexBufferObject(0)
	, mpTextures(NULL)
	, mpCurTexture(0)
	, mpTargets(NULL)
	, mpCurTarget(NULL)
	, mpShaders(NULL)
	, mpCurShader(NULL)
	, mCurBlenMode(BLEND_DEFAULT)
	, mCurVertexList(NULL)
	, mLastCustomPrimitive(0)
	, mPrimitive(0)
	, mBuffer(0)
	, mBufferSize(0)
    , mFrames(0)
    , mFPS(0.0)
    , mTime0(0.0)
    , mTime1(0.0f)
	, mDebugVertexUsed(0)
	, mIsDebug(false)
{
    
}

Render::~Render()
{
    
}

HG_ERROR Render::initialize(int width, int height)
{
    HG_ERROR ret = HG_OK;
    
    mWidth = width;
    mHeight = height;
    
    ret = _initOpenGL();
    if (ret != HG_OK)
        return ret;
    
    ret = _initBuffer();
    if (ret != HG_OK)
        return ret;
    
    _setupOrtho(mWidth, mHeight);
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST); // TODO: learn! must disable to enable alpha blend
	glDepthFunc(GL_LESS);
	glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // set data alignment and byte order
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glLineWidth(2.0f);
    
    _setBlendMode(BLEND_DEFAULT);
    
    mBufferSize = HG_MAX_IMAGE_SIZE * HG_MAX_IMAGE_SIZE * sizeof(uint32_t);
    mBuffer = new unsigned char[mBufferSize];
    memset(mBuffer, 0, mBufferSize);
    
    mTime0 = glfwGetTime();
    mFrames = 0;
    
    return ret;
}

/**
 *	@brief	deinitialize render
 */
void Render::deInitialize()
{
    while (mpTextures)
    {
        textureFree(mpTextures->tex);
    }
    mpTextures = NULL;
    
    while (mpTargets)
    {
        rtargetFree((HANDLE_RTARGET)mpTargets);
    }
    mpTargets = NULL;

	while (mpShaders)
	{
		shaderFree((HANDLE_SHADER)mpShaders);
	}
	mpShaders = NULL;
	
    
    if (mpVB != NULL)
    {
        delete[] mpVB;
        mpVB = NULL;
    }
    
    if (mpIB != NULL)
    {
        delete[] mpIB;
    }
    
    if (mIndexBufferObject != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mIndexBufferObject);
        mIndexBufferObject = 0;
    }
    
    delete [] mBuffer;
    
    glfwTerminate();
}

/**
 *	@brief	set clear color in opengl style
 *
 *	@param 	r 	red
 *	@param 	g 	green
 *	@param 	b 	blue
 *	@param 	a 	alpha
 */
void Render::setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glClearColor(r, g, b, a);
}

/**
 *	@brief	set clear color in hardware way
 *
 *	@param 	color 	0xAARRGGBB
 */
void Render::setClearColor(GLuint color)
{
    GLfloat a = GETA(color) / 255.0f;
    GLfloat r = GETR(color) / 255.0f;
    GLfloat g = GETG(color) / 255.0f;
    GLfloat b = GETB(color) / 255.0f;
    
    glClearColor(r, g, b, a);
}
    
/**
 *	@brief	set clipping
 *
 *	@param 	x 	x
 *	@param 	y 	y
 *	@param 	w 	width
 *	@param 	h 	height
 */
void Render::setClipping(int x, int y, int w, int h)

{
    int scr_width, scr_height;
    int vpX, vpY, vpWidth, vpHeight;
    
    if (!mpCurTarget)
    {
        scr_width = mWidth;
        scr_height = mHeight;
    }
    else
    {
        scr_width = textureGetWidth((HANDLE_TEXTURE)mpCurTarget->texture);
        scr_height = textureGetHeight((HANDLE_TEXTURE)mpCurTarget->texture);
    }
    
    if (!w)
    {
        vpX = 0; vpY = 0;
        vpWidth = scr_width;
        vpHeight = scr_height;
    }
    else
    {
        if (x<0) { w+=x; x=0; }
        
        if (y<0) { h+=y; y=0; }
        
        if (x+w > scr_width)
            w = scr_width-x;
        
        if (y+h > scr_height)
            h = scr_height-y;
        
        vpX = x;
        vpY = y;
        vpWidth = w;
        vpHeight = h;
    }
    
    _renderBatch();
    
    if (w)
    {
        glEnable(GL_SCISSOR_TEST);
        if (!mpCurTarget)
            glScissor(vpX, scr_height-(vpY+vpHeight), vpWidth, vpHeight);
        else
            glScissor(vpX, vpY, vpWidth, vpHeight);
    }
    else
        glDisable(GL_SCISSOR_TEST);
}

/**
 *	@brief	clear
 */
void Render::clear()
{
    bool cleanZ = false;
    if (mpCurTarget)
    {
        if (mpCurTarget->bDepth)
            cleanZ = true;
    }
    
    if (cleanZ)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
    }
    else
        glClear(GL_COLOR_BUFFER_BIT);
}

HG_ERROR Render::beginScene(HANDLE_RTARGET tar)
{
    if (mpVertices)
    {
        return HG_ERROR_BAD_INVOCATION;
    }
    
    RTargetList * target = (RTargetList *)tar;
    
    if (target != mpCurTarget)
    {
        if (target)
        {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->framebuffer);
        }
        else
        {
            
        }
        
        if (target)
        {
            if (target->bDepth)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDepthRange(-1.0, 10.0);
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
            _setupOrtho(target->width, target->height, true);
        }
        else // taget
        {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glDisable(GL_DEPTH_TEST);
            _setupOrtho(mWidth, mHeight);
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        mpCurTarget = target;
    }
    
    mpVertices = mpVB;
    
    return HG_OK;
}

void Render::endScene()
{
    _renderBatch(true);

	if (!mpCurTarget)
	{
		glfwSwapBuffers();
	}
    else
    {
        glFinish();
    }
}

Vertex* Render::startBatch(int primType, HANDLE_TEXTURE tex, int blend, int *maxPrim)
{
    if (mpVertices)
    {
        _renderBatch();
        
        mCurPrimitiveType = primType;
        
        if (mCurBlenMode != blend)
            _setBlendMode(blend);
        
        
        _bindTexture(tex);
        
        *maxPrim = HG_VERTEX_BUFFER_SIZE / primType;
        
        return mpVertices;
    }
    else
    {
        return NULL;
    }
}

void Render::finishBatch(int nprim)
{
    mPrimitive = nprim;
}

// texture methods

HANDLE_TEXTURE Render::textureCreate( int width, int height )
{
    HANDLE_TEXTURE ret = 0;
    
    BREAK_START;
    
        GLuint name;
        glGenTextures(1, &name);
        if (name == GL_INVALID_VALUE)
            break;
        
        TextureList *texItem = new TextureList;
        texItem->tex        = (HANDLE_TEXTURE)name;
        texItem->width      = _nextPow2(width);
        texItem->height     = _nextPow2(height);
        texItem->rawWidth   = width;
        texItem->rawHeight  = height;
        
        texItem->bpp            = 32;
        texItem->internalFormat = GL_RGBA;
        texItem->format         = GL_RGBA;
        texItem->type           = GL_UNSIGNED_INT_8_8_8_8_REV;
        texItem->lockInfo.data  = NULL;
        
        texItem->next = mpTextures;
        mpTextures = texItem;
        
        ret = name;
        
    BREAK_END;
    
    return (HANDLE_TEXTURE)ret;
}

void Render::textureFree( HANDLE_TEXTURE texture, bool force)
{
    TextureList* texItem = mpTextures;
    TextureList* texPrev = NULL;
    bool shouldDelete = false;
    
    while (texItem)
    {
        if (texItem->tex == texture)
        {
            texItem->refCount--;
            if (texItem->refCount > 0 && !force)
            {
                // texture still in use
                shouldDelete = false;
                break;
            }
            else
            {
                if (texPrev)
                    texPrev->next = texItem->next;
                else
                    mpTextures = texItem->next;
                
                shouldDelete = true;
                delete [] texItem->lockInfo.data;
                glDeleteTextures(1, (GLuint *)&texItem->tex);
                delete texItem;
                break;
            }
        }
        
        texPrev = texItem;
        texItem = texItem->next;
    }
}

int Render::textureGetWidth( HANDLE_TEXTURE texture )
{
    TextureList* texItem = mpTextures;
    
    while (texItem)
    {
        if (texItem->tex == texture)
        {
            return texItem->width;
        }
        
        texItem = texItem->next;
    }
    
    return 0;
}

int Render::textureGetHeight( HANDLE_TEXTURE texture )
{
    TextureList* texItem = mpTextures;
    
    while (texItem)
    {
        if (texItem->tex == texture)
        {
            return texItem->height;
        }
        
        texItem = texItem->next;
    }
    
    return 0;
}

unsigned char* Render::textureLock( HANDLE_TEXTURE tex, bool bReadOnly, int left, int top, int width, int height)
{
    TextureList *texItem = mpTextures;

    while (texItem)
    {
        if (texItem->tex == tex)
            break;
        else
            texItem = texItem->next;
    }
    
    if (texItem == NULL || texItem->tex == 0)
        return NULL;
    
    size_t texture = 0;
    texture = texItem->tex;
    
    GLint tmpTexture;
    int bytesPerPixel = texItem->bpp >> 3;
    
    if (texItem->lockInfo.data != NULL)
        texItem->lockInfo.data = new unsigned char[texItem->width * texItem->height * bytesPerPixel];
    
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);
    
    // can't alloc memory
    if (texItem->lockInfo.data == NULL)
    {
        glBindTexture(GL_TEXTURE_2D, tmpTexture);
        return NULL;
    }
    
    texItem->lockInfo.readonly          = bReadOnly;
    texItem->lockInfo.lockRect.top      = top;
    texItem->lockInfo.lockRect.left     = left;
    texItem->lockInfo.lockRect.right    = left+width;
    texItem->lockInfo.lockRect.bottom   = top+height;
    
    glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
    glGetTexImage(GL_TEXTURE_2D, 0, texItem->format, texItem->type, texItem->lockInfo.data);
    glBindTexture(GL_TEXTURE_2D, tmpTexture);
    
    unsigned char* ret = (unsigned char *)(texItem->lockInfo.data + (top * texItem->width + left) * bytesPerPixel);
    
    return ret;
}

void Render::textureUnlock( HANDLE_TEXTURE texture )
{
    TextureList *texItem = mpTextures;
    
    while (texItem)
    {
        if (texItem->tex == texture)
            break;
        else
            texItem = texItem->next;
    }
    
    if (texItem == NULL || texItem->tex == 0)
        return;
    
    if (!texItem->lockInfo.readonly)
    {
        glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        glTexImage2D(GL_TEXTURE_2D, 0, texItem->internalFormat, texItem->width, texItem->height, 0, texItem->format, texItem->type, texItem->lockInfo.data);
    }
}

HANDLE_TEXTURE Render::textureLoad( const char* filename )
{
    HANDLE_TEXTURE retval = 0;
    
    unsigned lodepngError;
    unsigned imageWidth = 0;
    unsigned imageHeight = 0;
    
    unsigned char* pngData = NULL;
    
    TextureList* texItem;
    
    // check if the texture is already loaded
    TextureList* findResult = _findLoadedTexture(filename);
    if (findResult != NULL)
    {
        findResult->refCount++;
        return (HANDLE_TEXTURE)findResult->tex;
    }
    
    
    Data* fileData = Data::CreateFromFileContent(filename);
    
    BREAK_START;
    
        if (fileData == NULL)
            break;
        
        lodepngError = lodepng_decode32(&pngData, &imageWidth, &imageHeight, (const unsigned char*)fileData->getData(), fileData->getLength());
        if (lodepngError != 0)
            break;
        
        TextureList newItem;
        newItem.format          = GL_RGBA;
        newItem.internalFormat  = 4;
        newItem.type            = GL_UNSIGNED_BYTE;
        newItem.rawWidth        = imageWidth;
        newItem.rawHeight       = imageHeight;
        newItem.width           = imageWidth;
        newItem.height          = imageHeight;
        newItem.hashName        = Hash(filename);
        newItem.refCount        = 1;
        
        GLuint name;
        glGenTextures(1, &name);
        if (name == GL_INVALID_VALUE)
            break;
        
        glBindTexture(GL_TEXTURE_2D, name);
        // Texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        // pixel data
        newItem.width = newItem.rawWidth;//_nextPow2(newItem.rawWidth);
        newItem.height = newItem.rawHeight;//_nextPow2(newItem.rawHeight);
        
        // manage memory blocks
        if (newItem.width <= HG_MAX_IMAGE_SIZE && newItem.height <= HG_MAX_IMAGE_SIZE)
        {
            if (0)//(newItem.width != newItem.rawWidth || newItem.height != newItem.rawHeight)
            {
                //unsigned char* pbuff = new unsigned char[newItem.width*newItem.height*sizeof(uint32_t)];
                memcpy(mBuffer, pngData, imageWidth * imageHeight * sizeof(uint32_t));
                
                glTexImage2D(GL_TEXTURE_2D, 0, newItem.internalFormat, newItem.width, newItem.height, 0, newItem.format, newItem.type, NULL);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, newItem.rawHeight, newItem.rawHeight, newItem.format, newItem.type, mBuffer);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, newItem.internalFormat, newItem.width, newItem.height, 0, newItem.format, newItem.type, pngData);
            }
        }
        else
        {
            break;
        }
        
        texItem = new TextureList;
        texItem->tex            = (HANDLE_TEXTURE)name;
        texItem->format         = newItem.format;
        texItem->internalFormat = newItem.internalFormat;
        texItem->type           = newItem.type;
        texItem->rawWidth       = newItem.rawWidth;
        texItem->rawHeight      = newItem.rawHeight;
        texItem->height         = newItem.height;
        texItem->width          = newItem.width;
        texItem->hashName       = newItem.hashName;
        texItem->refCount       = newItem.refCount;
        texItem->bpp            = 32;
        texItem->lockInfo.data  = NULL;
        
        texItem->next = mpTextures;
        mpTextures = texItem;
        
        retval = (HANDLE_TEXTURE)name;
        
    BREAK_END;
    
    free(pngData);
    delete fileData;

	if (retval == 0)
    {
		HGLog("Can't Load Image:%s\n", filename);
    }
    
    return retval;
}
    
HANDLE_TEXTURE Render::textureFind(uint32_t name)
{
    HANDLE_TEXTURE retval = 0;
    
    TextureList *texItem = mpTextures;
    TextureList *texPrev = NULL;
    
    while (texItem)
    {
        if (texItem->hashName == name)
        {
            retval = texItem->tex;
            break;
        }
        
        texPrev = texItem;
        texItem = texItem->next;
    }
    
    return retval;
}

// render target methods
HANDLE_RTARGET Render::rtargetCreate(int width, int height, bool zbuff)
{
    RTargetList *pTarget = NULL;
    pTarget = new RTargetList;
    
    if (pTarget == NULL) {
        return 0;
    }
    
    memset(pTarget, 0, sizeof(RTargetList));
    pTarget->bDepth = zbuff;
    
    GLenum status;
    glGenFramebuffersEXT(1, &pTarget->framebuffer);
    // set up FBO with one texture attachment
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pTarget->framebuffer);
    
    pTarget->texture = (GLuint)textureCreate(width, height);
    
    glBindTexture(GL_TEXTURE_2D, pTarget->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pTarget->texture, 0);
    
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        glDeleteTextures(1, &pTarget->texture);
        delete pTarget;
        return 0;
    }
    
    pTarget->width = width;
    pTarget->height = height;
    
    pTarget->next = mpTargets;
    mpTargets = pTarget;
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    return (HANDLE_RTARGET)pTarget;
}

void Render::rtargetFree(HANDLE_RTARGET target)
{
    RTargetList *pTarget = mpTargets;
    RTargetList *pPrevTarget = NULL;
    
    while (pTarget)
    {
        if ((RTargetList *)target == pTarget)
        {
            if (pPrevTarget)
                pPrevTarget->next = pTarget->next;
            else
                mpTargets = pTarget->next;
            
            if (pTarget->framebuffer)
            {
                textureFree(pTarget->texture);
                glDeleteFramebuffersEXT(1, &pTarget->framebuffer);
            }
            
            delete pTarget;
            return;
        }
        
        pPrevTarget = pTarget;
        pTarget = pTarget->next;
    }
}

HANDLE_TEXTURE Render::rtargetGetTexture(HANDLE_RTARGET target)
{
    RTargetList *targ = (RTargetList *)target;
    if (target)
        return targ->texture;
    else
        return 0;
}

HANDLE_SHADER Render::shaderLoad(const char* filename, bool activate)
{
	ShaderList *newShader = NULL;
	Data *vs = NULL;
	Data *fs = NULL;

	BREAK_START;

		if (filename == NULL)
			break;

		std::string vertexFile = std::string(filename) + ".vert";
		std::string fragmentFile = std::string(filename) + ".frag";

		vs = Data::CreateFromFileContent(vertexFile.c_str());
		if (vs == NULL)
			break;
		
		fs = Data::CreateFromFileContent(fragmentFile.c_str());
		if (fs == NULL)
			break;

		newShader = new ShaderList;
		if (newShader == NULL)
			break;
		
		memset(newShader, 0, sizeof(ShaderList));

		newShader->vertex = glCreateShader(GL_VERTEX_SHADER);
		newShader->fragment = glCreateShader(GL_FRAGMENT_SHADER);

		const char *vv = (const char *)vs->getData();
		const char *ff = (const char *)fs->getData();
        
        glShaderSource(newShader->vertex, 1, &vv, NULL);
		glShaderSource(newShader->fragment, 1, &ff, NULL);

		glCompileShader(newShader->vertex);
		glCompileShader(newShader->fragment);

		newShader->program = glCreateProgram();

		// can't create program
		if (newShader->program == 0)
		{
			delete newShader;
			newShader = NULL;
			break;
		}

		glAttachShader(newShader->program, newShader->vertex);
		glAttachShader(newShader->program, newShader->fragment);

		glLinkProgram(newShader->program);

		if (activate)
			shaderActive((HANDLE_SHADER)newShader);

		newShader->next = mpShaders;
		mpShaders = newShader;

		memset(mBuffer, 0, sizeof(HG_MAX_IMAGE_SIZE));
		glGetInfoLogARB(newShader->program, HG_MAX_IMAGE_SIZE, NULL, (GLchar *)mBuffer);
		HGLog("shaderLoad: %s\n", mBuffer);
		
    BREAK_END;
	
	delete vs;
	delete fs;

	return (HANDLE_SHADER)newShader;
}

void Render::shaderFree(HANDLE_SHADER shader)
{
	ShaderList *pShader = mpShaders;
	ShaderList *pPrevShader = NULL;

	if (shader == 0)
		return;

	while (pShader)
	{
		if ((ShaderList *)shader == pShader)
		{
			if (pPrevShader)
				pPrevShader->next = pShader->next;
			else
				mpShaders = pShader->next;

			glDetachShader(pShader->program, pShader->vertex);
			glDetachShader(pShader->program, pShader->fragment);
			glUseProgram(0);
			glDeleteProgram(pShader->program);

			delete pShader;
			return;
		}

		pPrevShader = pShader;
		pShader = pShader->next;
	}
}

void Render::shaderActive(HANDLE_SHADER shader)
{
	ShaderList *pShader = (ShaderList *)shader;
	if (pShader == NULL)
		return;
    
    if (pShader == mpCurShader)
        return;
	
    _renderBatch();
	glUseProgram(pShader->program);
	mpCurShader = pShader;
}

void Render::shaderDeActive()
{
    if (mpCurShader == NULL)
        return;
    
    _renderBatch();
	glUseProgram(0);
	mpCurShader = NULL;
}
    
void Render::shaderPush()
{
    ShaderList *pShader = (ShaderList *)mpCurShader;
    
    if (pShader != NULL)
        shaderDeActive();

    mShaderStack.push(mpCurShader);
}

void Render::shaderPop()
{
    mpCurShader = mShaderStack.top();
    mShaderStack.pop();
    
    ShaderList *pShader = (ShaderList *)mpCurShader;
    if (pShader != NULL)
        shaderActive((HANDLE_SHADER)pShader);
}

// render stuff
void Render::renderLine(float x1, float y1, float x2, float y2, uint32_t color, float z)
{
    if (mpVertices == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_LINES || mPrimitive >= HG_VERTEX_BUFFER_SIZE / PRIMITIVE_LINES || mpCurTexture || mCurBlenMode != BLEND_DEFAULT)
    {
        _renderBatch();
        
        mCurPrimitiveType = PRIMITIVE_LINES;
        
        if (mCurBlenMode != BLEND_DEFAULT)
            _setBlendMode(BLEND_DEFAULT);
        
        _bindTexture(0);
    }
    
    int i = mPrimitive * PRIMITIVE_LINES;
    mpVertices[i].x = x1; mpVertices[i+1].x = x2;
    mpVertices[i].y = y1; mpVertices[i+1].y = y2;
    mpVertices[i].z = mpVertices[i+1].z = z;
    mpVertices[i].u = 0.0f; mpVertices[i].v = 0.0f;
    mpVertices[i+1].u = 0.0f; mpVertices[i+1].v = 0.0f;
    setVertexColor(&mpVertices[i], color);
    setVertexColor(&mpVertices[i+1], color);
    mpVertices[i].u = mpVertices[i+1].u = mpVertices[i].v = mpVertices[i+1].v = 0;
    
    mPrimitive++;
    mDebugVertexUsed+=PRIMITIVE_LINES;
}
    
void Render::renderRect(float x, float y, float w, float h, uint32_t color, float z)
{
    Quad quad;
    memset(&quad, 0, sizeof(Quad));
    quad.v[0].x = x;
    quad.v[0].y = y;
    quad.v[1].x = x + w;
    quad.v[1].y = y;
    quad.v[2].x = x + w;
    quad.v[2].y = y + h;
    quad.v[3].x = x;
    quad.v[3].y = y + h;
    setVertexColor(&quad.v[0], color);
    setVertexColor(&quad.v[1], color);
    setVertexColor(&quad.v[2], color);
    setVertexColor(&quad.v[3], color);
    quad.v[0].u = 0;
    quad.v[0].v = 0;
    quad.v[1].u = 0;
    quad.v[1].v = 0;
    quad.v[2].u = 0;
    quad.v[2].v = 0;
    quad.v[3].u = 0;
    quad.v[3].v = 0;
    quad.blend = BLEND_DEFAULT;
    quad.tex = 0;
    
    renderQuad(&quad);
}

void Render::renderBox(float x, float y, float w, float h, uint32_t color, float z)
{
    renderLine(x, y, x+w, y, color, z);
    renderLine(x, y, x, y+h, color, z);
    renderLine(x+w, y, x+w, y+h, color, z);
    renderLine(x, y+h, x+w, y+h, color, z);
}

void Render::renderQuad(const Quad* quad)
{
    if (mpVertices == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_QUADS || mPrimitive >= HG_VERTEX_BUFFER_SIZE/PRIMITIVE_QUADS || mpCurTexture != quad->tex || mCurBlenMode != quad->blend)
    {
        _renderBatch();
        
        mCurPrimitiveType = PRIMITIVE_QUADS;
        
        if (mCurBlenMode != quad->blend)
            _setBlendMode(quad->blend);
        
        _bindTexture(quad->tex);
    }
    
    memcpy(&mpVertices[mPrimitive * PRIMITIVE_QUADS], quad->v, sizeof(Vertex) * PRIMITIVE_QUADS);
    mPrimitive++;
    mDebugVertexUsed+=PRIMITIVE_QUADS;
}

void Render::renderTriple(const Triple *triple)
{
    if (mpVertices == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_TRIANGLES || mPrimitive >= HG_VERTEX_BUFFER_SIZE/PRIMITIVE_TRIANGLES || mpCurTexture != triple->tex || mCurBlenMode != triple->blend)
    {
        _renderBatch();
        
        mCurPrimitiveType = PRIMITIVE_TRIANGLES;
        
        if (mCurBlenMode != triple->blend)
            _setBlendMode(triple->blend);
        
        _bindTexture(triple->tex);
    }
    
    memcpy(&mpVertices[mPrimitive * PRIMITIVE_TRIANGLES], triple->v, sizeof(Vertex) * PRIMITIVE_TRIANGLES);
    mPrimitive++;
    mDebugVertexUsed+=PRIMITIVE_TRIANGLES;
}

void Render::renderVertexList(const VertexList* list)
{
    if (mpVertices == NULL)
        return;
    
    if (list == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_CUSTOM || mPrimitive >= HG_VERTEX_BUFFER_SIZE/list->count || mpCurTexture != list->tex || mCurBlenMode != list->blend || mLastCustomPrimitive != list->primitive)
    {
        _renderBatch();
        
        mCurPrimitiveType = PRIMITIVE_CUSTOM;
        
        if (mCurBlenMode != list->blend)
            _setBlendMode(list->blend);
        
        _bindTexture(list->tex);
    }
    
    memcpy(&mpVertices[mPrimitive * list->count], list->v, sizeof(Vertex) * list->count);
    mPrimitive++;
    mDebugVertexUsed+=list->count;
    mCurVertexList = list;
    mLastCustomPrimitive = list->primitive;
}

void Render::_renderBatch(bool bEndScene)
{
    if (!mpVertices)
        return;
    
    if (mPrimitive == 0)
        return;
    
    if (mIsDebug)
    {
        glDrawArrays(GL_LINE_STRIP, 0, mDebugVertexUsed);
        mDebugVertexUsed = 0;
    }
    else
    {
        switch (mCurPrimitiveType)
        {
            case PRIMITIVE_CUSTOM:
                glDrawArrays(mCurVertexList->primitive, 0, mPrimitive*mCurVertexList->count);
                mCurVertexList = NULL;
                break;
                
            case PRIMITIVE_QUADS:
                glDrawElements(GL_TRIANGLES, mPrimitive * 6, GL_UNSIGNED_SHORT, mpIB);
                break;
                
            case PRIMITIVE_TRIANGLES:
                glDrawArrays(GL_TRIANGLES, 0, mPrimitive * 3);
                break;
                
            case PRIMITIVE_LINES:
                glDrawArrays(GL_LINES, 0, mPrimitive * 2);
                break;
                
            default:
                break;
        }
    }
    
    mPrimitive = 0;
    
    if (bEndScene)
    {
        mpVertices = NULL;
    }
    else
    {
        mpVertices = mpVB;
    }
}

/**
 *	@brief	setup perspective
 *
 *	@param 	fovy 	Specifies the field of view angle, in degrees, in the y direction.
 *	@param 	aspect 	Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
 *	@param 	zNear 	Specifies the distance from the viewer to the near clipping plane (always positive).
 *	@param 	zFar 	Specifies the distance from the viewer to the far clipping plane (always positive).
 */
void Render::_setupPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;
    
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}
    
/**
 *	@brief	setup projection
 *
 *	@param 	w 	width
 *	@param 	h 	height
 *	@param 	flip 	flip y axis
 */
void Render::_setupProjection(int w, int h, bool flip)

{
    glDisable(GL_SCISSOR_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (!flip)
        glScalef(2.0f/(float)w, 2.0f/(float)h, 1.0f);
    else
        glScalef(2.0f/(float)w, -2.0f/(float)h, 1.0f);
    
    glTranslatef(-((float)w/2.0f), -((float)h/2.0f), 0.0f);
    glViewport(0, 0, w, h);
}

/**
 *	@brief	set projection to ortho mode
 */
void Render::_setupOrtho(int width, int height, bool target)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (!target)
        glOrtho(0, width, height, 0, -1.0f, 10.0f);
    else
        glOrtho(0, width, 0, height, -1.0f, 10.0f);
}

/**
 *	@brief	init openGL and other related lib
 *
 *	@return	result
 */
HG_ERROR Render::_initOpenGL()
{
    HG_ERROR err = HG_OK;
    
    BREAK_START;

        if (!glfwInit())
        {
            err = HG_ERROR_GLFW;
            break;
        }
        
        glfwOpenWindowHint(GLFW_VERSION_MAJOR, 2);
        glfwOpenWindowHint(GLFW_VERSION_MINOR, 1);
        //glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
        
		glfwSwapInterval(1);
        // open window
        if (!glfwOpenWindow(mWidth, mHeight, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
        {
            glfwTerminate();
            err = HG_ERROR_GLFW;
        }
        
        GLenum glewErr = glewInit();
        
        if (glewErr != GLEW_OK)
        {
            err = HG_ERROR_GLEW;
            break;
        }
        
        HGLog("Version     : %s\n", glGetString(GL_VERSION));
        HGLog("Vendor      : %s\n", glGetString(GL_VENDOR));
        HGLog("Renderer    : %s\n", glGetString(GL_RENDERER));
        //HGLog("Extensions  : %s\n", glGetString(GL_EXTENSIONS));
        
        // sorry, no framebuffer
        if (!glewIsSupported("GL_EXT_framebuffer_object"))
        {
            err = HG_ERROR_GL_EXTENSIONS_ERROR;
            break;
        }
        
        // sorry, no shader
        if (!glewIsSupported("GL_ARB_vertex_shader") ||
            !glewIsSupported("GL_ARB_fragment_shader") ||
            !glewIsSupported("GL_ARB_shading_language_100") ||
            !glewIsSupported("GL_ARB_shader_objects") ||
            !glewIsSupported("GL_ARB_texture_non_power_of_two"))
        {
            err = HG_ERROR_GL_EXTENSIONS_ERROR;
            break;
        }
        
    BREAK_END;
    
    return err;
}

/**
 *	@brief	create and setup buffers
 *
 *	@return	OP result
 */
HG_ERROR Render::_initBuffer()
{
    HG_ERROR ret = HG_OK;
    
    BREAK_START;

        if (mpVB != NULL)
        {
            ret = HG_ERROR_BAD_INVOCATION;
            break;
        }
        
        mpVB = new Vertex[HG_VERTEX_BUFFER_SIZE];
        if (mpVB == NULL)
        {
            ret = HG_ERROR_INSUFFICIENT_MEMORY;
            break;
        }
        
        if (mpIB != NULL)
        {
            ret = HG_ERROR_BAD_INVOCATION;
            break;
        }
        
        mpIB = new GLushort[HG_VERTEX_BUFFER_SIZE*6/4];
        if (mpIB == NULL)
        {
            ret = HG_ERROR_INSUFFICIENT_MEMORY;
            break;
        }
        
        GLushort* pIndices = mpIB;
        int n = 0;
        for (int i = 0; i < HG_VERTEX_BUFFER_SIZE / 4; i++)
        {
            *pIndices++=n;
            *pIndices++=n+1;
            *pIndices++=n+2;
            *pIndices++=n+2;
            *pIndices++=n+3;
            *pIndices++=n;
            n+=4;
        }
        
        // initialize opengl buffer
        glGenBuffers(1, &mIndexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * ((HG_VERTEX_BUFFER_SIZE*6)/4), mpIB, GL_STATIC_DRAW);
        delete [] mpIB;
        mpIB = NULL;
        
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &mpVB[0].x);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &mpVB[0].color);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mpVB[0].u);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
    BREAK_END;
    
    return ret;
}
    

void Render::_bindTexture( HANDLE_TEXTURE texture )
{
    if (texture != mpCurTexture)
    {
        glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
        mpCurTexture = texture;
    }
}
    
TextureList* Render::_findLoadedTexture(const char *filename)
{
    TextureList* texItem = mpTextures;
    TextureList* texPrev = NULL;
    uint32_t hashname = Hash(filename);
    
    TextureList* retval = NULL;
    
    while (texItem)
    {
        if (texItem->hashName == hashname)
        {
            retval = texItem;
            break;
        }
        
        texPrev = texItem;
        texItem = texItem->next;
    }
    
    return retval;
}
    
void Render::_setBlendMode(int mode)
{
    if((mode & BLEND_ALPHABLEND) != (mCurBlenMode & BLEND_ALPHABLEND))
    {
        if(mode & BLEND_ALPHABLEND) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        else glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    
    if((mode & BLEND_ZWRITE) != (mCurBlenMode & BLEND_ZWRITE))
    {
        if(mode & BLEND_ZWRITE) glDepthMask(GL_TRUE);
        else glDepthMask(GL_FALSE);
    }
    
    if((mode & BLEND_COLORADD) != (mCurBlenMode & BLEND_COLORADD))
    {
        if(mode & BLEND_COLORADD) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    
    mCurBlenMode = mode;
}

bool Render::_isPow2( int raw )
{
    return ((raw & (raw - 1)) == 0);
}

GLuint Render::_nextPow2( int raw )
{
    raw--;
    for (int i = 1; i < (sizeof(GLuint) * 8); i *= 2)
        raw |= raw >> i;
    
    return raw + 1;
}

void Render::setVertexColor(Vertex* v, int rgba)
{
    if (v == NULL)
        return;
    
    v->color[0] = GETR(rgba);
    v->color[1] = GETG(rgba);
    v->color[2] = GETB(rgba);
    v->color[3] = GETA(rgba);
}

double Render::calculateFPS()
{
    double ret = 0.0;
    
    mTime1 = glfwGetTime();
    if ((mTime1 - mTime0) > 1.0 || mFrames == 0)
    {
        mFPS = (double)mFrames / (mTime1 - mTime0);
        mTime0 = mTime1;
        mFrames = 0;
        ret = mFPS;
    }
    
    mFrames++;
    
    return ret;
}
    
void Render::setDebug( bool debug /*= true*/ )
{
    if (debug)
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    else
    {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    mDebugVertexUsed = 0;
    mPrimitive = 0;
    mIsDebug = debug;
}
    
}