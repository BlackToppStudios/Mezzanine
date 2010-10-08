#ifndef _itemshoplist_cpp
#define _itemshoplist_cpp

#include "itemshoplist.h"

ItemShopList::ItemShopList(Real X, Real Y, Real Width, Real Height, UILayer* Layer, World* TheWorld)
{
    GraphMan = TheWorld->GetGraphicsManager();
    UIMan = TheWorld->GetUIManager();
    ParentLayer = Layer;
    Whole WWidth = UIMan->getRenderWidth();
    Whole WHeight = UIMan->getRenderHeight();
    Backdrop = Layer->CreateRectangle(WWidth*X,WHeight*Y,WWidth*Width,WHeight*Height);
    ListArea = NULL;
    Selected = NULL;
    OffscreenX = WWidth * 0.5;
    OffscreenY = WHeight * 1.5;
}

ItemShopList::~ItemShopList()
{
    ItemList.clear();
}

void ItemShopList::SetDistanceBetweenButtons(Real Distance)
{
    VertDist = Distance;
}

void ItemShopList::CreateListArea(Real X, Real Y, Real Width, Real Height)
{
    Whole WWidth = UIMan->getRenderWidth();
    Whole WHeight = UIMan->getRenderHeight();
    ListArea = ParentLayer->CreateRectangle(WWidth*X,WHeight*Y,WWidth*Width,WHeight*Height);
}

void ItemShopList::AddItemToList(UIButton* Item)
{
    ItemList.push_back(Item);
}

UIRectangle* ItemShopList::GetBackdrop()
{
    return Backdrop;
}

UIRectangle* ItemShopList::GetListArea()
{
    return ListArea;
}

UIButton* ItemShopList::GetSelectedItem()
{
    return Selected;
}

#endif
