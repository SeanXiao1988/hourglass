/**
 *  @file    HGEventDef.h
 *  @brief   Event definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/06
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

#ifndef HGEVENTDEF_H_
#define HGEVENTDEF_H_

#include "HGSystem.h"

namespace HG
{

typedef enum
{
    EVENT_WILDCARD = 0,
    
    EVENT_UPDATE,           /* frame update */
	EVENT_INPUT,            /* input event */
    EVENT_KEYBOARD,         /* keyboard event */
    
    EVENT_COUNT
}EventID;

typedef enum
{
    EVENT_RESULT_IGNORED = 0,
	EVENT_RESULT_PROCESSED,
    EVENT_RESULT_INTERCEPT
}EventResult;
    
}

#endif // HGEVENTDEF_H_
