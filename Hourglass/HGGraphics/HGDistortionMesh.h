/**
 *  @file    HGDistortionMesh.h
 *  @brief   Distortion mesh header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/13
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

#ifndef HGDISTORTIONMESH_H_
#define HGDISTORTIONMESH_H_

#include "HGSystem.h"
#include "HGGraphicsDef.h"

#define DISTORTION_NODE     0
#define DISTORTION_TOPLEFT  1
#define DISTORTION_CENTER   2

HGNAMESPACE_START

class DistortionMesh
{
public:
    explicit DistortionMesh(int cols, int rows);
    explicit DistortionMesh(const DistortionMesh& dm);
    ~DistortionMesh();
    
    DistortionMesh& operator= (const DistortionMesh& other);
    
    void        render(float x, float y);
    void        clear(uint32_t color = 0xFFFFFFFF, float z = 0.5f);
    
    void        setTexture(GLuint tex) { mQuad.tex = tex; }
    void        setTextureRect(float x, float y, float w, float h);
    void        setBlendMode(int blend) { mQuad.blend = blend; }
    void        setZ(int col, int row, float z);
    void        setColor(int col, int row, uint32_t color);
    void        setDisplacement(int col, int row, float dx, float dy, int ref);
    
    GLuint      getTexture() const { return mQuad.tex; }
    void        getTextureRect(float *x, float *y, float *w, float *h) { *x = mU; *y = mV; *w = mWidth; *h = mHeight; }
    int         getBlendMode() const { return mQuad.blend; }
    float       getZ(int col, int row) const;
    uint32_t    getColor(int col, int row) const;
    void        getDisplacement(int col, int row, float *dx, float *dy, int ref) const;
    
    int         getRows() { return mRows; }
    int         getCols() { return mCols; }
    
private:
    DistortionMesh();
    
    Vertex     *mDispArray;
    int         mRows;
    int         mCols;
    float       mCellW;
    float       mCellH;
    float       mU;
    float       mV;
    float       mWidth;
    float       mHeight;
    Quad        mQuad;
};
 
HGNAMESPACE_END

#endif // HGDISTORTIONMESH_H_