/**
 *  @file    HGSceneManager.h
 *  @brief   
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/08/09
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

#ifndef HGSCENEMANAGER_H_
#define HGSCENEMANAGER_H_

#include "HGSystem.h"

namespace HG
{

class SceneNode;
typedef std::map<uint32_t, SceneNode*> SceneNodeMap;

#define SCENEMANAGER SceneManager::Instance()

class SceneManager
{
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void		initialize();
	void		deInitialize();

    SceneNode*  createNode(const std::string& name);
    /*
	void		deleteNode(const std::string& name);
    void        deleteNode(const uint32_t hashname);
	void		deleteNode(SceneNode* node);
	void		deleteAll();
     */
    SceneNode*  getNode(const uint32_t hashedName);
    SceneNode*  getNode(const std::string& name);

	SceneNode*	getRoot();

	void		update(const float dt);
    void        render();
    
    void        run(const float dt);
	
	void		removeAll();
    
protected:
    friend class SceneNode;
    void        addNode(SceneNode* node);

private:
    SceneManager();
    SceneManager(const SceneManager& other);
    SceneManager& operator= (const SceneManager& rhs);
	~SceneManager();
    
	SceneNode*      mRootNode;

    SceneNodeMap    mNodeMap;
};

}

#endif // SCENEMANAGER_H_
