// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _uilistbox_cpp
#define _uilistbox_cpp

#include "UI/listbox.h"
#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "Input/inputmanager.h"
#include "Input/metacode.h"
#include "Input/mouse.h"
#include "entresol.h"

#include <cmath>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Static Members

        const String ListBox::TypeName = "ListBox";

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Methods

        ListBox::ListBox(Screen* Parent) :
            Widget(Parent)
        {

        }

        ListBox::ListBox(const String& RendName, Screen* Parent) :
            Widget(RendName,Parent)
        {

        }

        ListBox::ListBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Widget(RendName,RendRect,Parent)
        {

        }

        ListBox::ListBox(const XML::Node& XMLNode, Screen* Parent) :
            Widget(Parent)
        {

        }

        ListBox::~ListBox()
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Properties

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Configuration

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ListBox::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);
        }

        void ListBox::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);
        }

        String ListBox::GetSerializableName()
        {
            return ListBox::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods










        /*ListBox::ListBox(ConstString& name, const Rect& RendRect, const UI::ScrollbarStyle& ScrollStyle, Screen* PScreen) :
            Widget(name,PScreen),
            Selected(NULL),
            AutoHideScroll(true),
            LastScrollValue(0),
            MaxDisplay(3),
            SelectionsAdded(1)
        {
            /// @todo Currently this class has little support for a border around the selections.
            /// Ideally when the UI system is more complete we'll be able to seemlessly move
            /// objects around different layers and thus have more control over z-order.  When
            /// that happens we should add a rect to this class be be placed over the selections
            /// for use with a border or other kind of decoration.
            Type = Widget::W_ListBox;

            // Set some sane template defaults
            SelectionTemplate.BackgroundColour = ColourValue(1.0,1.0,1.0,1.0);
            SelectionTemplate.TextColour = ColourValue(0.0,0.0,0.0,1.0);
            SelectionTemplate.TextScale = 1.0;
            SelectionTemplate.CursorOffset = 0.0;
            SelectionTemplate.HorizontalAlign = UI::Txt_Middle;
            SelectionTemplate.VerticalAlign = UI::Txt_Center;
            SelectionTemplate.Priority = UI::RP_Medium;

            Rect ScrollRect, BoxRect;
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            if(RendRect.Relative)
            {
                RelPosition = RendRect.Position;
                RelSize = RendRect.Size;

                SelectionTemplate.Size = RendRect.Size * WinDim;

                ScrollRect.Position = Vector2((RelPosition.X + RelSize.X) - ((RendRect.Size.Y * WinDim.Y) / WinDim.X),RelPosition.Y);
                ScrollRect.Size = Vector2((RendRect.Size.Y * WinDim.Y) / WinDim.X,RelSize.Y * MaxDisplay);
                ScrollRect.Relative = RendRect.Relative;
            }else{
                RelPosition = RendRect.Position / WinDim;
                RelSize = RendRect.Size / WinDim;

                SelectionTemplate.Size = RendRect.Size;

                ScrollRect.Position = Vector2((RendRect.Position.X + RendRect.Size.X) - RendRect.Size.Y,RendRect.Position.Y);
                ScrollRect.Size = Vector2(RendRect.Size.Y,RendRect.Size.Y * MaxDisplay);
                ScrollRect.Relative = RendRect.Relative;
            }
            BoxRect.Position = RendRect.Position;
            BoxRect.Size.X = RendRect.Size.X;
            BoxRect.Size.Y = RendRect.Size.Y * MaxDisplay;
            BoxRect.Relative = RendRect.Relative;

            BoxBack = ParentScreen->CreateRectangle(BoxRect);
            VertScroll = ParentScreen->CreateScrollbar(Name+"Scr",ScrollRect,ScrollStyle);
            VertScroll->Hide();

            AddSubRenderable(0,BoxBack);
            AddSubRenderable(1,VertScroll);
        }

        ListBox::~ListBox()
        {
            ParentScreen->DestroyBasicRenderable(BoxBack);
            if(!Selections.empty())
            {
                for( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
                {
                    ParentScreen->DestroyBasicRenderable( *it );
                }
                Selections.clear();
            }
            VisibleSelections.clear();
        }

        void ListBox::ScrollerSizeCheck()
        {
            Real ScrollerSize = (Real)MaxDisplay / (Real)Selections.size();
            VertScroll->SetScrollerSize(ScrollerSize);
        }

        void ListBox::ScrollHideCheck()
        {
            if(!IsVisible())
            {
                VertScroll->Hide();
                return;
            }
            if(!AutoHideScroll)
            {
                VertScroll->Show();
                return;
            }
            if(Selections.size() > MaxDisplay)
                VertScroll->Show();
            else
                VertScroll->Hide();
        }

        void ListBox::SelectionSizeCheck(UI::Caption* Selection)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            Vector2 CurrSize = Selection->GetActualSize();
            Vector2 TargetSize;
            if(VertScroll->IsVisible())
                TargetSize = Vector2(SelectionTemplate.Size.X - VertScroll->GetActualSize().X,SelectionTemplate.Size.Y);
            else
                TargetSize = SelectionTemplate.Size;
            if(CurrSize != TargetSize)
            {
                Selection->SetActualSize(TargetSize);
            }
        }

        ListBox& ListBox::SetTemplateSize(const Vector2& Size, bool Relative)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            if(Relative)
            {
                this->SelectionTemplate.Size = Size * WinDim;
                Vector2 NewSize = Vector2(Size.X,Size.Y * MaxDisplay);
                SetArea(NewSize * WinDim);
            }else{
                this->SelectionTemplate.Size = Size;
                Vector2 NewSize = Vector2(Size.X,Size.Y * MaxDisplay);
                SetArea(NewSize);
            }
            return *this;
        }

        ListBox& ListBox::SetTemplateGlyphIndex(const Whole& Glyph)
        {
            this->SelectionTemplate.GlyphIndex = Glyph;
            return *this;
        }

        ListBox& ListBox::SetTemplateTextColour(const ColourValue& TextColour)
        {
            this->SelectionTemplate.TextColour = TextColour;
            return *this;
        }

        ListBox& ListBox::SetTemplateTextScale(const Real& Scale)
        {
            this->SelectionTemplate.TextScale = Scale;
            return *this;
        }

        ListBox& ListBox::SetTemplateCursorOffset(const Whole& Offset)
        {
            this->SelectionTemplate.CursorOffset = Offset;
            return *this;
        }

        ListBox& ListBox::SetTemplateBackgroundColour(const ColourValue& BackgroundColour)
        {
            this->SelectionTemplate.BackgroundColour = BackgroundColour;
            return *this;
        }

        ListBox& ListBox::SetTemplateHorizontalAlign(const UI::TextHorizontalAlign& HorAlign)
        {
            this->SelectionTemplate.HorizontalAlign = HorAlign;
            return *this;
        }

        ListBox& ListBox::SetTemplateVerticalAlign(const UI::TextVerticalAlign& VertAlign)
        {
            this->SelectionTemplate.VerticalAlign = VertAlign;
            return *this;
        }

        ListBox& ListBox::SetTemplateRenderPriority(const UI::RenderPriority& Priority)
        {
            this->SelectionTemplate.Priority = Priority;
            return *this;
        }

        const UI::TemplateParams& ListBox::GetTemplateInfo()
        {
            return this->SelectionTemplate;
        }

        Caption* ListBox::AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite)
        {
            SelectionsAdded++;
            Rect SelectionRect(RelPosition,SelectionTemplate.Size / ParentScreen->GetViewportDimensions(),true);
            Caption* Select = ParentScreen->CreateCaption(name,SelectionRect,SelectionTemplate.GlyphIndex,Text);
            if(!BackgroundSprite.empty())
                Select->SetBackgroundSprite(BackgroundSprite);
            if(SelectionTemplate.BackgroundColour != ColourValue(1.0,1.0,1.0,1.0))
                Select->SetBackgroundColour(SelectionTemplate.BackgroundColour);
            if(SelectionTemplate.CursorOffset != 0)
                Select->SetCursorOffset(SelectionTemplate.CursorOffset);
            if(SelectionTemplate.TextScale != 1)
                Select->SetTextScale(SelectionTemplate.TextScale);
            Select->SetTextColour(SelectionTemplate.TextColour);
            Select->HorizontallyAlign(SelectionTemplate.HorizontalAlign);
            Select->VerticallyAlign(SelectionTemplate.VerticalAlign);
            Select->SetRenderPriority(SelectionTemplate.Priority);
            Select->Hide();
            Selections.push_back(Select);
            AddSubRenderable(SelectionsAdded,Select);
            ScrollerSizeCheck();
            DrawList();
            return Select;
        }

        Caption* ListBox::GetSelection(ConstString &Name)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    UI::Caption* button = (*it);
                    return button;
                }
            }
            return 0;
        }

        void ListBox::DestroySelection(Caption* ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    Selections.erase(it);
                    break;
                }
            }
            for ( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it) == ToBeDestroyed )
                {
                    SubRenderables.erase(it);
                    break;
                }
            }
            ParentScreen->DestroyBasicRenderable(ToBeDestroyed);
        }//*/
    }//UI
}//Mezzanine

#endif
