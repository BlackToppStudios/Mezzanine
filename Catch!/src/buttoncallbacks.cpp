#ifndef _buttoncallbacks_cpp
#define _buttoncallbacks_cpp

#include "buttoncallbacks.h"

GSStore::GSStore(UI::Button* caller) : UI::ButtonCallback(caller)
{
}

GSStore::~GSStore()
{
}

void GSStore::DoHoverItems()
{
}

void GSStore::DoActivateItems()
{
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("ItemShopLayer");
    layer->SetVisible(!layer->IsVisible());
}

GSMenu::GSMenu(UI::Button* caller) : UI::ButtonCallback(caller)
{
}

GSMenu::~GSMenu()
{
}

void GSMenu::DoHoverItems()
{
}

void GSMenu::DoActivateItems()
{
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->SetVisible(!layer->IsVisible());
}

GSReturn::GSReturn(UI::Button* caller) : UI::ButtonCallback(caller)
{
}

GSReturn::~GSReturn()
{
}

void GSReturn::DoHoverItems()
{
}

void GSReturn::DoActivateItems()
{
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->Hide();
}

#endif
