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

HGNAMESPACE_START
    
Render::Render()
	: mpVB(NULL)
	, mpIB(NULL)
	, mpVertices(NULL)
	, mIndexBufferObject(0)
    , mCurTexture(0)
    , mCurRTarget(NULL)
	, mCurShader(0)
	, mCurBlenMode(BLEND_DEFAULT)
	, mLastCustomPrimitive(0)
	, mPrimitive(0)
	, mBuffer(0)
	, mBufferSize(0)
    , mFrames(0)
    , mFPS(0.0)
    , mTime0(0.0)
    , mTime1(0.0f)
    , mWidth(0)
    , mHeight(0)
    , mIsFullScreen(false)
	, mDebugVertexUsed(0)
    , mDefaultFont(NULL)
{
    
}

Render::~Render()
{
    
}

HG_ERROR Render::initialize(int width, int height, bool fullscreen)
{
    HG_ERROR ret = HG_OK;
    
    BREAK_START;
    
    mWidth = width;
    mHeight = height;
    mIsFullScreen = fullscreen;
    
    ret = _initLib();
    if (ret != HG_OK)
        break;
    
    ret = _initBuffer();
    if (ret != HG_OK)
        break;
    
    ret = _initOpenGL();
    if (ret != HG_OK)
        break;
    
    
    mDefaultFont = new TextureFontContext;
    mDefaultFont->font = new FTTextureFont(HG_DEFAULT_FONT_FILE);
    setDefaultFontSize(HG_DEFAULT_FONT_SIZE);
    if (mDefaultFont->font == NULL || mDefaultFont->font->Error())
    {
        ret = HG_ERROR_BAD_INVOCATION;
        break;
    }
    
    mBufferSize = HG_MAX_IMAGE_SIZE * HG_MAX_IMAGE_SIZE * sizeof(uint32_t);
    mBuffer = new unsigned char[mBufferSize];
    if (mBuffer == NULL)
    {
        ret = HG_ERROR_INSUFFICIENT_MEMORY;
        break;
    }
    memset(mBuffer, 0, mBufferSize);
    
    mTime0 = glfwGetTime();
    mFrames = 0;
    
    BREAK_END;
    
    return ret;
}

/**
 *	@brief	deinitialize render
 */
void Render::deInitialize()
{
    textureFreeAll();
    rtargetFreeAll();
    shaderFreeAll();
    
    if (mDefaultFont->font != NULL)
    {
        delete mDefaultFont->font;
        mDefaultFont->font = NULL;
        delete mDefaultFont;
        mDefaultFont = NULL;
    }
    
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
    
    if (!mCurRTarget)
    {
        scr_width = mWidth;
        scr_height = mHeight;
    }
    else
    {
        scr_width = textureGetWidth(mCurRTarget->tex);
        scr_height = textureGetHeight(mCurRTarget->tex);
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
        if (!mCurRTarget)
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClearDepth(1.0f);
}

HG_ERROR Render::beginScene(GLuint tar)
{
    if (mpVertices)
    {
        return HG_ERROR_BAD_INVOCATION;
    }
    
    rtarget_t *target = rtargetFind(tar);
    
    if (target != mCurRTarget)
    {
        if (target)
        {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->framebuffer);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, target->renderbuffer);

            _setupOrtho(target->width, target->height, true);
        }
        else // !taget
        {
            glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
            _setupOrtho(mWidth, mHeight);
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        mCurRTarget = target;
    }
    
    mpVertices = mpVB;
    
    return HG_OK;
}

void Render::endScene()
{
    _renderBatch(true);

	if (!mCurRTarget)
	{
		glfwSwapBuffers();
	}
    else
    {
        glFinish();
    }
}

Vertex* Render::startBatch(int primType, GLuint tex, int blend, int *maxPrim)
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

GLuint Render::textureCreate( int width, int height )
{
    GLuint ret = 0;
    
    BREAK_START;
    
        GLuint name;
        glGenTextures(1, &name);
        if (name == GL_INVALID_VALUE)
            break;
        
        texture_t *texItem  = new texture_t;
        texItem->tex        = name;
        texItem->width      = _nextPow2(width);
        texItem->height     = _nextPow2(height);
        texItem->rawWidth   = width;
        texItem->rawHeight  = height;
        
        texItem->bpp            = 32;
        texItem->internalFormat = GL_RGBA;
        texItem->format         = GL_RGBA;
        texItem->type           = GL_UNSIGNED_INT_8_8_8_8_REV;
        texItem->lockInfo.data  = NULL;
        
        mTextureMap.insert(TextureMap::value_type(name, texItem));
    
        ret = name;
        
    BREAK_END;
    
    return ret;
}

void Render::textureFree(GLuint texture, bool force)
{
    TextureMap::iterator iter = mTextureMap.find(texture);
    if (iter == mTextureMap.end())
        return;
    
    texture_t *tex = iter->second;
    if (tex == NULL)
        return;
    
    if (mCurTexture == texture)
        mCurTexture = 0;
    
    tex->refCount--;
    if (tex->refCount <= 0 || force)
    {
        delete [] tex->lockInfo.data;
        glDeleteTextures(1, &tex->tex);
        delete tex;
        mTextureMap.erase(iter);
    }
}

int Render::textureGetWidth(GLuint texture)
{
    TextureMap::iterator iter = mTextureMap.find(texture);
    if (iter == mTextureMap.end())
        return 0;
    
    texture_t *tex = iter->second;
    if (tex == NULL)
        return 0;
    
    return tex->width;
}

int Render::textureGetHeight(GLuint texture)
{
    TextureMap::iterator iter = mTextureMap.find(texture);
    if (iter == mTextureMap.end())
        return 0;
    
    texture_t *tex = iter->second;
    if (tex == NULL)
        return 0;
    
    return tex->height;
}

char* Render::textureLock(GLuint texture, bool bReadOnly, int left, int top, int width, int height)
{
    TextureMap::iterator iter = mTextureMap.find(texture);
    if (iter == mTextureMap.end())
        return NULL;
    
    texture_t *tex = iter->second;
    if (tex == NULL)
        return NULL;
    
    GLint tmpTexture = tex->tex;
    int bytesPerPixel = tex->bpp >> 3;
    
    if (tex->lockInfo.data != NULL)
        tex->lockInfo.data = new char[tex->width * tex->height * bytesPerPixel];
    
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);
    
    // can't alloc memory
    if (tex->lockInfo.data == NULL)
    {
        glBindTexture(GL_TEXTURE_2D, tmpTexture);
        return NULL;
    }
    
    tex->lockInfo.readonly          = bReadOnly;
    tex->lockInfo.lockRect.top      = top;
    tex->lockInfo.lockRect.left     = left;
    tex->lockInfo.lockRect.right    = left+width;
    tex->lockInfo.lockRect.bottom   = top+height;
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexImage(GL_TEXTURE_2D, 0, tex->format, tex->type, tex->lockInfo.data);
    glBindTexture(GL_TEXTURE_2D, tmpTexture);
    
    char* ret = (char *)(tex->lockInfo.data + (top * tex->width + left) * bytesPerPixel);
    
    return ret;
}

void Render::textureUnlock(GLuint texture)
{
    TextureMap::iterator iter = mTextureMap.find(texture);
    if (iter == mTextureMap.end())
        return;
    
    texture_t *tex = iter->second;
    if (tex == NULL)
        return;
    
    if (!tex->lockInfo.readonly)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        glTexImage2D(GL_TEXTURE_2D, 0, tex->internalFormat, tex->width, tex->height, 0, tex->format, tex->type, tex->lockInfo.data);
    }
}

GLuint Render::textureLoad(const char* filename)
{
    GLuint retval = 0;
    
    unsigned lodepngError;
    unsigned imageWidth = 0;
    unsigned imageHeight = 0;
    
    unsigned char* pngData = NULL;
    
    texture_t *texItem;
    
    // check if the texture is already loaded
    texture_t *findResult = _findLoadedTexture(filename);
    if (findResult != NULL)
    {
        findResult->refCount++;
        return findResult->tex;
    }
    
    Data* fileData = Data::CreateFromFileContent(filename);
    
    BREAK_START;
    
        if (fileData == NULL)
            break;
        
        lodepngError = lodepng_decode32(&pngData, &imageWidth, &imageHeight, (const unsigned char*)fileData->getData(), fileData->getLength());
        if (lodepngError != 0)
            break;
        
        texture_t newItem;
        newItem.format          = GL_RGBA;
        newItem.internalFormat  = 4;
        newItem.type            = GL_UNSIGNED_BYTE;
        newItem.rawWidth        = imageWidth;
        newItem.rawHeight       = imageHeight;
        newItem.width           = imageWidth;
        newItem.height          = imageHeight;
        newItem.fileNameHash    = Hash(filename);
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
        newItem.width = newItem.rawWidth;//*/_nextPow2(newItem.rawWidth);
        newItem.height = newItem.rawHeight;//*/_nextPow2(newItem.rawHeight);
        
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
        
        texItem = new texture_t;
        texItem->tex            = name;
        texItem->format         = newItem.format;
        texItem->internalFormat = newItem.internalFormat;
        texItem->type           = newItem.type;
        texItem->rawWidth       = newItem.rawWidth;
        texItem->rawHeight      = newItem.rawHeight;
        texItem->height         = newItem.height;
        texItem->width          = newItem.width;
        texItem->fileNameHash   = newItem.fileNameHash;
        texItem->refCount       = newItem.refCount;
        texItem->bpp            = 32;
        texItem->lockInfo.data  = NULL;
        
        mTextureMap.insert(TextureMap::value_type(name, texItem));
    
        retval = name;
        
    BREAK_END;
    
    free(pngData);
    delete fileData;

	if (retval == 0)
    {
		HGLog("Can't Load Image:%s\n", filename);
    }
    
    return retval;
}
    
GLuint Render::textureLoad(uint32_t filenameHash)
{
    GLuint ret = 0;
    
    TextureMap::iterator iter = mTextureMap.begin();
    for (; iter != mTextureMap.end(); ++iter)
    {
        if (iter->second->fileNameHash == filenameHash)
        {
            ret = iter->first;
            break;
        }
    }
    
    return ret;
}
    
void Render::textureFreeAll()
{
    for (TextureMap::iterator iter = mTextureMap.begin(); iter != mTextureMap.end(); ++iter)
    {
        texture_t *tex = iter->second;
        delete [] tex->lockInfo.data;
        glDeleteTextures(1, &tex->tex);
        delete tex;
    }
    mTextureMap.clear();
}

// render target methods
GLuint Render::rtargetCreate(int width, int height)
{
    rtarget_t *target = new rtarget_t;
    if (target == NULL)
        return 0;
    
    memset(target, 0, sizeof(rtarget_t));
    
    GLenum status;
    
    // set up FBO with one texture attachment
    glGenFramebuffersEXT(1, &target->framebuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->framebuffer);
    
    // create texture
    glGenTextures(1, &target->tex);
    
    glBindTexture(GL_TEXTURE_2D, target->tex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // depth
    glGenRenderbuffersEXT(1, &target->renderbuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, target->renderbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, target->renderbuffer);
    
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, target->tex, 0);
    
    // check for result
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        glDeleteTextures(1, &target->tex);
        delete target;
        return 0;
    }
    
    target->width = width;
    target->height = height;
    
    mRTargetMap.insert(RTargetMap::value_type(target->framebuffer, target));
    
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);

    return target->framebuffer;
}

void Render::rtargetFree(GLuint target)
{
    RTargetMap::iterator iter = mRTargetMap.find(target);
    if (iter == mRTargetMap.end())
        return;
    
    rtarget_t *tar = iter->second;
    textureFree(tar->tex);
    glDeleteFramebuffers(1, &tar->framebuffer);
    glDeleteRenderbuffers(1, &tar->renderbuffer);
    
    delete tar;
    
    mRTargetMap.erase(iter);
}

GLuint Render::rtargetGetTexture(GLuint target)
{
    RTargetMap::iterator iter = mRTargetMap.find(target);
    if (iter == mRTargetMap.end())
        return 0;
    
    return iter->second->tex;
}

rtarget_t* Render::rtargetFind(GLuint target)
{
    RTargetMap::iterator iter = mRTargetMap.find(target);
    if (iter == mRTargetMap.end())
        return NULL;
    
    return iter->second;
}
    
void Render::rtargetFreeAll()
{
    for (RTargetMap::iterator iter = mRTargetMap.begin(); iter != mRTargetMap.end(); ++iter)
    {
        rtarget_t *tar = iter->second;
        textureFree(tar->tex);
        glDeleteFramebuffers(1, &tar->framebuffer);
        delete tar;
    }
    mTextureMap.clear();
}

GLuint Render::shaderLoad(const char* filename, bool activate)
{
	shader_t *newShader = NULL;
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

		newShader = new shader_t;
		if (newShader == NULL)
			break;
		
		memset(newShader, 0, sizeof(shader_t));

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
    
        // add shader
        mShaders.insert(ShaderMap::value_type(newShader->program, newShader));

		if (activate)
			shaderActive(newShader->program);

		memset(mBuffer, 0, sizeof(unsigned char) * HG_MAX_IMAGE_SIZE);
		glGetInfoLogARB(newShader->program, HG_MAX_IMAGE_SIZE, NULL, (GLchar *)mBuffer);
		HGLog("shaderLoad: %s\n", mBuffer);
		
    BREAK_END;
	
	delete vs;
	delete fs;

	return newShader->program;
}

void Render::shaderFree(GLuint shader)
{
    ShaderMap::iterator iter;
    
    BREAK_START;
    
    iter = mShaders.find(shader);
    
    if (iter == mShaders.end())
        break;
    
    shader_t* pShader = iter->second;
    
    glDetachShader(pShader->program, pShader->vertex);
    glDetachShader(pShader->program, pShader->fragment);
    glUseProgram(0);
    glDeleteProgram(pShader->program);
    
    delete pShader;
    mShaders.erase(iter);
    
    BREAK_END;
}

void Render::shaderActive(GLuint shader)
{
    ShaderMap::iterator iter;
    
    BREAK_START;
    
    iter = mShaders.find(shader);
    
    if (iter == mShaders.end())
        break;
    
    if (iter->second->program == mCurShader)
        break;
    
    _renderBatch();
    
    mCurShader = iter->second->program;
    glUseProgram(mCurShader);
    
    BREAK_END;
}

void Render::shaderDeActive()
{
    if (mCurShader == 0)
        return;
    
    _renderBatch();
	glUseProgram(0);
	mCurShader = 0;
}
    
void Render::shaderPush()
{
    if (mCurShader != 0)
        shaderDeActive();

    mShaderStack.push(mCurShader);
}

void Render::shaderPop()
{
    mCurShader = mShaderStack.top();
    mShaderStack.pop();
    
    if (mCurShader != 0)
        shaderActive(mCurShader);
}

void Render::shaderFreeAll()
{
    BREAK_START;
    
    if (mShaders.empty())
        break;
 
    ShaderMap::iterator iter = mShaders.begin();
    
    for (; iter != mShaders.end(); ++iter)
    {
        shader_t* pShader = iter->second;
        
        glDetachShader(pShader->program, pShader->vertex);
        glDetachShader(pShader->program, pShader->fragment);
        glUseProgram(0);
        glDeleteProgram(pShader->program);
        
        delete pShader;
    }
    
    mShaders.clear();
    
    BREAK_END;
}
    
// render stuff
void Render::renderLine(float x1, float y1, float x2, float y2, uint32_t color, float z)
{
    if (mpVertices == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_LINES || mPrimitive >= HG_VERTEX_BUFFER_SIZE / PRIMITIVE_LINES || mCurTexture || mCurBlenMode != BLEND_DEFAULT)
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
    
    vertex_set_color(&mpVertices[i], color);
    vertex_set_color(&mpVertices[i+1], color);

    mpVertices[i].u = mpVertices[i+1].u = mpVertices[i].v = mpVertices[i+1].v = 0;
    
    mPrimitive++;
    mDebugVertexUsed+=PRIMITIVE_LINES;
}
    
void Render::renderRect(float x, float y, float w, float h, uint32_t color, float z)
{
    Quad quad;
    quad_set_default(&quad);
    quad_set_color(&quad, color);
    
    quad.v[0].x = x;
    quad.v[0].y = y;
    quad.v[1].x = x + w;
    quad.v[1].y = y;
    quad.v[2].x = x + w;
    quad.v[2].y = y + h;
    quad.v[3].x = x;
    quad.v[3].y = y + h;
    
    renderQuad(&quad);
}

void Render::renderBox(float x, float y, float w, float h, uint32_t color, float z)
{
    renderLine(x, y, x+w, y, color, z);
    renderLine(x, y, x, y+h, color, z);
    renderLine(x+w, y, x+w, y+h, color, z);
    renderLine(x, y+h, x+w, y+h, color, z);
}
    
void Render::renderCircle(float cx, float cy, float r, uint32_t v, uint32_t color, float z, int fill)
{
    if (v == 0)
        return;
    
    int vcount = v + 2;
    
    VertexList vl;
    vl.blend = BLEND_DEFAULT;
    vl.tex = 0;
    vl.primitive = fill ? GL_TRIANGLE_FAN : GL_LINE_STRIP;
    vl.primitive_count = fill ? 3 : 1;
    
    Vertex *vertices = (Vertex *)alloca(sizeof(Vertex) * vcount);
    memset(vertices, 0, sizeof(Vertex) * vcount);
    
    vl.v = vertices;
    vl.vertex_count = vcount;
    
    float angleStep = (float)(2 * M_PI / (vcount - 2));
    
    for (int i = 1; i < vcount; i++)
    {
        vertices[i].u = 0.0f;
        vertices[i].v = 0.0f;
        vertices[i].x = cx + r * cosf(angleStep * (i-1));
        vertices[i].y = cy + r * sinf(angleStep * (i-1));
        vertex_set_color(&vertices[i], color);
    }
    
    if (fill)
    {
        vertices[0].u = 0.0f;
        vertices[0].v = 0.0f;
        vertices[0].x = cx;
        vertices[0].y = cy;
        vertex_set_color(&vertices[0], color);
        
        memcpy(&vertices[1], &vertices[vcount-1], sizeof(Vertex));
    }
    else
    {
        memcpy(&vertices[0], &vertices[vcount-1], sizeof(Vertex));
    }
    
    renderVertexList(&vl);
}

void Render::renderQuad(const Quad* quad)
{
    if (mpVertices == NULL)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_QUADS || mPrimitive >= HG_VERTEX_BUFFER_SIZE/PRIMITIVE_QUADS || mCurTexture != quad->tex || mCurBlenMode != quad->blend)
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
    
    if (mCurPrimitiveType != PRIMITIVE_TRIANGLES || mPrimitive >= HG_VERTEX_BUFFER_SIZE/PRIMITIVE_TRIANGLES || mCurTexture != triple->tex || mCurBlenMode != triple->blend)
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
    
    if (mCurPrimitiveType != PRIMITIVE_CUSTOM || mPrimitive >= HG_VERTEX_BUFFER_SIZE/list->vertex_count || mCurTexture != list->tex || mCurBlenMode != list->blend || mLastCustomPrimitive != list->primitive)
    {
        _renderBatch();
        
        mCurPrimitiveType = PRIMITIVE_CUSTOM;
        
        if (mCurBlenMode != list->blend)
            _setBlendMode(list->blend);
        
        _bindTexture(list->tex);
    }
    
    memcpy(&mpVertices[mPrimitive * list->vertex_count], list->v, sizeof(Vertex) * list->vertex_count);
    mPrimitive++;
    mDebugVertexUsed += list->vertex_count;
    mCurVertexList.vertex_count     = list->vertex_count;
    mCurVertexList.primitive_count  = list->primitive_count;
    mLastCustomPrimitive            = list->primitive;
}
    
void Render::renderText(float x, float y, float z, uint32_t color,const char* format, ...)
{    
    if (mDefaultFont->font == NULL)
        return;
    
    if (strlen(format) >= mBufferSize)
        return;
    
    if (mCurPrimitiveType != PRIMITIVE_TEXT)
    {
        // force end scene
        _renderBatch(true);
        mCurPrimitiveType = PRIMITIVE_TEXT;
    }
    
    memset(mBuffer, 0, mBufferSize);
    
    va_list ap;
    va_start(ap, format);
    vsprintf((char *)mBuffer, format, ap);
    va_end(ap);
    
    // actuall rendering
    glPushMatrix();
    glLoadIdentity();
    
    glTranslatef(x, y + mDefaultFont->size, z);
    glScalef(1.0f, -1.0f, 1.0f);
    
    glColor4f(GETR(color)/255.0f, GETG(color)/255.0f, GETB(color)/255.0f, GETA(color)/255.0f);

    mDefaultFont->font->Render((char *)mBuffer);
    
    glPopMatrix();
}
    
void Render::setDefaultFontSize(uint32_t size)
{
    mDefaultFont->size = size;
    if (mDefaultFont->font != NULL)
    {
        mDefaultFont->font->FaceSize(size);
    }
}

void Render::_renderBatch(bool bEndScene)
{
    if (!mpVertices)
        return;
    
    if (mPrimitive == 0)
        return;
    
    switch (mCurPrimitiveType)
    {
        case PRIMITIVE_CUSTOM:
            glDrawArrays(mLastCustomPrimitive, 0, mCurVertexList.vertex_count);
            //mCurVertexList = NULL;
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
        glOrtho(0, width, height, 0, -10.0f, 10.0f);
    else
        glOrtho(0, width, 0, height, -10.0f, 10.0f);
}

HG_ERROR Render::_initLib()
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
    uint32_t screenMode = (mIsFullScreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;
    if (!glfwOpenWindow(mWidth, mHeight, 0, 0, 0, 0, 32, 0, screenMode))
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
 *	@brief	init openGL and other related lib
 *
 *	@return	result
 */
HG_ERROR Render::_initOpenGL()
{
    _setupOrtho(mWidth, mHeight);
    
    glAlphaFunc(GL_GREATER, 0.1f);   // why we need this ?
    glEnable(GL_ALPHA_TEST);

	glEnable(GL_DEPTH_TEST);         // answer http://www.sjbaker.org/steve/omniv/alpha_sorting.html
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
    
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
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // set data alignment and byte order
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);
    
    
    _setBlendMode(BLEND_DEFAULT);
    
    return HG_OK;
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
        
        // initialize openGL buffer
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
    

void Render::_bindTexture(GLuint texture )
{
    if (texture != mCurTexture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        mCurTexture = texture;
    }
}
    
texture_t* Render::_findLoadedTexture(const char *filename)
{
    texture_t *ret = NULL;
    uint32_t hashname = Hash(filename);
    
    TextureMap::iterator iter = mTextureMap.begin();
    for (; iter != mTextureMap.end(); ++iter)
    {
        if (iter->second->fileNameHash == hashname)
        {
            ret = iter->second;
            break;
        }
    }
    
    return ret;
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
    
HGNAMESPACE_END