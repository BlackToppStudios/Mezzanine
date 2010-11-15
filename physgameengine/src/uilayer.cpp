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
#include "uicaption.h"
#include "uiwidget.h"

#include "graphicsmanager.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    UILayer::UILayer(const String& name, Gorilla::Layer* GLayer, Gorilla::Screen* GScreen, UIManager* manager)
        : Name(name)
    {
        GorillaLayer = GLayer;
        Parent = GScreen;
        Manager = manager;
    }

    UILayer::~UILayer()
    {
        Parent->destroy(GorillaLayer);
    }

    String& UILayer::GetName()
    {
        return Name;
    }

    void UILayer::SetVisible(bool Visable)
    {
        GorillaLayer->setVisible(Visable);
    }

    bool UILayer::GetVisible()
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

    UI::Button* UILayer::CreateButton(String& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text)
    {
        GraphicsManager* Graphics = World::GetWorldPointer()->GetGraphicsManager();
        Real Width = (Real)Graphics->getRenderWidth();
        Real Height = (Real)Graphics->getRenderHeight();
        Gorilla::Caption* GCaption = GorillaLayer->createCaption(Glyph, Position.X * Width, Position.Y * Height, Text);
        GCaption->size(Size.X * Width, Size.Y * Height);
        UI::Button* button = new UI::Button(Name, GCaption, this);
        Buttons.push_back(button);
        return button;
    }

    UI::Button* UILayer::GetButton(String& Name)
    {
        for ( std::vector<UI::Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UI::Button* button = (*it);
                return button;
            }
        }
        return 0;
    }

    UI::Button* UILayer::GetButton(Whole Index)
    {
        return Buttons[Index];
    }

    Whole UILayer::GetNumButtons()
    {
        return Buttons.size();
    }

    UI::Rectangle* UILayer::CreateRectangle(Vector2 Position, Vector2 Size)
    {
        GraphicsManager* Graphics = World::GetWorldPointer()->GetGraphicsManager();
        Real Width = (Real)Graphics->getRenderWidth();
        Real Height = (Real)Graphics->getRenderHeight();
        Gorilla::Rectangle* GRectangle = GorillaLayer->createRectangle(Position.X * Width, Position.Y * Height, Size.X * Width, Size.Y * Height);
        UI::Rectangle* rectangle = new UI::Rectangle(GRectangle, this);
        Rectangles.push_back(rectangle);
        return rectangle;
    }

    UI::Rectangle* UILayer::GetRectangle(Whole Index)
    {
        return Rectangles[Index];
    }

    Whole UILayer::GetNumRectangles()
    {
        return Rectangles.size();
    }

    UI::Caption* UILayer::CreateCaption(String& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text)
    {
        GraphicsManager* Graphics = World::GetWorldPointer()->GetGraphicsManager();
        Real Width = (Real)Graphics->getRenderWidth();
        Real Height = (Real)Graphics->getRenderHeight();
        Gorilla::Caption* GCaption = GorillaLayer->createCaption(Glyph, Position.X * Width, Position.Y * Height, Text);
        GCaption->size(Size.X * Width, Size.Y * Height);
        UI::Caption* caption = new UI::Caption(Name, GCaption, this);
        Captions.push_back(caption);
        return caption;
    }

    UI::Caption* UILayer::GetCaption(String& Name)
    {
        for ( std::vector<UI::Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UI::Caption* caption = (*it);
                return caption;
            }
        }
        return 0;
    }

    UI::Caption* UILayer::GetCaption(Whole Index)
    {
        return Captions[Index];
    }

    Whole UILayer::GetNumCaptions()
    {
        return Captions.size();
    }

    UI::Widget* UILayer::GetWidget(String& Name)
    {
        for ( std::vector<UI::Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UI::Widget* widget = (*it);
                return widget;
            }
        }
        return 0;
    }

    UI::Widget* UILayer::GetWidget(Whole Index)
    {
        return Widgets[Index];
    }

    Whole UILayer::GetNumWidgets()
    {
        return Widgets.size();
    }

    UI::Button* UILayer::CheckButtonMouseIsOver()
    {
        if(Buttons.empty())
            return 0;
        UI::Button* button = NULL;
        for( std::vector<UI::Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
        {
            button = (*it);
            if(button->CheckMouseHover())
            {
                return button;
            }
        }
        return 0;
    }

    UI::Widget* UILayer::CheckWidgetMouseIsOver()
    {
        if(Widgets.empty())
            return 0;
        UI::Widget* widget = NULL;
        for( std::vector<UI::Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
        {
            widget = (*it);
            if(widget->CheckMouseHover())
            {
                return widget;
            }
        }
        return 0;
    }

    Gorilla::Layer* UILayer::GetGorillaLayer()
    {
        return GorillaLayer;
    }
}//phys

#endif
