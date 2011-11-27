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
#ifndef _uilinelist_cpp
#define _uilinelist_cpp

#include "uilinelist.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        LineList::LineList(Layer* PLayer)
            : Parent(PLayer)
        {
            Manager = UIManager::GetSingletonPtr();

            GLineList = Parent->GetGorillaLayer()->createLineList();
        }

        LineList::~LineList()
        {
            Parent->GetGorillaLayer()->destroyLineList(GLineList);
        }

        void LineList::SetVisible(bool Visible)
        {
            GLineList->SetVisible(Visible);
        }

        bool LineList::IsVisible()
        {
            return GLineList->IsVisible();
        }

        void LineList::Show()
        {
            GLineList->Show();
        }

        void LineList::Hide()
        {
            GLineList->Hide();
        }

        void LineList::Begin(const Whole& Thickness, const ColourValue& Colour)
        {
            GLineList->begin((Real)Thickness, Colour.GetOgreColourValue());
        }

        void LineList::AddPoint(const Real& X, const Real& Y)
        {
            AddPoint(Vector2(X,Y));
        }

        void LineList::AddPoint(const Vector2& Position)
        {
            GLineList->position((Position * Parent->GetParent()->GetViewportDimensions()).GetOgreVector2());
        }

        void LineList::AddActualPoint(const Real& X, const Real& Y)
        {
            AddActualPoint(Vector2(X,Y));
        }

        void LineList::AddActualPoint(const Vector2& Position)
        {
            GLineList->position(Position.GetOgreVector2());
        }

        void LineList::End(bool Closed)
        {
            GLineList->end(Closed);
        }

        void LineList::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            Gorilla::RenderPriority RP;
            switch(Priority)
            {
                case UI::RP_Low:
                    RP = Gorilla::RP_Low;
                    break;
                case UI::RP_Medium:
                    RP = Gorilla::RP_Medium;
                    break;
                case UI::RP_High:
                    RP = Gorilla::RP_High;
                    break;
                default:
                    break;
            }
            GLineList->RenderPriority(RP);
        }

        UI::RenderPriority LineList::GetRenderPriority()
        {
            Gorilla::RenderPriority RP = this->GLineList->RenderPriority();
            switch(RP)
            {
                case Gorilla::RP_Low:
                    return UI::RP_Low;
                    break;
                case Gorilla::RP_Medium:
                    return UI::RP_Medium;
                    break;
                case Gorilla::RP_High:
                    return UI::RP_High;
                    break;
                default:
                    break;
            }
            return UI::RP_Medium;
        }
    }//UI
}//phys

#endif
