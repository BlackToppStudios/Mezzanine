#ifndef _itemshop_cpp
#define _itemshop_cpp

#include "itemshop.h"

ItemShop::ItemShop()
{
    Mesh* SelectSphereMesh = World::GetWorldPointer()->GetMeshManager()->CreateSphereMesh("SelectSphereMesh",ColourValue(0.15,0.15,1.0),50.0);
    SelectionSphere = World::GetWorldPointer()->GetSceneManager()->CreateEntity("SelectionSphere",SelectSphereMesh->GetName(),SelectSphereMesh->GetGroup());
}

ItemShop::~ItemShop()
{
    World::GetWorldPointer()->GetSceneManager()->DestroyEntity(SelectionSphere);
}

#endif
