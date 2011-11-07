#ifndef _itemshopitems_h
#define _itemshopitems_h

#include <physgame.h>

using namespace phys;

class ItemShopItem : public ActorRigid
{
    protected:
    public:
        ItemShopItem(const String& Name, const String& Mesh, const String& Group);
        ~ItemShopItem();
};//Itemshopitem

class Teleporter : public ItemShopItem
{

};//Teleporter

#endif
