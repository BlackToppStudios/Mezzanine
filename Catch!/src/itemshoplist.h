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
        UIRectangle* SideScrollBack;
        UIRectangle* SideScroll;
        std::vector<UIButton*> ItemList;
        UIButton* Selected;
        Real VertDist;
        Vector2 Offscreen;
        Real ScrollUpperLimit;
        Real ScrollLowerLimit;
    public:
        ItemShopList(Real X, Real Y, Real Width, Real Height, UILayer* Layer, World* TheWorld);
        ~ItemShopList();
        void DrawList();
        void SetDistanceBetweenButtons(Real Distance);
        void CreateListArea(Real X, Real Y, Real Width, Real Height);
        void AddItemToList(UIButton* Item);
        UIRectangle* GetBackdrop();
        UIRectangle* GetListArea();
        UIRectangle* GetSideScrollBack();
        UIRectangle* GetSideScroll();
        UIButton* GetSelectedItem();
};

#endif
