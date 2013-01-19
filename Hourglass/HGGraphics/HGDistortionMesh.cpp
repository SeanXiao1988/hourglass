/**
 *  @file    HGDistortionMesh.cpp
 *  @brief   Distortion mesh implementation
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

#include "HGDistortionMesh.h"
#include "HGRender.h"

HGNAMESPACE_START

DistortionMesh::DistortionMesh(int cols, int rows)
{
    mCols = cols;
    mRows = rows;
    mCellW = mCellH = 0;
    mQuad.tex = 0;
    mQuad.blend = BLEND_DEFAULT;
    mDispArray = new Vertex[rows * cols];
    
    for (int i = 0; i < rows * cols; i++)
    {
        mDispArray[i].x = 0.0f;
        mDispArray[i].y = 0.0f;
        mDispArray[i].u = 0.0f;
        mDispArray[i].v = 0.0f;
        
        mDispArray[i].z = 0.5f;
        RENDER.setVertexColor(&mDispArray[i], 0xFFFFFFFF);
    }
}

DistortionMesh::DistortionMesh(const DistortionMesh& dm)
{
    mCols = dm.mCols;
    mRows = dm.mRows;
    mCellW = dm.mCellW;
    mCellH = dm.mCellH;
    mU = dm.mU;
    mV = dm.mV;
    mWidth = dm.mWidth;
    mHeight = dm.mHeight;
    mQuad = dm.mQuad;
    
    mDispArray = new Vertex[mCols * mRows];
    memcpy(mDispArray, dm.mDispArray, sizeof(Vertex)*mCols*mRows);
}

DistortionMesh::~DistortionMesh()
{
    if (mDispArray != NULL)
    {
        delete[] mDispArray;
        mDispArray = NULL;
    }
    
    if (mQuad.tex != 0)
    {
        RENDER.textureFree(mQuad.tex);
    }
}

DistortionMesh& DistortionMesh::operator= (const DistortionMesh& other)
{
    if (this != &other)
    {
        mCols = other.mCols;
        mRows = other.mRows;
        mCellW = other.mCellW;
        mCellH = other.mCellH;
        mU = other.mU;
        mV = other.mV;
        mWidth = other.mWidth;
        mHeight = other.mHeight;
        mQuad = other.mQuad;
        
        delete[] mDispArray;
        mDispArray = new Vertex[mCols * mRows];
        memcpy(mDispArray, other.mDispArray, sizeof(Vertex)*mCols*mRows);
    }
    
    return *this;
}

void DistortionMesh::setTextureRect(float x, float y, float w, float h)
{
    float tw, th;
    mU = x; mV = y; mWidth = w; mHeight = h;
    
    if (mQuad.tex != 0)
    {
        tw = (float)RENDER.textureGetWidth(mQuad.tex);
        th = (float)RENDER.textureGetHeight(mQuad.tex);
    }
    else
    {
        tw = w;
        th = h;
    }
    
    mCellW = w / (mCols - 1);
    mCellH = h / (mRows - 1);
    
    for (int j = 0; j < mRows; j++)
    {
        for (int i = 0; i < mCols; i++)
        {
            mDispArray[j*mCols+i].u = (x+i*mCellW)/tw;
            mDispArray[j*mCols+i].v = (y+j*mCellH)/th;
            
            mDispArray[j*mCols+i].x = i*mCellW;
            mDispArray[j*mCols+i].y = j*mCellH;
        }
    }
}

void DistortionMesh::clear(uint32_t color, float z)
{
    for (int j = 0; j < mRows; j++)
    {
        for (int i = 0; i < mCols; i++)
        {
            mDispArray[j*mCols+i].x = i*mCellW;
            mDispArray[j*mCols+i].y = j*mCellH;
            RENDER.setVertexColor(&mDispArray[j*mCols+i], color);
            mDispArray[j*mCols+i].z = z;
        }
    }
}

void DistortionMesh::render(float x, float y)
{
    int idx;
    for (int j = 0; j < mRows - 1; j++)
    {
        for (int i = 0; i < mCols - 1; i++)
        {
            idx = j * mCols + i;
            
            mQuad.v[0].u = mDispArray[idx].u;
            mQuad.v[0].v = mDispArray[idx].v;
            mQuad.v[0].x = x + mDispArray[idx].x;
            mQuad.v[0].y = y + mDispArray[idx].y;
            mQuad.v[0].z = mDispArray[idx].z;
            RENDER.setVertexColor(&mQuad.v[0], RGBA(mDispArray[idx].color[0], mDispArray[idx].color[1], mDispArray[idx].color[2], mDispArray[idx].color[3]));
            
            mQuad.v[1].u = mDispArray[idx+1].u;
            mQuad.v[1].v = mDispArray[idx+1].v;
            mQuad.v[1].x = x + mDispArray[idx+1].x;
            mQuad.v[1].y = y + mDispArray[idx+1].y;
            mQuad.v[1].z = mDispArray[idx+1].z;
            RENDER.setVertexColor(&mQuad.v[1], RGBA(mDispArray[idx+1].color[0], mDispArray[idx+1].color[1], mDispArray[idx+1].color[2], mDispArray[idx+1].color[3]));
            
            mQuad.v[2].u = mDispArray[idx+mCols+1].u;
            mQuad.v[2].v = mDispArray[idx+mCols+1].v;
            mQuad.v[2].x = x + mDispArray[idx+mCols+1].x;
            mQuad.v[2].y = y + mDispArray[idx+mCols+1].y;
            mQuad.v[2].z = mDispArray[idx+mCols+1].z;
            RENDER.setVertexColor(&mQuad.v[2], RGBA(mDispArray[idx+mCols+1].color[0], mDispArray[idx+mCols+1].color[1], mDispArray[idx+mCols+1].color[2], mDispArray[idx+mCols+1].color[3]));
            
            mQuad.v[3].u = mDispArray[idx+mCols].u;
            mQuad.v[3].v = mDispArray[idx+mCols].v;
            mQuad.v[3].x = x + mDispArray[idx+mCols].x;
            mQuad.v[3].y = y + mDispArray[idx+mCols].y;
            mQuad.v[3].z = mDispArray[idx+mCols].z;
            RENDER.setVertexColor(&mQuad.v[3], RGBA(mDispArray[idx+mCols].color[0], mDispArray[idx+mCols].color[1], mDispArray[idx+mCols].color[2], mDispArray[idx+mCols].color[3]));
            
            RENDER.renderQuad(&mQuad);
        }
    }
}

void DistortionMesh::setZ(int col, int row, float z)
{
    if (row < mRows && col < mCols)
    {
        mDispArray[row*mCols+col].z = z;
    }
}

void DistortionMesh::setColor(int col, int row, uint32_t color)
{
    if (row < mRows && col < mCols)
    {
        RENDER.setVertexColor(&mDispArray[row*mCols+col], color);
    }
}

void DistortionMesh::setDisplacement(int col, int row, float dx, float dy, int ref)
{
    if (row < mRows && col < mCols)
    {
        switch (ref)
        {
            case DISTORTION_NODE:
                dx+=col*mCellW;
                dy+=row*mCellH;
                break;
            
            case DISTORTION_CENTER:
                dx+=mCellW*(mCols-1)/2;
                dy+=mCellH*(mRows-1)/2;
                break;
                
            case DISTORTION_TOPLEFT:
                break;
                
            default:
                break;
        }
        
        mDispArray[row*mCols+col].x = dx;
        mDispArray[row*mCols+col].y = dy;
    }
}

float DistortionMesh::getZ(int col, int row) const
{
    if (row < mRows && col < mCols)
    {
        return mDispArray[row*mCols+col].z;
    }
    else
    {
        return 0.0f;
    }
}

uint32_t DistortionMesh::getColor(int col, int row) const
{
    if (row < mRows && col < mCols)
    {
        int idx = row * mCols + col;
        return RGBA(mDispArray[idx].color[0], mDispArray[idx].color[1], mDispArray[idx].color[2], mDispArray[idx].color[3]);
    }
    else
    {
        return 0;
    }
}


void DistortionMesh::getDisplacement(int col, int row, float *dx, float *dy, int ref) const
{
	if(row<mRows && col<mCols)
	{
		switch(ref)
		{
            case DISTORTION_NODE:
                *dx=mDispArray[row*mCols+col].x-col*mCellW;
                *dy=mDispArray[row*mCols+col].y-row*mCellH;
                break;
                
            case DISTORTION_CENTER:	
                *dx=mDispArray[row*mCols+col].x-mCellW*(mCols-1)/2;
                *dy=mDispArray[row*mCols+col].x-mCellH*(mRows-1)/2;
                break;
                
            case DISTORTION_TOPLEFT:
                *dx=mDispArray[row*mCols+col].x;
                *dy=mDispArray[row*mCols+col].y;
                break;
		}
	}
}

HGNAMESPACE_END
