/**
 *  @file    HGMapDef.h
 *  @brief   Map definitions
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

#ifndef HGMAPDEF_H_
#define HGMAPDEF_H_

#include "HGSystem.h"

// this size if fixed for mask array
#define TILE_SIZE 32
#define MAX_MAP_SIZE 256

// minimum climb height
// this value is calculate by the
// most slant tile_45
#define MIN_CLIMB_HEIGHT 1

// use to detect platform tile
#define DETECT_PLATFORM_UP     1
#define DETECT_PLATFORM_DOWN   2

HGNAMESPACE_START

typedef enum
{
    Tile_None = 0,
    Tile_Solid,
	Tile_Slope_Left_45,		// '\'
	Tile_Slope_Left_27_1,
	Tile_Slope_Left_27_2,
	Tile_Slope_Right_45,	// '/'
    Tile_Slope_Right_27_1,
    Tile_Slope_Right_27_2,
    Tile_Platform,
    
    Tiles_Count
}TileType;

// map header structure
typedef struct
{
    uint32_t	tag;
    uint32_t	version;
    int32_t		width;
    int32_t		height;
    uint32_t	checksum;
}map_header_t;

// slope resistance factor
const static float tile_resistance_factor[Tiles_Count] =
{ 1.0f, 1.0f, 0.8f, 0.95f, 0.95f, 0.8f, 0.95f, 0.95f, 1.0f };
//{ 1.0f, 1.0f, 0.6f, 0.8f, 0.8f, 0.6f, 0.8f, 0.8f, 1.0f };

/*
// tile height
const static int tile_mask[Tiles_Count][TILE_SIZE] =
{
//const static char tile_mask_none[TILE_SIZE]
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
//const static char tile_mask_solid[TILE_SIZE]
{ 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 },
//const static char tile_mask_slope_left_45[TILE_SIZE] =
{ 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1 },
//const static char tile_mask_slope_left_27_1[TILE_SIZE] =
{ 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17 },
//const static char tile_mask_slope_left_27_2[TILE_SIZE] =
{ 16, 16, 15, 15, 14, 14, 13, 13, 12, 12 ,11, 11, 10, 10,  9,  9,  8,  8,  7,  7,  6,  6,  5,  5,  4,  4,  3,  3,  2,  2,  1,  1 },
//const static char tile_mask_slope_right_45[TILE_SIZE] =
{  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
//const static char tile_mask_slope_right_27_1[TILE_SIZE] =
{  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16 },
//const static char tile_mask_slope_right_27_2[TILE_SIZE] =
{ 17, 17, 18, 18, 19, 19, 20, 20, 21, 21 ,22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32 },
//const static char tile_mask_platform[TILE_SIZE] =
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }

};
 */

/* ***************************** */
/* *****         *             * */
/* ********      *             * */
/* ***********   *             * */
/* ***************             * */
/* ******************          * */
/* *********************       * */
/* ************************    * */
/* *************************** * */
/*   left_27_1     left_27_2     */
/*                               */
/* ***************************** */
/* *             *         ***** */
/* *             *   *********** */
/* *             * ************* */
/* *          ****************** */
/* *       ********************* */
/* *    ************************ */
/* ***************************** */
/*   right_27_1    right_27_2    */

HGNAMESPACE_END

#endif // HGMAPDEF_H_
