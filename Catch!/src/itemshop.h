#ifndef _itemshop_h
#define _itemshop_h

#include <physgame.h>

using namespace phys;

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
