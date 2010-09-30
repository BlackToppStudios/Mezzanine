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
#include "internalGorilla.h.cpp"

namespace phys
{
    UIScreen::UIScreen(Gorilla::Screen* GScreen, UIManager* manager)
    {
        GorillaScreen = GScreen;
        Manager = manager;
    }

    UIScreen::~UIScreen()
    {
        Manager->GetSilverbackPointer()->destroyScreen(GorillaScreen);
    }

    void UIScreen::SetVisable(bool Visable)
    {
        GorillaScreen->setVisible(Visable);
    }

    bool UIScreen::IsVisable()
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

    UILayer* UIScreen::CreateLayer(const String& Name, Whole Index)
    {
        Gorilla::Layer* layer = GorillaScreen->createLayer(Index);
        UILayer* physlayer = new UILayer(layer, this->GorillaScreen, Manager);
        GUILayers[Name] = physlayer;
        return physlayer;
    }

    UILayer* UIScreen::GetLayer(const String& Name)
    {
        return GUILayers[Name];
    }

    UILayer* UIScreen::GetLayer(Whole Index)
    {
        std::map<String,UILayer*>::iterator it = GUILayers.begin();
        for ( Whole x=0 ; x != Index ; x++ )
        {
            it++;
        }
        UILayer* Layer = (*it).second;
        return Layer;
    }

    Whole UIScreen::GetNumLayers()
    {
        return GUILayers.size();
    }

    void UIScreen::DestroyLayer(UILayer* Layer)
    {
        if(GUILayers.empty())
            return;
        for( std::map<String,UILayer*>::iterator it = GUILayers.begin() ; it != GUILayers.end() ; it++ )
        {
            if( Layer == (*it).second )
            {
                delete (*it).second;
                GUILayers.erase(it);
                return;
            }
        }
    }
}

#endif
