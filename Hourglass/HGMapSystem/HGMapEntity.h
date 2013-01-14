/**
 *  @file    HGMapEntity.h
 *  @brief   Map Entity
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/05
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

#ifndef HGMAPENTITY_H_
#define HGMAPENTITY_H_

#include "HGISceneEntity.h"
#include "HGGraphicsDef.h"

namespace HG
{

class MapEntity : public ISceneEntity
{
public:
    MapEntity();
    ~MapEntity();
    
    void render();
    
	void		setTexture(GLuint tex);
	void		setTextureRect(float x, float y, float w, float h, bool adjust = true);
    
private:
    Quad mQuad;
    
	float	mWidth;
	float	mHeight;
	float	mTexWidth;
	float	mTexHeight;
};

}

#endif // HGMAPENTITY_H_
