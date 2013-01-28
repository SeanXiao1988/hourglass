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

extern void vertex_set_default(Vertex* v);
extern void vertex_set_color(Vertex* v, uint32_t color);

extern void quad_set_default(Quad* q);
extern void quad_set_color(Quad* q, uint32_t color);
extern void quad_set_alpha(Quad* q, uint8_t alpha);
extern void quad_set_texture_rect(Quad* q, float x, float y, float w, float h, float texW, float texH);
extern void quad_set_coord(Quad* q, float x, float y, float w, float h);

HGNAMESPACE_END

#endif // HGGRAPHICSUTIL_H_