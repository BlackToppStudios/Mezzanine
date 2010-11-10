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
        UI::Rectangle* Backdrop;
        UI::Rectangle* ListArea;
        UI::Rectangle* SideScrollBack;
        UI::Rectangle* SideScroll;
        std::vector<UI::Button*> ItemList;
        UI::Button* Selected;
        Real VertDist;
        Vector2 Offscreen;
        Real ScrollUpperLimit;
        Real ScrollLowerLimit;
        Real ScrollMinHeight;
        Real ButtonXAlign;
        Whole ItemsToDisplay;
        Vector2 OrigPos;
        void ResizeScrollBar();
    public:
        bool MouseScrollLock;
        ItemShopList(Real X, Real Y, Real Width, Real Height, UILayer* Layer, World* TheWorld);
        ~ItemShopList();
        void DrawList();
        void UpdateScroll(Vector2 MousePos);
        void SetDistanceBetweenButtons(Real Distance);
        void CreateListArea(Real X, Real Y, Real Width, Real Height);
        void AddItemToList(UI::Button* Item);
        void RemoveItemFromList(UI::Button* Item);
        void SetScrollBarMinHeight(Real MinHeight);
        void SetItemsToDisplay(Whole Items);
        UI::Rectangle* GetBackdrop();
        UI::Rectangle* GetListArea();
        UI::Rectangle* GetSideScrollBack();
        UI::Rectangle* GetSideScroll();
        UI::Button* GetSelectedItem();
};

#endif
