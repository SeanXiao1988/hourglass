/**
 *  @file    HGInputListener.h
 *  @brief   InputListener header
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/01/17
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2013 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGINPUTLISTENER_H_
#define HGINPUTLISTENER_H_

#include "HGSystem.h"
#include "HGIComponent.h"

HGNAMESPACE_START
    
class InputListener : public IComponent
{
public:
    InputListener() {}
    ~InputListener() {}
    
    void            setScriptCallback(const char* func) { mScriptCallback = func; }
    std::string&    getScriptCallback() { return mScriptCallback; }
    
    // Composite
    static void             RegisterComponentType(void);
    virtual void            deInitialize();
    virtual EventResult     handleEvent(const Event& event);
    virtual ComponentTypeID getComponentTypeID() { return COMP_INPUT_LISTENER; };
    virtual uint32_t        getComponentName();
    
private:
    std::string mScriptCallback;
};
    
HGNAMESPACE_END

#endif // HGINPUTLISTENER_H_