#ifndef _itemshoplist_h
#define _itemshoplist_h

#include <physgame.h>

using namespace phys;

class ItemShopList
{
    protected:
        GraphicsManager* GraphMan;
        UILayer* ParentLayer;
        UIManager* UIMan;
        UIRectangle* Backdrop;
        UIRectangle* ListArea;
        std::vector<UIButton*> ItemList;
        UIButton* Selected;
        Real VertDist;
        Whole OffscreenX;
        Whole OffscreenY;
    public:
        ItemShopList(Real X, Real Y, Real Width, Real Height, UILayer* Layer, World* TheWorld);
        ~ItemShopList();
        void SetDistanceBetweenButtons(Real Distance);
        void CreateListArea(Real X, Real Y, Real Width, Real Height);
        void AddItemToList(UIButton* Item);
        UIRectangle* GetBackdrop();
        UIRectangle* GetListArea();
        UIButton* GetSelectedItem();
};

#endif
