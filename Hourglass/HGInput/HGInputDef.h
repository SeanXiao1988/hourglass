/**
 *  @file    HGInputDef.h
 *  @brief   Input definitions
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/08
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

#ifndef HGINPUTDEF_H_
#define HGINPUTDEF_H_

#include "HGSystem.h"

HGNAMESPACE_START

typedef enum
{
    INPUT_PRESS = 0,
    INPUT_RELEASE,
    INPUT_TRIGGER,
    
    INPUT_MOUSE_MOVE,
    INPUT_MOUSE_LEFT,
    INPUT_MOUSE_RIGHT
    
}INPUT_TYPE;
    
typedef struct
{
    union
    {
        int     key;
        int     x;
        int     botton;
    };
    int         y;
    INPUT_TYPE  action;
}InputInfo;
    
HGNAMESPACE_END

#endif // HGINPUTDEF_H_
