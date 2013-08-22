#ifndef _itemshop_h
#define _itemshop_h

#include <mezzanine.h>

#include "itemshopitems.h"

using namespace Mezzanine;

class ItemShop
{
    protected:
        Entity* SelectionSphere;
        Whole StarterCash;
        Whole CurrentCash;
        std::vector<ItemShopItem*> Items;
        //std::map<String,Whole> ShopCostValues;
    public:
        ItemShop();
        ~ItemShop();

        Whole GetCurrentCash();
        Whole GetStarterCash();

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void SetLevelCash(const Whole& Cash);
        void RegisterShopItem();
        void EnableShopItem();
        void ResetLevelData();
};//ItemShop

#endif
