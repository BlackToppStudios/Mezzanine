#ifndef _itemshoplist_cpp
#define _itemshoplist_cpp

#include "itemshoplist.h"

ItemShopList::ItemShopList(Real X, Real Y, Real Width, Real Height, UILayer* Layer, World* TheWorld)
{
    GraphMan = TheWorld->GetGraphicsManager();
    UIMan = TheWorld->GetUIManager();
    ParentLayer = Layer;
    Whole WWidth = GraphMan->getRenderWidth();
    Whole WHeight = GraphMan->getRenderHeight();
    Backdrop = Layer->CreateRectangle(WWidth*X,WHeight*Y,WWidth*Width,WHeight*Height);
    ListArea = NULL;
    SideScroll = NULL;
    Selected = NULL;
    Offscreen.X = WWidth * 0.5;
    Offscreen.Y = WHeight * 1.5;
    ScrollMinHeight = 0;
}

ItemShopList::~ItemShopList()
{
    ItemList.clear();
}

void ItemShopList::ResizeScrollBar()
{
    Whole Items = ItemList.size();
    Real Bar = 0;
    Vector2 Size = SideScroll->GetSize();
    if(ItemsToDisplay>=Items)
    {
        Bar = ScrollLowerLimit - ScrollUpperLimit;
        if(Bar<ScrollMinHeight)
        {
            Bar = ScrollMinHeight;
        }
        Vector2 NewSize(Size.X,Bar);
        SideScroll->SetSize(NewSize);
    }else{
        Bar = ItemsToDisplay/Items;
        Bar = Bar * (ScrollLowerLimit - ScrollUpperLimit);
        if(Bar<ScrollMinHeight)
        {
            Bar = ScrollMinHeight;
        }
        Vector2 NewSize(Size.X,Bar);
        SideScroll->SetSize(NewSize);
    }
}

void ItemShopList::DrawList()
{
    if(ItemList.empty())
        return;
    for( std::vector<UIButton*>::iterator it = ItemList.begin() ; it != ItemList.end() ; it++ )
    {
        (*it)->SetPosition(Offscreen);
    }
    Whole TopItem = 0;
    Whole ItemLoop = ItemList.size();
    if(ItemList.size()>ItemsToDisplay)
    {
        //Determine what buttons will be visable.
        Real ScrollSpace = (ScrollLowerLimit - ScrollUpperLimit) - SideScroll->GetSize().Y;
        Real ScrollPos = (SideScroll->GetPosition().Y) - ScrollUpperLimit;
        Real Pos = ScrollPos/ScrollSpace;
        TopItem = (Whole)(ItemsToDisplay * Pos);
        ItemLoop = ItemsToDisplay;
    }
    //place as necessary
    Real YPos = (ListArea->GetPosition().Y) + VertDist;
    for( Whole x=0 ; x != ItemLoop ; x++, TopItem++)
    {
        UIButton* TempButton = ItemList[TopItem];
        YPos+=(VertDist+(ItemList[TopItem-1]->GetSize().Y));
        Vector2 Position(ButtonXAlign,YPos);
        TempButton->SetPosition(Position);
    }
}

void ItemShopList::UpdateScroll(Vector2 MousePos)
{
    if(MouseScrollLock)
    {
        Real Diff = OrigPos.Y - MousePos.Y;
        Vector2 Scroll = SideScroll->GetPosition();
        if(ScrollUpperLimit<=(Scroll.Y + Diff))
        {
            SideScroll->SetPosition(Vector2(Scroll.X,ScrollUpperLimit));
        }
        else if(ScrollLowerLimit>=(Scroll.Y + Diff))
        {
            SideScroll->SetPosition(Vector2(Scroll.X,ScrollLowerLimit));
        }
        else
        {
            SideScroll->SetPosition(Vector2(Scroll.X,(Scroll.Y + Diff)));
        }
    }
}

void ItemShopList::SetDistanceBetweenButtons(Real Distance)
{
    VertDist = Distance;
}

void ItemShopList::CreateListArea(Real X, Real Y, Real Width, Real Height)
{
    Whole WWidth = GraphMan->getRenderWidth();
    Whole WHeight = GraphMan->getRenderHeight();
    ListArea = ParentLayer->CreateRectangle(WWidth*X,WHeight*Y,WWidth*Width,WHeight*Height);
    Vector2 LAP = ListArea->GetPosition();
    Vector2 LAS = ListArea->GetSize();
    SideScrollBack = ParentLayer->CreateRectangle((LAS.X * 0.94)+LAP.X, (LAS.Y * 0.04)+LAP.Y, LAS.X * 0.05, LAS.Y * 0.94);
    Vector2 SSBP = SideScrollBack->GetPosition();
    Vector2 SSBS = SideScrollBack->GetSize();
    SideScroll = ParentLayer->CreateRectangle((SSBS.X * 0.005)+SSBP.X, (SSBS.Y * 0.005)+SSBP.Y, SSBS.X * 0.95, SSBS.Y * 0.95);
    ScrollUpperLimit = SideScroll->GetPosition().Y;
    ScrollLowerLimit = (SideScroll->GetPosition().Y) + (SideScroll->GetSize().Y);
    ButtonXAlign = (ListArea->GetPosition().X) + (ListArea->GetSize().X * 0.05);
}

void ItemShopList::AddItemToList(UIButton* Item)
{
    ItemList.push_back(Item);
    ResizeScrollBar();
}

void ItemShopList::RemoveItemFromList(UIButton* Item)
{
    for( std::vector<UIButton*>::iterator it = ItemList.begin() ; it != ItemList.end() ; it++ )
    {
        if(Item == (*it))
        {
            ItemList.erase(it);
            break;
        }
    }
    ResizeScrollBar();
}

void ItemShopList::SetScrollBarMinHeight(Real MinHeight)
{
    ScrollMinHeight = MinHeight;
}

void ItemShopList::SetItemsToDisplay(Whole Items)
{
    ItemsToDisplay = Items;
}

UI::Rectangle* ItemShopList::GetBackdrop()
{
    return Backdrop;
}

UI::Rectangle* ItemShopList::GetListArea()
{
    return ListArea;
}

UI::Rectangle* ItemShopList::GetSideScrollBack()
{
    return SideScrollBack;
}

UI::Rectangle* ItemShopList::GetSideScroll()
{
    return SideScroll;
}

UIButton* ItemShopList::GetSelectedItem()
{
    return Selected;
}

#endif
