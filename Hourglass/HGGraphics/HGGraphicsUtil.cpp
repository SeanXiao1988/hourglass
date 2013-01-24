/**
 *  @file    HGGraphicsUtil.cpp
 *  @brief   Graphics Util implementation
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

#include "HGGraphicsUtil.h"

HGNAMESPACE_START

void vertex_set_default(Vertex* v)
{
    memset(v, 0, sizeof(Vertex));
    memset(v->color, 255, sizeof(GLbyte) * 4);
}

void vertex_set_color(Vertex* v, uint32_t color)
{
    v->color[0] = GETR(color);
    v->color[1] = GETG(color);
    v->color[2] = GETB(color);
    v->color[3] = GETA(color);
}

void quad_set_default(Quad* q)
{
    q->tex = 0;
    q->blend = BLEND_DEFAULT;
    
    vertex_set_default(&q->v[0]);
    vertex_set_default(&q->v[1]);
    vertex_set_default(&q->v[2]);
    vertex_set_default(&q->v[3]);
}

void quad_set_alpha(Quad* q, uint8_t alpha)
{
    q->v[0].color[3] = alpha;
    q->v[1].color[3] = alpha;
    q->v[2].color[3] = alpha;
    q->v[3].color[3] = alpha;
}

void quad_set_color(Quad* q, uint32_t color)
{
    vertex_set_color(&q->v[0], color);
    vertex_set_color(&q->v[1], color);
    vertex_set_color(&q->v[2], color);
    vertex_set_color(&q->v[3], color);
}

void quad_set_texture_rect(Quad* q, float x, float y, float w, float h, float texW, float texH)
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

HGNAMESPACE_END