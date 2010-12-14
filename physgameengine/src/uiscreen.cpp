//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "uiscreen.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uibutton.h"
#include "world.h"
#include "internalGorilla.h.cpp"

namespace phys
{
    UIScreen::UIScreen(const String& name, Gorilla::Screen* GScreen)
        : Name(name),
          GorillaScreen(GScreen)
    {
        Manager = World::GetWorldPointer()->GetUIManager();
    }

    UIScreen::~UIScreen()
    {
        Manager->GetSilverbackPointer()->destroyScreen(GorillaScreen);
    }

    String& UIScreen::GetName()
    {
        return Name;
    }

    void UIScreen::SetVisible(bool Visible)
    {
        GorillaScreen->setVisible(Visible);
    }

    bool UIScreen::IsVisible()
    {
        return GorillaScreen->isVisible();
    }

    void UIScreen::Show()
    {
        GorillaScreen->show();
    }

    void UIScreen::Hide()
    {
        GorillaScreen->hide();
    }

    UILayer* UIScreen::CreateLayer(const String& Name, Whole Zorder)
    {
        Gorilla::Layer* layer = GorillaScreen->createLayer(Zorder);
        UILayer* physlayer = new UILayer(Name, layer, this->GorillaScreen);
        std::pair<std::map<Whole,UILayer*>::iterator,bool> TestPair = Layers.insert(std::pair<Whole,UILayer*>(Zorder,physlayer));
        if(TestPair.second)
        {
            return physlayer;
        }else{
            /// @todo add an exception here or maybe log entry, some notification it failed.
            return 0;
        }
    }

    UILayer* UIScreen::GetLayer(const String& Name)
    {
        for ( std::map<Whole,UILayer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
        {
            if ( Name == (*it).second->GetName() )
            {
                UILayer* Layer = (*it).second;
                return Layer;
            }
        }
        return 0;
    }

    UILayer* UIScreen::GetLayer(Whole Index)
    {
        std::map<Whole,UILayer*>::iterator it = Layers.begin();
        while(0 < Index)
        {
            it++;
            Index--;
        }
        if(it!=Layers.end())
            return (*it).second;
        return 0;
    }

    UILayer* UIScreen::GetLayerbyZorder(Whole Zorder)
    {
        std::map<Whole,UILayer*>::iterator it = Layers.find(Zorder);
        if(it!=Layers.end())
            return (*it).second;
        return 0;
    }

    Whole UIScreen::GetNumLayers()
    {
        return Layers.size();
    }

    void UIScreen::DestroyLayer(UILayer* Layer)
    {
        if(Layers.empty())
            return;
        for( std::map<Whole,UILayer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
        {
            if( Layer == (*it).second )
            {
                delete (*it).second;
                Layers.erase(it);
                return;
            }
        }
    }

    UI::Button* UIScreen::CheckButtonMouseIsOver()
    {
        for( std::map<Whole,UILayer*>::reverse_iterator it = Layers.rbegin() ; it != Layers.rend() ; it++ )
        {
            if( (*it).second->GetVisible() )
            {
                UI::Button* button = (*it).second->CheckButtonMouseIsOver();
                if(button)
                {
                    return button;
                }
            }
        }
        return 0;
    }

    UI::Widget* UIScreen::CheckWidgetMouseIsOver()
    {
        for( std::map<Whole,UILayer*>::reverse_iterator it = Layers.rbegin() ; it != Layers.rend() ; it++ )
        {
            if( (*it).second->GetVisible() )
            {
                UI::Widget* widget = (*it).second->CheckWidgetMouseIsOver();
                if(widget)
                {
                    return widget;
                }
            }
        }
        return 0;
    }
}

#endif
