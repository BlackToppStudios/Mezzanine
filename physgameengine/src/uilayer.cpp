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
#ifndef _uilayer_cpp
#define _uilayer_cpp

#include "uilayer.h"
#include "uibutton.h"
#include "uirectangle.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    UILayer::UILayer(Gorilla::Layer* GLayer, Gorilla::Screen* GScreen, UIManager* manager)
    {
        GorillaLayer = GLayer;
        Parent = GScreen;
        Manager = manager;
    }

    UILayer::~UILayer()
    {
        Parent->destroy(GorillaLayer);
    }

    void UILayer::SetVisable(bool Visable)
    {
        GorillaLayer->setVisible(Visable);
    }

    bool UILayer::GetVisable()
    {
        return GorillaLayer->isVisible();
    }

    void UILayer::Show()
    {
        GorillaLayer->show();
    }

    void UILayer::Hide()
    {
        GorillaLayer->hide();
    }

    UIButton* UILayer::CreateButton(String& Name, Real X, Real Y, Real Width, Real Height, Whole Glyph, String Text)
    {
        Gorilla::Caption* GCaption = GorillaLayer->createCaption(Glyph, X, Y, Text);
        GCaption->size(Width, Height);
        UIButton* Button = new UIButton(Name, GCaption, GorillaLayer, Manager);
        Buttons.push_back(Button);
        return Button;
    }

    UIRectangle* UILayer::CreateRectangle(Real X, Real Y, Real Width, Real Height)
    {
        Gorilla::Rectangle* GRectangle = GorillaLayer->createRectangle(X, Y, Width, Height);
        UIRectangle* Rectangle = new UIRectangle(GRectangle, GorillaLayer, Manager);
        return Rectangle;
    }

    UIButton* UILayer::GetButtonMouseIsOver(Whole MouseX, Whole MouseY)
    {
        UIButton* Button = NULL;
        for( std::vector<UIButton*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
        {
            Button = (*it);
            if(Button->MouseIsOver(MouseX, MouseY))
            {
                return Button;
            }
        }
        return 0;
    }
}//phys

#endif
