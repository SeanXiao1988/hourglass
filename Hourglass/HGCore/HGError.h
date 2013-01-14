/**
 *  @file    HGError.h
 *  @brief   Error definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/08/22
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

#ifndef HGERROR_H_
#define HGERROR_H_

typedef enum hg_error_types
{
    HG_OK							= 0 << 0,
    HG_ERROR_FILE_NOT_EXIST			= 1 << 0,
    HG_ERROR_FILE_CORRUPT			= 1 << 1,
    HG_ERROR_NAME_DUPLICATE			= 1 << 2,
    HG_ERROR_TEXTURE_INUSE			= 1 << 3,
    HG_ERROR_TEXTURE_NOT_EXIST		= 1 << 4,
    HG_ERROR_GLFW					= 1 << 5,
    HG_ERROR_GLEW					= 1 << 6,
    HG_ERROR_GL_VER_NOT_SUPPORT		= 1 << 7,
	HG_ERROR_GL_EXTENSIONS_ERROR	= 1 << 8,
    HG_ERROR_INSUFFICIENT_MEMORY    = 1 << 9,
    HG_ERROR_BAD_INVOCATION         = 1 << 10
}HG_ERROR;

#endif // HGERROR_H_
