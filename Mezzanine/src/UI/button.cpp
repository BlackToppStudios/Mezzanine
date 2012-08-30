//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _uibutton_cpp
#define _uibutton_cpp

#include "uimanager.h"
#include "UI/button.h"
#include "UI/caption.h"
#include "UI/screen.h"
#include "UI/viewportupdatetool.h"
#include "eventmanager.h"
#include "entresol.h"

namespace Mezzanine
{
    namespace UI
    {
        Button::Button(ConstString& name, const RenderableRect& Rect, Screen* PScreen)
            : Activatable(name,PScreen)
        {
            Type = Widget::W_Button;
            Real LineHeight = 0.0;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
                LineHeight = Rect.Size.Y * 0.8;
            }else{
                RelPosition = Rect.Position / ParentScreen->GetViewportDimensions();
                RelSize = Rect.Size / ParentScreen->GetViewportDimensions();
                LineHeight = (Rect.Size.Y / ParentScreen->GetViewportDimensions().Y) * 0.8;
            }
            Clickable = ParentScreen->CreateCaption(Name+"Cl",Rect,LineHeight,"");
            AddSubRenderable(0,Clickable);
        }

        Button::Button(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Screen* PScreen)
            : Activatable(name,PScreen)
        {
            Type = Widget::W_Button;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / ParentScreen->GetViewportDimensions();
                RelSize = Rect.Size / ParentScreen->GetViewportDimensions();
            }
            Clickable = ParentScreen->CreateCaption(Name+"Cl",Rect,Glyph,Text);
            AddSubRenderable(0,Clickable);
        }

        Button::Button(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Screen* PScreen)
            : Activatable(name,PScreen)
        {
            Type = Widget::W_Button;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / ParentScreen->GetViewportDimensions();
                RelSize = Rect.Size / ParentScreen->GetViewportDimensions();
            }
            Clickable = ParentScreen->CreateCaption(Name+"Cl",Rect,LineHeight,Text);
            AddSubRenderable(0,Clickable);
        }

        Button::~Button()
        {
            ParentScreen->DestroyBasicRenderable(Clickable);
        }

        void Button::UpdateImpl(bool Force)
        {
        }

        void Button::SetVisibleImpl(bool visible)
        {
            Clickable->SetVisible(visible);
        }

        bool Button::CheckMouseHoverImpl()
        {
            return Clickable->CheckMouseHover();
        }

        void Button::SetLocation(const Vector2& Location)
        {
            Clickable->SetActualPosition(Location);
        }

        void Button::SetArea(const Vector2& Area)
        {
            Clickable->SetActualSize(Area);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void Button::UpdateDimensions()
        {
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            Clickable->UpdateDimensions();
            SetPosition(RelPosition);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void Button::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ActPos = Position * ParentScreen->GetViewportDimensions();
            SetLocation(ActPos);
        }

        void Button::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / ParentScreen->GetViewportDimensions();
            SetLocation(Position);
        }

        void Button::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Vector2 ActSize = Size * ParentScreen->GetViewportDimensions();
            SetArea(ActSize);
        }

        void Button::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / ParentScreen->GetViewportDimensions();
            SetArea(Size);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Caption* Button::GetClickable() const
        {
            return Clickable;
        }
    }//UI
}//Mezzanine

#endif
