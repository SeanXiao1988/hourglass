/**
 *  @file    HGMapData.cpp
 *  @brief   Map data implementation
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/10/11
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

#include "HGMapData.h"
#include "HGData.h"
#include "HGHash.h"
#include "HGRender.h"

HGNAMESPACE_START
    
int MapData::TileMask[Tiles_Count][TILE_SIZE];

void MapData::GenerateTileMask()
{
    for (int i = 0; i < TILE_SIZE; i++)
    {
        TileMask[Tile_None][i]              = 0;
        TileMask[Tile_Platform][i]          = 0;
        TileMask[Tile_Solid][i]             = TILE_SIZE;
        TileMask[Tile_Slope_Left_45][i]     = TILE_SIZE - i;
        TileMask[Tile_Slope_Right_45][i]    = i + 1;
        TileMask[Tile_Slope_Left_27_1][i]   = TILE_SIZE - i/2;
        TileMask[Tile_Slope_Left_27_2][i]   = TILE_SIZE/2 - i/2;
        TileMask[Tile_Slope_Right_27_1][i]  = i/2 + 1;
        TileMask[Tile_Slope_Right_27_2][i]  = i/2 + TILE_SIZE/2 + 1;
    }
}
//
    
// Composite
void MapData::RegisterComponentType(void)
{
    
}

void MapData::deInitialize()
{
    
}

EventResult MapData::handleEvent(const Event& event)
{
    return EVENT_RESULT_IGNORED;
}

uint32_t MapData::getComponentName()
{
    return Hash("MapData");
}

MapData::MapData()
	: mData(NULL)
{
	memset(&mMapHeader, 0, sizeof(map_header_t));
}

MapData::~MapData()
{
    clear();
}

bool MapData::loadFromFile(const char* filename)
{
	bool loaded = false;

	Data *mapData = Data::CreateFromFileContent(filename);

	BREAK_START;
        
    if (mapData == NULL)
        break;

    memcpy(&mMapHeader, mapData->getData(), sizeof(map_header_t));

    if (mMapHeader.width > MAX_MAP_SIZE || mMapHeader.height > MAX_MAP_SIZE)
        break;

    if (mapData->getLength() < (sizeof(map_header_t)+mMapHeader.width*mMapHeader.height))
        break;

    clear();
    
    memcpy(&mMapHeader, mapData->getData(), sizeof(map_header_t));
    
    mData = new TileType[mMapHeader.width*mMapHeader.height];
    memcpy(mData, mapData->getData() + sizeof(map_header_t), sizeof(TileType) * mMapHeader.width*mMapHeader.height);

    loaded = true;

    BREAK_END;

	delete mapData;

	return loaded;
}
    
void MapData::clear()
{
	memset(&mMapHeader, 0, sizeof(map_header_t));

    if (mData != NULL)
    {
        delete[] mData;
    }
    
    mData = NULL;
}

bool MapData::createMap(int w, int h)
{
    bool ok = false;

    BREAK_START;
    
    if (w <= 0 || h <= 0)
        break;
    
    clear();
    
    mMapHeader.version = 1;
    mMapHeader.tag = 0;
    mMapHeader.width = w;
    mMapHeader.height = h;

    mData = new TileType[mMapHeader.width * mMapHeader.height];
    memset(mData, 0, sizeof(TileType) * mMapHeader.width * mMapHeader.height);

    BREAK_END;
    
    return ok;
}

bool MapData::saveToFile(const char* filename)
{
	bool saved = false;

    BREAK_START;
    
    if (mData == NULL)
        break;

    if (filename == NULL)
        break;
    
    Data *data = Data::CreateEmpty();
    if (data == NULL)
        break;

    data->appendData((char*)&mMapHeader, sizeof(map_header_t));
    data->appendData((char*)mData, mMapHeader.width * mMapHeader.height * sizeof(TileType));
    saved = data->writeToFile(filename, true);

    delete data;
		
    BREAK_END;

    return saved;
}

void MapData::setTile(int gridX, int gridY, TileType type)
{
	if (mData == NULL)
		return;
	
	if (gridX >= mMapHeader.width || gridY >= mMapHeader.height || gridX < 0 || gridY < 0)
		return;

	if (type < Tile_None || type >= Tiles_Count)
		return;

	mData[gridY * mMapHeader.width + gridX] = type;
}
    
void MapData::setTexture(GLuint tex)
{
    if (mDebugTexture != 0)
    {
        RENDER.textureFree(mDebugTexture);
    }
    
    mDebugTexture = tex;
}
    
void MapData::update()
{
    static bool isClicked = false;
    int mouseX;
    int mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    
    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && isClicked)
    {
        int type = (int)getTileType(mouseX, mouseY);
        type++;
        type %= (int)Tiles_Count;
        setTile(mouseX/TILE_SIZE, mouseY/TILE_SIZE, (TileType)type);
        
        isClicked = false;
    }
    else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        isClicked = true;
    }
    
    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        setTile(mouseX/TILE_SIZE, mouseY/TILE_SIZE, Tile_None);
    }
}
    
void MapData::verify()
{

}
        
bool MapData::isSolid(int x, int y, int special)
{
    if (mData == NULL)
    {
        return true;
    }
    
    bool solid = true;
    
    int gridX = x/TILE_SIZE;
    int gridY = y/TILE_SIZE;
    int tileX = x%TILE_SIZE;
    int tileY = y%TILE_SIZE;
    
    if (gridX >= mMapHeader.width || gridY >= mMapHeader.height)
    {
        return true;
    }
    
    TileType tileType = mData[gridY * mMapHeader.width + gridX];

    switch (tileType)
    {
        case Tile_None:
            solid = false;
            break;
        
        case Tile_Solid:
            solid = true;
            break;
            
        case Tile_Platform:
        {
            if (special == 0)
                solid = false;
            else if (special == DETECT_PLATFORM_DOWN)
                solid = true;
            else
                solid = false;
            
            break;
        }
            
        case Tile_Slope_Left_27_1:
        case Tile_Slope_Left_27_2:
        case Tile_Slope_Left_45:
        case Tile_Slope_Right_27_1:
        case Tile_Slope_Right_27_2:
        case Tile_Slope_Right_45:
            solid = (tileY >= (TILE_SIZE - TileMask[tileType][tileX])) ? true : false;
            break;
            
        default:
            break;
    }
    
	return solid;
}

TileType MapData::getTileType(int x, int y, int special)
{
    TileType ret = Tile_Solid;
    
    BREAK_START;
    
    int gridX = x/TILE_SIZE;
    int gridY = y/TILE_SIZE;
    
    if (gridX >= mMapHeader.width || gridY >= mMapHeader.height || gridX < 0 || gridY < 0)
        break;
        
    ret = (TileType)mData[gridY * mMapHeader.width + gridX];
    
    if (special == 0)
    {
        if (ret == Tile_Platform)
            ret = Tile_None;
    }
    else if (special == DETECT_PLATFORM_UP)
    {
        if (ret == Tile_Platform)
            ret = Tile_None;
    }
    else if (special == DETECT_PLATFORM_DOWN)
    {
        if (ret == Tile_Platform)
            ret = Tile_Solid;
    }

    BREAK_END;
    
    return ret;
}
    
TileType MapData::getTileTypeByGridCoord(int gridX, int gridY)
{
    if (gridX >= mMapHeader.width || gridY >= mMapHeader.height)
    {
        return Tile_Solid;
    }
    
    return (TileType)mData[gridY * mMapHeader.width + gridX];
}
    
void MapData::debug_draw(int x, int y)
{
	int i, j;
	
	Quad quad;
	quad.blend = BLEND_DEFAULT;
	quad.tex = mDebugTexture;
    
    float textureWidth = (float)RENDER.textureGetWidth(mDebugTexture);
    float textureHeight = (float)RENDER.textureGetHeight(mDebugTexture);
    
    quad_set_color(&quad, 0xFFFFFFFF);
	
	for(i = 0; i < mMapHeader.height; i++){
		for(j = 0; j < mMapHeader.width; j++){
			TileType type = mData[i*mMapHeader.width + j];
            int typeX = type % (Tiles_Count - 1);
            int typeY = type / (Tiles_Count - 1);
			
			quad.v[0].u = TILE_SIZE * typeX / textureWidth;
			quad.v[0].v = typeY * TILE_SIZE / textureHeight;
			quad.v[0].x = j*TILE_SIZE;
			quad.v[0].y = i*TILE_SIZE;
			
			quad.v[1].u = TILE_SIZE * (typeX + 1) / textureWidth;
			quad.v[1].v = typeY * TILE_SIZE / textureHeight;
			quad.v[1].x = (j+1)*TILE_SIZE;
			quad.v[1].y = i*TILE_SIZE;
			
			quad.v[2].u = quad.v[1].u;
			quad.v[2].v = TILE_SIZE * (typeY + 1) / textureHeight;
			quad.v[2].x = quad.v[1].x;
			quad.v[2].y = (i+1)*TILE_SIZE;
			
			quad.v[3].u = quad.v[0].u;
			quad.v[3].v = quad.v[2].v;
			quad.v[3].x = quad.v[0].x;
			quad.v[3].y = quad.v[2].y;
			
			quad.v[0].z = 1.0f;
			quad.v[1].z = 1.0f;
			quad.v[2].z = 1.0f;
			quad.v[3].z = 1.0f;
			
			RENDER.renderQuad(&quad);
		}
	}
}

HGNAMESPACE_END