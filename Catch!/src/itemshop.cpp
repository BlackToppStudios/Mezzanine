#ifndef _itemshop_cpp
#define _itemshop_cpp

#include "itemshop.h"

ItemShop::ItemShop()
{
    Mesh* SelectSphereMesh = MeshManager::GetSingletonPtr()->CreateSphereMesh("SelectSphereMesh",ColourValue(0.15,0.15,1.0),50.0);
    SelectionSphere = SceneManager::GetSingletonPtr()->CreateEntity("SelectionSphere",SelectSphereMesh->GetName(),SelectSphereMesh->GetGroup());
}

ItemShop::~ItemShop()
{
    SceneManager::GetSingletonPtr()->DestroyEntity(SelectionSphere);
}

#endif
