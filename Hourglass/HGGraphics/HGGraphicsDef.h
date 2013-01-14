/**
 *  @file    HGGraphicsDef.h
 *  @brief   Graphics definitions
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

#ifndef HGGRAPHICSDEF_H_
#define HGGRAPHICSDEF_H_

#include "HGSystem.h"

namespace HG
{

// Color operate
#define RGBA(r,g,b,a)	((uint32_t(r)<<24) + (uint32_t(g)<<16) + (uint32_t(b)<<8) + uint32_t(a))
#define GETR(col)		((col)>>24)
#define GETG(col)		(((col)>>16) & 0xFF)
#define GETB(col)		(((col)>>8) & 0xFF)
#define GETA(col)		((col) & 0xFF)
#define SETR(col,r)		(((col) & 0x00FFFFFF) + (uint32_t(r)<<24))
#define SETG(col,g)		(((col) & 0xFF00FFFF) + (uint32_t(g)<<16))
#define SETB(col,b)		(((col) & 0xFFFF00FF) + (uint32_t(b)<<8))
#define SETA(col,a)		(((col) & 0xFFFFFF00) + uint32_t(a))

   
/**
 * primitive type
 */
#define PRIMITIVE_TEXT          -1
#define PRIMITIVE_CUSTOM        0
#define PRIMITIVE_LINES         2
#define PRIMITIVE_TRIANGLES     3
#define PRIMITIVE_QUADS         4
    
// Blend mode
#define BLEND_COLORADD          1
#define BLEND_COLORMUL          0
#define BLEND_ALPHABLEND        2
#define BLEND_ALPHAADD          0
#define BLEND_ZWRITE            4
#define BLEND_NOZWRITE          0
    
#define BLEND_DEFAULT           (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z         (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)
    
typedef enum _blend_mode_t_
{
    
}BLEND_MODE;
    
/**
 *	@brief	vertex define
 */
typedef struct _vertex
{
    float   x, y, z;
    float   u, v;
    GLbyte  color[4];
}Vertex;
    
typedef struct _quad
{
    uint32_t    blend;
    GLuint      tex;
    Vertex      v[4];
}Quad;
    
typedef struct _triple
{
    uint32_t    blend;
    GLuint      tex;
    Vertex      v[3];
}Triple;
    
typedef struct _vertex_list
{
    uint32_t    blend;
    GLuint      tex;
    int32_t     count;
    GLenum      primitive;
    Vertex*     v;
}VertexList;
    
typedef struct _TRect
{
    int top;
    int left;
    int right;
    int bottom;
}TRect;

typedef struct _rtarget_t_
{
    int     width;
    int     height;
    GLuint  framebuffer;
    GLuint  tex;
    bool    bDepth;
}rtarget_t;

typedef struct _TextureLockInfo
{
    bool    readonly;
    TRect   lockRect;
    char*   data;
}TextureLockInfo;
    
typedef struct _texture_t_
{
    GLuint          tex;
    int             rawWidth;
    int             rawHeight;
    int             width;
    int             height;
    int             bpp;
    GLenum          internalFormat;
    GLenum          format;
    GLenum          type;
    TextureLockInfo lockInfo;
    uint32_t        fileNameHash;
    int32_t         refCount;
}texture_t;
    
typedef struct _shader_t_
{
    uint32_t    hashName;
    GLuint      vertex;
    GLuint      fragment;
    GLuint      program;
}shader_t;
    
typedef struct _PixmapFontContext
{
    FTPixmapFont    *font;
    uint32_t        size;
    uint32_t        color;
}PixmapFontContext;

}

#endif // HGGRAPHICSDEF_H_
