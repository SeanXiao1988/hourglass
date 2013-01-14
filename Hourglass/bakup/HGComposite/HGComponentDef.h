/**
 *  @file    HGComponentDef.h
 *  @brief   Component definition
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/11
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

#ifndef HGCOMPONENTDEF_H_
#define HGCOMPONENTDEF_H_

#include "HGSystem.h"

namespace HG
{

typedef enum
{
    COMP_NONE = 0,
    COMP_SCENE_NODE,
    COMP_QUAD_ENTITY,
    COMP_MAP_ENTITY,
    COMP_PHYSICAL_OBJECT,
    COMP_INPUT_LISTENER,
    
    COMP_COUNT
}ComponentTypeID;

}

#endif // HGCOMPONENTDEF_H_
