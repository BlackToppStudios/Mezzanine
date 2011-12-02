#ifndef _itemshop_h
#define _itemshop_h

#include <mezzanine.h>

#include "itemshopitems.h"

using namespace Mezzanine;

class ItemShop
{
    protected:
        Entity* SelectionSphere;
        std::vector<ItemShopItem*> Items;
    public:
        ItemShop();
        ~ItemShop();

        void RegisterShopItem();
        void ResetEnabledShopItems();
        void EnableShopItem();
};//ItemShop

#endif
