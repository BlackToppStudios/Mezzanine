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
#ifndef _uirenderablecontainerwidget_cpp
#define _uirenderablecontainerwidget_cpp

#include "UI/renderablecontainerwidget.h"
#include "uimanager.h"
#include "UI/screen.h"
#include "UI/caption.h"
#include "UI/markuptext.h"
#include "UI/linelist.h"
#include "UI/rectangle.h"
#include "UI/button.h"
#include "UI/scrollbar.h"
#include "UI/listbox.h"
#include "UI/checkbox.h"
#include "UI/spinner.h"
#include "UI/scrolledcellgrid.h"
#include "UI/pagedcellgrid.h"
#include "UI/dropdownlist.h"
#include "UI/tabset.h"
#include "UI/viewportupdatetool.h"
#include "Input/metacode.h"
#include "entresol.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // RenderableContainerWidget Methods

        RenderableContainerWidget::RenderableContainerWidget(const String& name, Screen* Parent)
            : Widget(name,Parent),
              RenderablesAdded(0)
        {
            Type = Widget::W_GenericWidgetContainer;
        }

        RenderableContainerWidget::~RenderableContainerWidget()
        {
        }

        void RenderableContainerWidget::UpdateImpl(bool Force)
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // OpenRenderableContainerWidget Methods

        OpenRenderableContainerWidget::OpenRenderableContainerWidget(const String& name, Screen* Parent)
            : RenderableContainerWidget(name,Parent)
        {
            RelPosition.SetValues(0,0);
            RelSize.SetValues(1,1);
        }

        OpenRenderableContainerWidget::~OpenRenderableContainerWidget()
        {
            DestroyAllWidgets();
            DestroyAllAreaRenderables();
        }

        void OpenRenderableContainerWidget::SetVisibleImpl(bool visible)
        {
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                (*ArIt)->SetVisible(visible);
            }
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                (*WidIt)->SetVisible(visible);
            }
        }

        bool OpenRenderableContainerWidget::CheckMouseHoverImpl()
        {
            if(HoveredSubWidget)
            {
                if(HoveredSubWidget->CheckMouseHover())
                    return true;
            }
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                if( (*ArIt)->CheckMouseHover() )
                    return true;
            }
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; WidIt++ )
            {
                if( (*WidIt)->CheckMouseHover() )
                {
                    HoveredSubWidget = (*WidIt);
                    return true;
                }
            }//*/
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating BasicRenderables

        Rectangle* OpenRenderableContainerWidget::CreateRectangle(const RenderableRect& Rect)
        {
            ++RenderablesAdded;
            Rectangle* renderable = ParentScreen->CreateRectangle(Rect);
            AreaRenderables.push_back(renderable);
            AddSubRenderable(RenderablesAdded,renderable);
            return renderable;
        }

        Caption* OpenRenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            ++RenderablesAdded;
            Caption* renderable = ParentScreen->CreateCaption(Name,Rect,Glyph,Text);
            AreaRenderables.push_back(renderable);
            AddSubRenderable(RenderablesAdded,renderable);
            return renderable;
        }

        Caption* OpenRenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            ++RenderablesAdded;
            Caption* renderable = ParentScreen->CreateCaption(Name,Rect,LineHeight,Text);
            AreaRenderables.push_back(renderable);
            AddSubRenderable(RenderablesAdded,renderable);
            return renderable;
        }

        MarkupText* OpenRenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            ++RenderablesAdded;
            MarkupText* renderable = ParentScreen->CreateMarkupText(Name,Rect,Glyph,Text);
            AreaRenderables.push_back(renderable);
            AddSubRenderable(RenderablesAdded,renderable);
            return renderable;
        }

        MarkupText* OpenRenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            ++RenderablesAdded;
            MarkupText* renderable = ParentScreen->CreateMarkupText(Name,Rect,LineHeight,Text);
            AreaRenderables.push_back(renderable);
            AddSubRenderable(RenderablesAdded,renderable);
            return renderable;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with All AreaRenderables

        AreaRenderable* OpenRenderableContainerWidget::GetAreaRenderable(ConstString& Name)
        {
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                if( Name == (*ArIt)->GetName() )
                {
                    return (*ArIt);
                }
            }
        }

        AreaRenderable* OpenRenderableContainerWidget::GetAreaRenderable(const Whole& Index)
        {
            return AreaRenderables.at(Index);
        }

        Whole OpenRenderableContainerWidget::GetNumAreaRenderables()
        {
            return AreaRenderables.size();
        }

        void OpenRenderableContainerWidget::DestroyAreaRenderable(AreaRenderable* ToBeDestroyed)
        {
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                if( (*ArIt) == ToBeDestroyed )
                {
                    ParentScreen->DestroyBasicRenderable( (*ArIt) );
                    AreaRenderables.erase(ArIt);
                    return;
                }
            }
        }

        void OpenRenderableContainerWidget::DestroyAllAreaRenderables()
        {
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                ParentScreen->DestroyBasicRenderable( (*ArIt) );
            }
            AreaRenderables.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Widgets

        Button* OpenRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            ++RenderablesAdded;
            Button* widget = ParentScreen->CreateButton(Name,Rect);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        Button* OpenRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            ++RenderablesAdded;
            Button* widget = ParentScreen->CreateButton(Name,Rect,Glyph,Text);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        Button* OpenRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            ++RenderablesAdded;
            Button* widget = ParentScreen->CreateButton(Name,Rect,LineHeight,Text);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        Scrollbar* OpenRenderableContainerWidget::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            ++RenderablesAdded;
            Scrollbar* widget = ParentScreen->CreateScrollbar(Name,Rect,Style);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        CheckBox* OpenRenderableContainerWidget::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
        {
            ++RenderablesAdded;
            CheckBox* widget = ParentScreen->CreateCheckBox(Name,Rect,LineHeight,LabelText);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        ListBox* OpenRenderableContainerWidget::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
        {
            ++RenderablesAdded;
            ListBox* widget = ParentScreen->CreateListBox(Name,Rect,ScrollStyle);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        Spinner* OpenRenderableContainerWidget::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            ++RenderablesAdded;
            Spinner* widget = ParentScreen->CreateSpinner(Name,Rect,SStyle,GlyphHeight);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        ScrolledCellGrid* OpenRenderableContainerWidget::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            ++RenderablesAdded;
            ScrolledCellGrid* widget = ParentScreen->CreateScrolledCellGrid(Name,Rect,Thickness,Style);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        PagedCellGrid* OpenRenderableContainerWidget::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            ++RenderablesAdded;
            PagedCellGrid* widget = ParentScreen->CreatePagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        DropDownList* OpenRenderableContainerWidget::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            ++RenderablesAdded;
            DropDownList* widget = ParentScreen->CreateDropDownList(Name,Rect,LineHeight,ScrollStyle);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        TabSet* OpenRenderableContainerWidget::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            ++RenderablesAdded;
            TabSet* widget = ParentScreen->CreateTabSet(Name,SetRect);
            Widgets.push_back(widget);
            AddSubRenderable(RenderablesAdded,widget);
            return widget;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with all Widgets

        Widget* OpenRenderableContainerWidget::GetWidget(ConstString& Name)
        {
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                if( Name == (*WidIt)->GetName() )
                {
                    return (*WidIt);
                }
            }
        }

        Widget* OpenRenderableContainerWidget::GetWidget(const Whole& Index)
        {
            return Widgets.at(Index);
        }

        Whole OpenRenderableContainerWidget::GetNumWidgets()
        {
            return Widgets.size();
        }

        void OpenRenderableContainerWidget::DestroyWidget(Widget* ToBeDestroyed)
        {
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                if( (*WidIt) == ToBeDestroyed )
                {
                    ParentScreen->DestroyWidget( (*WidIt) );
                    Widgets.erase(WidIt);
                    return;
                }
            }
        }

        void OpenRenderableContainerWidget::DestroyAllWidgets()
        {
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                ParentScreen->DestroyWidget( (*WidIt) );
            }
            Widgets.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from Widget

        void OpenRenderableContainerWidget::SetPosition(const Vector2& Position)
        {
            // This widget has no real defined position.
        }

        void OpenRenderableContainerWidget::SetActualPosition(const Vector2& Position)
        {
            // This widget has no real defined position.
        }

        void OpenRenderableContainerWidget::SetSize(const Vector2& Size)
        {
            // This widget has no real defined size.
        }

        void OpenRenderableContainerWidget::SetActualSize(const Vector2& Size)
        {
            // This widget has no real defined size.
        }

        void OpenRenderableContainerWidget::UpdateDimensions()
        {
            for( AreaRenderableIterator ArIt = AreaRenderables.begin() ; ArIt != AreaRenderables.end() ; ++ArIt )
            {
                (*ArIt)->UpdateDimensions();
            }
            for( WidgetIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                (*WidIt)->UpdateDimensions();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // EnclosedRenderableContainerWidget Methods

        EnclosedRenderableContainerWidget::EnclosedRenderableContainerWidget(ConstString& name, const RenderableRect& Rect, Screen* Parent)
            : RenderableContainerWidget(name,Parent)
        {
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / ParentScreen->GetViewportDimensions();
                RelSize = Rect.Size / ParentScreen->GetViewportDimensions();
            }

            WidgetBack = ParentScreen->CreateRectangle(Rect);
            AddSubRenderable(0,WidgetBack);
        }

        EnclosedRenderableContainerWidget::~EnclosedRenderableContainerWidget()
        {
            DestroyAllWidgets();
            DestroyAllAreaRenderables();
        }

        void EnclosedRenderableContainerWidget::SetVisibleImpl(bool visible)
        {
            WidgetBack->SetVisible(visible);
            for( AreaRenderableInfoIterator it = AreaRenderables.begin() ; it != AreaRenderables.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( WidgetInfoIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
        }

        bool EnclosedRenderableContainerWidget::CheckMouseHoverImpl()
        {
            if(HoveredSubWidget)
            {
                if(HoveredSubWidget->CheckMouseHover())
                    return true;
            }
            for( WidgetInfoIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; WidIt++ )
            {
                if( (*WidIt).Object->CheckMouseHover() )
                {
                    HoveredSubWidget = (*WidIt).Object;
                    return true;
                }
            }
            if(WidgetBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                return true;
            }
            return false;
        }

        void EnclosedRenderableContainerWidget::SetLocation(const Vector2& Position)
        {
            WidgetBack->SetActualPosition(Position);
            for( AreaRenderableInfoIterator it = AreaRenderables.begin() ; it != AreaRenderables.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( WidgetInfoIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        void EnclosedRenderableContainerWidget::SetArea(const Vector2& Size)
        {
            Vector2 OldSize = WidgetBack->GetActualSize();
            Vector2 Position = WidgetBack->GetActualPosition();
            WidgetBack->SetActualSize(Size);
            for( AreaRenderableInfoIterator it = AreaRenderables.begin() ; it != AreaRenderables.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( WidgetInfoIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        Vector2 EnclosedRenderableContainerWidget::CalculateOffset(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleOffset, const UI::ResizeableAnchor& Anchor)
        {
            Vector2 NewOffset;
            switch (Anchor)
            {
                case RA_AnchorTopLeft:
                {
                    NewOffset = EleOffset;
                    break;
                }
                case RA_AnchorTop:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y;
                    break;
                }
                case RA_AnchorTopRight:
                {
                    NewOffset.X = EleOffset.X + (NewSize.X - OldSize.X);
                    NewOffset.Y = EleOffset.Y;
                    break;
                }
                case RA_AnchorLeft:
                {
                    NewOffset.X = EleOffset.X;
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorMiddle:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorRight:
                {
                    NewOffset.X = EleOffset.X + (NewSize.X - OldSize.X);
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorBottomLeft:
                {
                    NewOffset.X = EleOffset.X;
                    NewOffset.Y = EleOffset.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                case RA_AnchorBottom:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                case RA_AnchorBottomRight:
                {
                    NewOffset = EleOffset + (NewSize - OldSize);
                    break;
                }
                default:
                    return NewOffset;
            }
            return NewOffset;
        }

        Vector2 EnclosedRenderableContainerWidget::CalculateSize(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleSize, const UI::ResizeableTether& Tether)
        {
            Vector2 NewEleSize;
            switch (Tether)
            {
                case RT_TetherBoth:
                {
                    NewEleSize = EleSize + (NewSize - OldSize);
                    break;
                }
                case RT_TetherNone:
                {
                    NewEleSize = EleSize;
                    break;
                }
                case RT_TetherHorizontal:
                {
                    NewEleSize.X = EleSize.X + (NewSize.X - OldSize.X);
                    NewEleSize.Y = EleSize.Y;
                    break;
                }
                case RT_TetherVertical:
                {
                    NewEleSize.X = EleSize.X;
                    NewEleSize.Y = EleSize.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                default:
                    return NewEleSize;
            }
            return NewEleSize;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating AreaRenderables

        Rectangle* EnclosedRenderableContainerWidget::CreateRectangle(const RenderableRect& Rect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetAreaRenderableInfo renderableinfo(ParentScreen->CreateRectangle(Rect),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            AreaRenderables.push_back(renderableinfo);
            AddSubRenderable(RenderablesAdded,renderableinfo.Object);
            renderableinfo.Object->SetVisible(Visible);
            return static_cast<Rectangle*>(renderableinfo.Object);
        }

        Caption* EnclosedRenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetAreaRenderableInfo renderableinfo(ParentScreen->CreateCaption(Name,Rect,Glyph,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            AreaRenderables.push_back(renderableinfo);
            AddSubRenderable(RenderablesAdded,renderableinfo.Object);
            renderableinfo.Object->SetVisible(Visible);
            return static_cast<Caption*>(renderableinfo.Object);
        }

        Caption* EnclosedRenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetAreaRenderableInfo renderableinfo(ParentScreen->CreateCaption(Name,Rect,LineHeight,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            AreaRenderables.push_back(renderableinfo);
            AddSubRenderable(RenderablesAdded,renderableinfo.Object);
            renderableinfo.Object->SetVisible(Visible);
            return static_cast<Caption*>(renderableinfo.Object);
        }

        MarkupText* EnclosedRenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetAreaRenderableInfo renderableinfo(ParentScreen->CreateMarkupText(Name,Rect,Glyph,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            AreaRenderables.push_back(renderableinfo);
            AddSubRenderable(RenderablesAdded,renderableinfo.Object);
            renderableinfo.Object->SetVisible(Visible);
            return static_cast<MarkupText*>(renderableinfo.Object);
        }

        MarkupText* EnclosedRenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetAreaRenderableInfo renderableinfo(ParentScreen->CreateMarkupText(Name,Rect,LineHeight,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            AreaRenderables.push_back(renderableinfo);
            AddSubRenderable(RenderablesAdded,renderableinfo.Object);
            renderableinfo.Object->SetVisible(Visible);
            return static_cast<MarkupText*>(renderableinfo.Object);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with All AreaRenderables

        AreaRenderable* EnclosedRenderableContainerWidget::GetAreaRenderable(ConstString& Name)
        {
            return GetOffsetAreaRenderableInfo(Name).Object;
        }

        AreaRenderable* EnclosedRenderableContainerWidget::GetAreaRenderable(const Whole& Index)
        {
            return GetOffsetAreaRenderableInfo(Index).Object;
        }

        EnclosedRenderableContainerWidget::OffsetAreaRenderableInfo& EnclosedRenderableContainerWidget::GetOffsetAreaRenderableInfo(ConstString& Name)
        {
            for( AreaRenderableInfoIterator RendIt = AreaRenderables.begin() ; RendIt != AreaRenderables.end() ; ++RendIt )
            {
                if( Name == (*RendIt).Object->GetName() )
                {
                    return (*RendIt);
                }
            }
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Renderable name: \"" + Name + "\" not found.");
        }

        EnclosedRenderableContainerWidget::OffsetAreaRenderableInfo& EnclosedRenderableContainerWidget::GetOffsetAreaRenderableInfo(const Whole& Index)
        {
            return AreaRenderables.at(Index);
        }

        Whole EnclosedRenderableContainerWidget::GetNumAreaRenderables()
        {
            return AreaRenderables.size();
        }

        void EnclosedRenderableContainerWidget::DestroyAreaRenderable(AreaRenderable* ToBeDestroyed)
        {
            for( AreaRenderableInfoIterator RendIt = AreaRenderables.begin() ; RendIt != AreaRenderables.end() ; ++RendIt )
            {
                if( ToBeDestroyed == (*RendIt).Object )
                {
                    ParentScreen->DestroyBasicRenderable(ToBeDestroyed);
                    AreaRenderables.erase(RendIt);
                    return;
                }
            }
        }

        void EnclosedRenderableContainerWidget::DestroyAllAreaRenderables()
        {
            for( AreaRenderableInfoIterator RendIt = AreaRenderables.begin() ; RendIt != AreaRenderables.end() ; ++RendIt )
            {
                ParentScreen->DestroyBasicRenderable( (*RendIt).Object );
            }
            AreaRenderables.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Widgets

        Button* EnclosedRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo ButtonInfo(ParentScreen->CreateButton(Name,Rect),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ButtonInfo);
            AddSubRenderable(RenderablesAdded,ButtonInfo.Object);
            ButtonInfo.Object->SetVisible(Visible);
            return static_cast<Button*>(ButtonInfo.Object);
        }

        Button* EnclosedRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo ButtonInfo(ParentScreen->CreateButton(Name,Rect,Glyph,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ButtonInfo);
            AddSubRenderable(RenderablesAdded,ButtonInfo.Object);
            ButtonInfo.Object->SetVisible(Visible);
            return static_cast<Button*>(ButtonInfo.Object);
        }

        Button* EnclosedRenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo ButtonInfo(ParentScreen->CreateButton(Name,Rect,LineHeight,Text),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ButtonInfo);
            AddSubRenderable(RenderablesAdded,ButtonInfo.Object);
            ButtonInfo.Object->SetVisible(Visible);
            return static_cast<Button*>(ButtonInfo.Object);
        }

        Scrollbar* EnclosedRenderableContainerWidget::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo ScrollInfo(ParentScreen->CreateScrollbar(Name,Rect,Style),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ScrollInfo);
            AddSubRenderable(RenderablesAdded,ScrollInfo.Object);
            ScrollInfo.Object->SetVisible(Visible);
            return static_cast<Scrollbar*>(ScrollInfo.Object);
        }

        CheckBox* EnclosedRenderableContainerWidget::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo CheckInfo(ParentScreen->CreateCheckBox(Name,Rect,LineHeight,LabelText),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(CheckInfo);
            AddSubRenderable(RenderablesAdded,CheckInfo.Object);
            CheckInfo.Object->SetVisible(Visible);
            return static_cast<CheckBox*>(CheckInfo.Object);
        }

        ListBox* EnclosedRenderableContainerWidget::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo LBInfo(ParentScreen->CreateListBox(Name,Rect,ScrollbarStyle),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(LBInfo);
            AddSubRenderable(RenderablesAdded,LBInfo.Object);
            LBInfo.Object->SetVisible(Visible);
            return static_cast<ListBox*>(LBInfo.Object);
        }

        Spinner* EnclosedRenderableContainerWidget::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo SpnInfo(ParentScreen->CreateSpinner(Name,Rect,SStyle,GlyphHeight),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SpnInfo);
            AddSubRenderable(RenderablesAdded,SpnInfo.Object);
            SpnInfo.Object->SetVisible(Visible);
            return static_cast<Spinner*>(SpnInfo.Object);
        }

        ScrolledCellGrid* EnclosedRenderableContainerWidget::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo SCGInfo(ParentScreen->CreateScrolledCellGrid(Name,Rect,Thickness,Style),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SCGInfo);
            AddSubRenderable(RenderablesAdded,SCGInfo.Object);
            SCGInfo.Object->SetVisible(Visible);
            return static_cast<ScrolledCellGrid*>(SCGInfo.Object);
        }

        PagedCellGrid* EnclosedRenderableContainerWidget::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo PCGInfo(ParentScreen->CreatePagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(PCGInfo);
            AddSubRenderable(RenderablesAdded,PCGInfo.Object);
            PCGInfo.Object->SetVisible(Visible);
            return static_cast<PagedCellGrid*>(PCGInfo.Object);
        }

        DropDownList* EnclosedRenderableContainerWidget::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetWidgetInfo DDLInfo(ParentScreen->CreateDropDownList(Name,Rect,LineHeight,ScrollStyle),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(DDLInfo);
            AddSubRenderable(RenderablesAdded,DDLInfo.Object);
            DDLInfo.Object->SetVisible(Visible);
            return static_cast<DropDownList*>(DDLInfo.Object);
        }

        TabSet* EnclosedRenderableContainerWidget::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(SetRect.Relative) Offset = (SetRect.Position - RelPosition) * ParentScreen->GetViewportDimensions();
            else Offset = SetRect.Position - GetActualPosition();
            OffsetWidgetInfo TSInfo(ParentScreen->CreateTabSet(Name,SetRect),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(TSInfo);
            AddSubRenderable(RenderablesAdded,TSInfo.Object);
            TSInfo.Object->SetVisible(Visible);
            return static_cast<TabSet*>(TSInfo.Object);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with all Widgets

        Widget* EnclosedRenderableContainerWidget::GetWidget(ConstString& Name)
        {
            return GetOffsetWidgetInfo(Name).Object;
        }

        Widget* EnclosedRenderableContainerWidget::GetWidget(const Whole& Index)
        {
            return GetOffsetWidgetInfo(Index).Object;
        }

        EnclosedRenderableContainerWidget::OffsetWidgetInfo& EnclosedRenderableContainerWidget::GetOffsetWidgetInfo(ConstString& Name)
        {
            for( WidgetInfoIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                if( Name == (*WidIt).Object->GetName() )
                {
                    return (*WidIt);
                }
            }
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Widget name: \"" + Name + "\" not found.");
        }

        EnclosedRenderableContainerWidget::OffsetWidgetInfo& EnclosedRenderableContainerWidget::GetOffsetWidgetInfo(const Whole& Index)
        {
            return Widgets.at(Index);
        }

        Whole EnclosedRenderableContainerWidget::GetNumWidgets()
        {
            return Widgets.size();
        }

        void EnclosedRenderableContainerWidget::DestroyWidget(Widget* ToBeDestroyed)
        {
            for( WidgetInfoIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                if( ToBeDestroyed == (*WidIt).Object )
                {
                    ParentScreen->DestroyWidget(ToBeDestroyed);
                    Widgets.erase(WidIt);
                    return;
                }
            }
        }

        void EnclosedRenderableContainerWidget::DestroyAllWidgets()
        {
            for( WidgetInfoIterator WidIt = Widgets.begin() ; WidIt != Widgets.end() ; ++WidIt )
            {
                ParentScreen->DestroyWidget( (*WidIt).Object );
            }
            Widgets.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from Widget

        void EnclosedRenderableContainerWidget::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ActPos = Position * ParentScreen->GetViewportDimensions();
            SetLocation(ActPos);
        }

        void EnclosedRenderableContainerWidget::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / ParentScreen->GetViewportDimensions();
            SetLocation(Position);
        }

        void EnclosedRenderableContainerWidget::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Vector2 ActSize = Size * ParentScreen->GetViewportDimensions();
            SetArea(ActSize);
        }

        void EnclosedRenderableContainerWidget::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / ParentScreen->GetViewportDimensions();
            SetArea(Size);
        }

        void EnclosedRenderableContainerWidget::UpdateDimensions()
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            //const Vector2& OldViewportSize = ViewportUpdateTool::GetOldSize();
            Real Scale = ViewportUpdateTool::GetNewSize().Y / ViewportUpdateTool::GetOldSize().Y;
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            WidgetBack->UpdateDimensions();
            for( AreaRenderableInfoIterator it = AreaRenderables.begin() ; it != AreaRenderables.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            for( WidgetInfoIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            SetPosition(RelPosition);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Rectangle* EnclosedRenderableContainerWidget::GetWidgetBack()
        {
            return WidgetBack;
        }
    }//ui
}//Mezzanine

#endif
