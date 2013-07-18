#ifndef _itemshop_cpp
#define _itemshop_cpp

#include "itemshop.h"

ItemShop::ItemShop()
    : StarterCash(0),
      CurrentCash(0)
{
    //Mesh* SelectSphereMesh = MeshManager::GetSingletonPtr()->CreateSphereMesh("SelectSphereMesh",ColourValue(0.15,0.15,1.0),50.0);
    //SelectionSphere = Entresol::GetSingletonPtr()->GetSceneManager()->CreateEntity("SelectionSphere",SelectSphereMesh->GetName(),SelectSphereMesh->GetGroup());
}

ItemShop::~ItemShop()
{
    //Entresol::GetSingletonPtr()->GetSceneManager()->DestroyEntity(SelectionSphere);
}

Whole ItemShop::GetCurrentCash()
{
    return CurrentCash;
}

Whole ItemShop::GetStarterCash()
{
    return StarterCash;
}

void ItemShop::SetLevelCash(const Whole& Cash)
{
    StarterCash = Cash;
    CurrentCash = Cash;
}

void ItemShop::RegisterShopItem()
{

}

void ItemShop::EnableShopItem()
{

}

void ItemShop::ResetLevelData()
{

}

#endif
