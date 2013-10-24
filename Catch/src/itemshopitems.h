#ifndef _itemshopitems_h
#define _itemshopitems_h

#include <mezzanine.h>

using namespace Mezzanine;

class ItemShopItem : public Debris
{
    protected:
    public:
        ItemShopItem(const Real& Mass, const String& Name, const String& Mesh, const String& Group);
        ~ItemShopItem();
};//Itemshopitem

class Teleporter : public ItemShopItem
{

};//Teleporter

#endif
