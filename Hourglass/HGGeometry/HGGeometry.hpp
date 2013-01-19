/**
 *  @file    HGGeometry.hpp
 *  @brief   Geometry
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/23
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

#ifndef HGGEOMETRY_HPP_
#define HGGEOMETRY_HPP_

#include "HGVector2D.hpp"
#include "HGRectangle.hpp"

HGNAMESPACE_START

template<class T, class U>
T Clamp(T in, U low, U high)
{
    if(in <= low)
        return low;
    else if(in >= high)
        return high;
    else
        return in;
}
    
HGNAMESPACE_END

#endif // HGGEOMETRY_HPP_
