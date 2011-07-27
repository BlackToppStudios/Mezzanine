//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uidropdownlist_cpp
#define _uidropdownlist_cpp

#include "uidropdownlist.h"
#include "uilistbox.h"
#include "uicaption.h"
#include "uibutton.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"

namespace phys
{
    namespace UI
    {
        DropDownList::DropDownList(const String& name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle, Layer* parent)
            : Widget(name,parent),
              ToggleActivated(false)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            std::pair<Whole,Real> Result;
            if(Rect.Relative) Result = Manager->SuggestGlyphIndex((Whole)(LineHeight * WinDim.Y),Parent->GetParent()->GetPrimaryAtlas());
            else Result = Manager->SuggestGlyphIndex((Whole)LineHeight,Parent->GetParent()->GetPrimaryAtlas());

            ConstructDropDownList(Rect,Result.first,ScrollStyle);

            if(1.f != Result.second)
            {
                Selection->SetTextScale(Result.second);
                SelectionList->SetTemplateTextScale(Result.second);
            }
        }

        DropDownList::DropDownList(const String& name, const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle, Layer* parent)
            : Widget(name,parent),
              ToggleActivated(false)
        {
            ConstructDropDownList(Rect,Glyph,ScrollStyle);
        }

        DropDownList::~DropDownList()
        {
            delete Selection;
            delete ListToggle;
            delete SelectionList;
        }

        void DropDownList::ConstructDropDownList(const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle)
        {
            Type = Widget::DropDownList;
            RenderableRect SelectionRect, ListToggleRect, SelectionListRect;
            Real ScrollbarWidth;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();

                SelectionRect.Position = Rect.Position;
                SelectionRect.Size.X = Rect.Size.X - ((Rect.Size.Y * WinDim.Y) / WinDim.X);
                SelectionRect.Size.Y = Rect.Size.Y;
                SelectionRect.Relative = Rect.Relative;

                ListToggleRect.Position.X = Rect.Position.X + SelectionRect.Size.X;
                ListToggleRect.Position.Y = Rect.Position.Y;
                ListToggleRect.Size.X = (Rect.Size.Y * WinDim.Y) / WinDim.X;
                ListToggleRect.Size.Y = Rect.Size.Y;
                ListToggleRect.Relative = Rect.Relative;

                ScrollbarWidth = (Rect.Size.Y * WinDim.Y) / WinDim.X;
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                SelectionRect.Position = Rect.Position;
                SelectionRect.Size.X = Rect.Size.X - Rect.Size.Y;
                SelectionRect.Size.Y = Rect.Size.Y;
                SelectionRect.Relative = Rect.Relative;

                ListToggleRect.Position.X = Rect.Position.X + SelectionRect.Size.X;
                ListToggleRect.Position.Y = Rect.Position.Y;
                ListToggleRect.Size.X = Rect.Size.Y;
                ListToggleRect.Size.Y = Rect.Size.Y;
                ListToggleRect.Relative = Rect.Relative;

                ScrollbarWidth = Rect.Size.Y;
            }
            SelectionListRect.Position.X = Rect.Position.X;
            SelectionListRect.Position.Y = Rect.Position.Y + SelectionRect.Size.Y;
            SelectionListRect.Size.X = Rect.Size.X;
            SelectionListRect.Size.Y = Rect.Size.Y * 2;
            SelectionListRect.Relative = Rect.Relative;

            Selection = new Caption(Name+"Select",SelectionRect,Glyph,"",Parent);
            ListToggle = new Button(Name+"Toggle",ListToggleRect,Parent);
            SelectionList = new UI::ListBox(Name+"List",SelectionListRect,ScrollbarWidth,ScrollStyle,Parent);

            SelectionList->SetTemplateSize(Selection->GetSize(),true).SetTemplateGlyphIndex(Glyph);
        }

        void DropDownList::SetVisible(bool visible)
        {
            if(Visible==visible) return;
            Selection->SetVisible(visible);
            ListToggle->SetVisible(visible);
            if(ToggleActivated)
                SelectionList->SetVisible(visible);
            Visible = visible;
        }

        bool DropDownList::IsVisible()
        {
            return Visible;
        }

        void DropDownList::Show()
        {
            if(Visible) return;
            Selection->Show();
            ListToggle->Show();
            if(ToggleActivated)
                SelectionList->Show();
            Visible = true;
        }

        void DropDownList::Hide()
        {
            if(!Visible) return;
            Selection->Hide();
            ListToggle->Hide();
            if(ToggleActivated)
                SelectionList->Hide();
            Visible = false;
        }

        bool DropDownList::CheckMouseHover()
        {

        }

        void DropDownList::SetPosition(const Vector2& Position)
        {

        }

        Vector2 DropDownList::GetPosition()
        {
            return RelPosition;
        }

        void DropDownList::SetActualPosition(const Vector2& Position)
        {

        }

        Vector2 DropDownList::GetActualPosition()
        {
            return RelPosition / Parent->GetParent()->GetViewportDimensions();
        }

        void DropDownList::SetSize(const Vector2& Size)
        {

        }

        Vector2 DropDownList::GetSize()
        {
            return RelSize;
        }

        void DropDownList::SetActualSize(const Vector2& Size)
        {

        }

        Vector2 DropDownList::GetActualSize()
        {
            return RelSize / Parent->GetParent()->GetViewportDimensions();
        }

    }//ui
}//phys

#endif
