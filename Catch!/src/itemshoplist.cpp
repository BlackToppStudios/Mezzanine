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
    SideScroll = NULL;
    Selected = NULL;
    Offscreen.X = WWidth * 0.5;
    Offscreen.Y = WHeight * 1.5;
}

ItemShopList::~ItemShopList()
{
    ItemList.clear();
}

void ItemShopList::DrawList()
{
    Real RelativeX;
    Real RelativeY;
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
    Vector2 LAP = ListArea->GetPosition();
    Vector2 LAS = ListArea->GetSize();
    SideScrollBack = ParentLayer->CreateRectangle(LAP.X * 0.94, LAP.Y * 0.4, LAS.X * 0.05, LAS.Y * 0.95);
    Vector2 SSBP = SideScrollBack->GetPosition();
    Vector2 SSBS = SideScrollBack->GetSize();
    SideScroll = ParentLayer->CreateRectangle(SSBP * 0.005, SSBP * 0.005, SSBS.X * 0.95, SSBS.Y * 0.95);
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

UIRectangle* ItemShopList::GetSideScrollBack()
{
    return SideScrollBack;
}

UIRectangle* ItemShopList::GetSideScroll()
{
    return SideScroll;
}

UIButton* ItemShopList::GetSelectedItem()
{
    return Selected;
}

#endif
