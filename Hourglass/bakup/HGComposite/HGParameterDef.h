/**
 *  @file    HGParameterDef.h
 *  @brief   parameter pre-definitions
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/12/16
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

#ifndef HGPARAMETERDEF_H_
#define HGPARAMETERDEF_H_

#include "HGSystem.h"

namespace HG
{

// physical object
const uint32_t HashPhysicalObject       = 0xED78A78B;
const uint32_t ParamObjectX             = 0x40873A04;
const uint32_t ParamObjectY             = 0x40873A05;
const uint32_t ParamMapColliderX        = 0x13D3FB9B;
const uint32_t ParamMapColliderY        = 0x13D3FB9A;
const uint32_t ParamMapColliderH        = 0x13D3FB8B;
const uint32_t ParamPhysicalObjID       = 0x5788F36E;
const uint32_t ParamCollisionRectCount  = 0x2FBDCE26;

// Scene Node
const uint32_t HashSceneNode            = 0xC750FB87;
const uint32_t ParamName                = 0xB440BCF4;
const uint32_t ParamParent              = 0xB152A0D5;
const uint32_t ParamX                   = 0x050C5D47;
const uint32_t ParamY                   = 0x050C5D46;
const uint32_t ParamAlpha               = 0x4073673B;
const uint32_t ParamScaleX              = 0xDD7D11E7;
const uint32_t ParamScaleY              = 0xDD7D11E6;
const uint32_t ParamRotation            = 0xB6FE2F5B;
const uint32_t ParamVisibility          = 0xE04CE2B9;
    
// Scene Entity
const uint32_t HashQuadEntity           = 0x23E90259;

}
#endif // HGPARAMETERDEF_H_
