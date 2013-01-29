/**
 *  @file    HGGraphicsUtil.h
 *  @brief   Graphics Util header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/24
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

#ifndef HGGRAPHICSUTIL_H_
#define HGGRAPHICSUTIL_H_

#include "HGGraphicsDef.h"

HGNAMESPACE_START
/*
extern void vertex_set_default(Vertex* v);
extern void vertex_set_color(Vertex* v, uint32_t color);
extern void vertex_print(Vertex* v);

extern void quad_set_default(Quad* q);
extern void quad_set_color(Quad* q, uint32_t color);
extern void quad_set_color_4f(Quad* q, color4f_t& color);
extern void quad_set_alpha(Quad* q, uint8_t alpha);
extern void quad_set_texture_rect(Quad* q, float x, float y, float w, float h, float texW, float texH);
extern void quad_set_coord(Quad* q, float x, float y, float w, float h);
extern void quad_print(Quad* q);
*/

inline void vertex_set_default(Vertex* v)
{
    memset(v, 0, sizeof(Vertex));
    memset(v->color, 255, sizeof(GLbyte) * 4);
    v->z = 1.0f;
}

inline void vertex_set_color(Vertex* v, uint32_t color)
{
    v->color[0] = GETR(color);
    v->color[1] = GETG(color);
    v->color[2] = GETB(color);
    v->color[3] = GETA(color);
}

inline void vertex_print(Vertex* v)
{
    HGLog("vertex:%p | x[%.3f] y[%.3f] z[%.3f] u[%.3f] v[%.3f] color[%08x]\n", v, v->x, v->y, v->z, v->u, v->v, RGBA(v->color[0], v->color[1], v->color[2], v->color[3]));
}

inline void quad_set_default(Quad* q)
{
    q->tex = 0;
    q->blend = BLEND_DEFAULT;
    
    vertex_set_default(&q->v[0]);
    vertex_set_default(&q->v[1]);
    vertex_set_default(&q->v[2]);
    vertex_set_default(&q->v[3]);
}

inline void quad_set_alpha(Quad* q, uint8_t alpha)
{
    q->v[0].color[3] = alpha;
    q->v[1].color[3] = alpha;
    q->v[2].color[3] = alpha;
    q->v[3].color[3] = alpha;
}

inline void quad_set_color(Quad* q, uint32_t color)
{
    vertex_set_color(&q->v[0], color);
    vertex_set_color(&q->v[1], color);
    vertex_set_color(&q->v[2], color);
    vertex_set_color(&q->v[3], color);
}

inline void quad_set_color_4f(Quad* q, color4f_t& color)
{
    q->v[0].color[0] = (GLbyte)(255.0f * color.r);
    q->v[0].color[1] = (GLbyte)(255.0f * color.g);
    q->v[0].color[2] = (GLbyte)(255.0f * color.b);
    q->v[0].color[3] = (GLbyte)(255.0f * color.a);
    
    q->v[1].color[0] = q->v[0].color[0];
    q->v[1].color[1] = q->v[0].color[1];
    q->v[1].color[2] = q->v[0].color[2];
    q->v[1].color[3] = q->v[0].color[3];
    
    q->v[2].color[0] = q->v[0].color[0];
    q->v[2].color[1] = q->v[0].color[1];
    q->v[2].color[2] = q->v[0].color[2];
    q->v[2].color[3] = q->v[0].color[3];
    
    q->v[3].color[0] = q->v[0].color[0];
    q->v[3].color[1] = q->v[0].color[1];
    q->v[3].color[2] = q->v[0].color[2];
    q->v[3].color[3] = q->v[0].color[3];
}

inline void quad_set_texture_rect(Quad* q, float x, float y, float w, float h, float texW, float texH)
{
    q->v[0].u = x / texW;
    q->v[0].v = y / texH;
    
    q->v[1].u = (x + w) / texW;
    q->v[1].v = q->v[0].v;
    
    q->v[2].u = q->v[1].u;
    q->v[2].v = (y + h) / texH;
    
    q->v[3].u = q->v[0].u;
    q->v[3].v = q->v[2].v;
}

inline void quad_set_coord(Quad* q, float x, float y, float w, float h)
{
    q->v[0].x = x;
    q->v[0].y = y;
    
    q->v[1].x = x + w;
    q->v[1].y = y;
    
    q->v[2].x = q->v[1].x;
    q->v[2].y = y + h;
    
    q->v[3].x = x;
    q->v[3].y = q->v[2].y;
}

inline void quad_print(Quad* q)
{
    HGLog("quad:%p | blend[%d] texture[%d] \n", q, q->blend, q->tex);
    vertex_print(&q->v[0]);
    vertex_print(&q->v[1]);
    vertex_print(&q->v[2]);
    vertex_print(&q->v[3]);
}

HGNAMESPACE_END

#endif // HGGRAPHICSUTIL_H_