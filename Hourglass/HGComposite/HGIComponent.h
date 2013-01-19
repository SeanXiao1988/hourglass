/**
 *  @file    HGIComponent.h
 *  @brief   Component interface
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

#ifndef HGICOMPONENT_H_
#define HGICOMPONENT_H_

#include "HGComponentDef.h"
#include "HGEvent.h"

HGNAMESPACE_START
    
class IComponent
{
public:
    IComponent() : mObjectName(0) {}
    virtual ~IComponent() {};
    
    virtual void            deInitialize() = 0;
    virtual EventResult     handleEvent(const Event& event) { return EVENT_RESULT_IGNORED; }
    virtual ComponentTypeID getComponentTypeID() = 0;
    virtual uint32_t        getComponentName() = 0;
    const uint32_t          getObjectName() const { return mObjectName; }

protected:
    friend class ObjectManager;
    uint32_t     mObjectName;
};
    
HGNAMESPACE_END

#endif // HGICOMPONENT_H_