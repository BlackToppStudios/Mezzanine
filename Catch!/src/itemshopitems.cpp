#ifndef _itemshopitems_cpp
#define _itemshopitems_cpp

#include "itemshopitems.h"

ItemShopItem::ItemShopItem(const String& Name, const String& Mesh, const String& Group)
    : ActorRigid(Name,Mesh,Group);
{

}

ItemShopItem::~ItemShopItem()
{

}

#endif
