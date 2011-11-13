#ifndef _itemshopitems_cpp
#define _itemshopitems_cpp

#include "itemshopitems.h"

ItemShopItem::ItemShopItem(const Real& Mass, const String& Name, const String& Mesh, const String& Group)
    : ActorRigid(Mass,Name,Mesh,Group)
{

}

ItemShopItem::~ItemShopItem()
{

}

#endif
