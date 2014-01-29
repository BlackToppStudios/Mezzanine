#ifndef _itemshopcell_cpp
#define _itemshopcell_cpp

#include "itemshopcell.h"

/*ItemShopCell::ItemShopCell(const String& name, const UI::RenderableRect& Rect, const String& ItemName, UI::Screen* parent)
    : UI::Cell(name,parent)
{
    Real LineHeight = Rect.Relative ? Rect.Size.Y * 0.25 : (Rect.Size.Y / ParentScreen->GetViewportDimensions().Y) * 0.25;
    ItemCaption = ParentScreen->CreateCaption(name,Rect,LineHeight,ItemName);
    AddSubRenderable(0,ItemCaption);
}

ItemShopCell::~ItemShopCell()
{
    ParentScreen->DestroyBasicRenderable(ItemCaption);
}

void ItemShopCell::UpdateImpl(bool Force)
{

}

void ItemShopCell::SetVisibleImpl(bool visible)
{
    ItemCaption->SetVisible(visible);
}

bool ItemShopCell::CheckMouseHoverImpl()
{
    if(ItemCaption->CheckMouseHover()) return true;
    else return false;
}

void ItemShopCell::SetPosition(const Vector2& Position)
{
    RelSize = Position;
    ItemCaption->SetPosition(Position);
}

void ItemShopCell::SetActualPosition(const Vector2& Position)
{
    RelSize = Position / ParentScreen->GetViewportDimensions();
    ItemCaption->SetActualPosition(Position);
}

void ItemShopCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    ItemCaption->SetSize(Size);
}

void ItemShopCell::SetActualSize(const Vector2& Size)
{
    RelSize = Size / ParentScreen->GetViewportDimensions();
    ItemCaption->SetActualSize(Size);
}

void ItemShopCell::UpdateDimensions()
{
    UI::WidgetResult Result = UI::ViewportUpdateTool::UpdateWidget(this);
    RelPosition = Result.first / UI::ViewportUpdateTool::GetNewSize();
    RelSize = Result.second / UI::ViewportUpdateTool::GetNewSize();
    ItemCaption->UpdateDimensions();
    SetPosition(RelPosition);
}

UI::Caption* ItemShopCell::GetItemCaption()
{
    return ItemCaption;
}

ItemShopCB::ItemShopCB()
{

}

ItemShopCB::~ItemShopCB()
{

}

void ItemShopCB::SetCaller(UI::Cell* Caller)
{

}

void ItemShopCB::DoSelectedItems()
{

}

void ItemShopCB::DoUnselectedItems()
{

}
//*/

#endif
