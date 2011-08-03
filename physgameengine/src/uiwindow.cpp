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
#ifndef _uiwindow_cpp
#define _uiwindow_cpp

#include "uiwindow.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uicaption.h"
#include "uimarkuptext.h"
#include "uilinelist.h"
#include "uirectangle.h"
#include "uibutton.h"
#include "uitextbutton.h"
#include "uiscrollbar.h"
#include "uilistbox.h"
#include "uibuttonlistbox.h"
#include "uicheckbox.h"
#include "uispinner.h"
#include "uiscrolledcellgrid.h"
#include "uipagedcellgrid.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace phys
{
    namespace UI
    {
        Window::Window(ConstString& name, const RenderableRect& Rect, Layer* PLayer)
            : Widget(name,PLayer),
              BorderWidth(0),
              CurrentRM(RM_None)
        {
            Type = Widget::Window;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();
            }

            WindowBack = new Rectangle(Rect,Parent);
        }

        Window::~Window()
        {
            delete WindowBack;
            while(!Buttons.empty())
            {
                UI::Button* button = Buttons.back().Object;
                delete button;
                Buttons.pop_back();
            }
            while(!Rectangles.empty())
            {
                UI::Rectangle* rectangle = Rectangles.back().Object;
                delete rectangle;
                Rectangles.pop_back();
            }
            while(!Captions.empty())
            {
                UI::Caption* caption = Captions.back().Object;
                delete caption;
                Captions.pop_back();
            }
            while(!MarkupTexts.empty())
            {
                UI::MarkupText* markup = MarkupTexts.back().Object;
                delete markup;
                MarkupTexts.pop_back();
            }
            while(!Widgets.empty())
            {
                UI::Widget* widget = Widgets.back().Object;
                delete widget;
                Widgets.pop_back();
            }
        }

        void Window::Update(bool Force)
        {
            if(!Force)
                SubWidgetUpdate();
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredButton)
            {
            }
            else if(HoveredSubWidget)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    SubWidgetFocus = HoveredSubWidget;
                }
            }
            if(SubWidgetFocus && (SubWidgetFocus != HoveredSubWidget))
            {
                SubWidgetFocusUpdate(true);
            }
            else if(MetaCode::BUTTON_DOWN == State && Force)
            {
                SubWidgetFocusUpdate(Force);
            }
            if(MetaCode::BUTTON_LIFTING == State)
            {
                SubWidgetFocus = NULL;
            }
        }

        void Window::SetLocation(const Vector2& Position)
        {
            WindowBack->SetActualPosition(Position);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        void Window::SetArea(const Vector2& Size)
        {
            Vector2 OldSize = WindowBack->GetActualSize();
            Vector2 Position = WindowBack->GetActualPosition();
            WindowBack->SetActualSize(Size);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                //if((*it).Tether != RT_TetherNone)
                //    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        void Window::BorderAreaCheck(const Vector2& ScreenLoc)
        {
            if(!BorderWidth)
                return;
            Vector2 Size = WindowBack->GetActualSize();
            Vector2 Position = WindowBack->GetActualPosition();
            bool Left = ScreenLoc.X >= Position.X && ScreenLoc.X <= Position.X + BorderWidth;
            bool Right = ScreenLoc.X >= Position.X + Size.X && ScreenLoc.X <= (Position.X + Size.X) - BorderWidth;
            bool Top = ScreenLoc.Y >= Position.Y && ScreenLoc.Y <= Position.Y + BorderWidth;
            bool Bottom = ScreenLoc.Y >= Position.Y + Size.Y && ScreenLoc.Y <= (Position.Y + Size.Y) - BorderWidth;
            if( Top && Left ) CurrentRM = Window::RM_TopLeft;
            else if( Top && Right ) CurrentRM = Window::RM_TopRight;
            else if( Bottom && Left ) CurrentRM = Window::RM_BottomLeft;
            else if( Bottom && Right ) CurrentRM = Window::RM_BottomRight;
            else if( Left ) CurrentRM = Window::RM_Left;
            else if( Right ) CurrentRM = Window::RM_BottomRight;
            else if( Top ) CurrentRM = Window::RM_BottomRight;
            else if( Bottom ) CurrentRM = Window::RM_BottomRight;
        }

        Vector2 Window::CalculateOffset(const Vector2 NewSize, const Vector2 OldSize, const Vector2 EleOffset, UI::ResizeableAnchor Anchor)
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

        Vector2 Window::CalculateSize(const Vector2 NewSize, const Vector2 OldSize, const Vector2 EleSize, UI::ResizeableTether Tether)
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

        void Window::SetVisible(bool visible)
        {
            if(Visible == visible)
                return;
            //WindowTitle->SetVisible(visible);
            WindowBack->SetVisible(visible);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            Visible = visible;
        }

        bool Window::IsVisible()
        {
            return Visible && Parent->IsVisible() && Parent->GetParent()->IsVisible();
        }

        void Window::Show()
        {
            if(Visible)
                return;
            //WindowTitle->Show();
            WindowBack->Show();
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->Show();
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->Show();
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->Show();
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->Show();
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->Show();
            }
            Visible = true;
        }

        void Window::Hide()
        {
            if(!Visible)
                return;
            //WindowTitle->Hide();
            WindowBack->Hide();
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->Hide();
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->Hide();
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->Hide();
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->Hide();
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->Hide();
            }
            Visible = false;
        }

        Rectangle* Window::GetWindowBack()
        {
            return WindowBack;
        }

        bool Window::CheckMouseHover()
        {
            if(!IsVisible())
                return false;
            if(HoveredButton)
            {
                if(HoveredButton->CheckMouseHover())
                    return true;
            }
            if(HoveredSubWidget)
            {
                if(HoveredSubWidget->CheckMouseHover())
                    return true;
            }
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if((*it).Object->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredButton = (*it).Object;
                    return true;
                }
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if((*it).Object->CheckMouseHover())
                {
                    HoveredSubWidget = (*it).Object;
                    HoveredButton = NULL;
                    return true;
                }
            }
            if(WindowBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                return true;
            }
            HoveredSubWidget = NULL;
            HoveredButton = NULL;
            return false;
        }

        void Window::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ActPos = Position * Parent->GetParent()->GetViewportDimensions();
            SetLocation(ActPos);
        }

        Vector2 Window::GetPosition()
        {
            return RelPosition;
        }

        void Window::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            SetLocation(Position);
        }

        Vector2 Window::GetActualPosition()
        {
            return RelPosition * Parent->GetParent()->GetViewportDimensions();
        }

        void Window::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Vector2 ActSize = Size * Parent->GetParent()->GetViewportDimensions();
            SetArea(ActSize);
        }

        Vector2 Window::GetSize()
        {
            return RelSize;
        }

        void Window::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            SetArea(Size);
        }

        Vector2 Window::GetActualSize()
        {
            return RelSize * Parent->GetParent()->GetViewportDimensions();
        }

        void Window::UpdateDimensions(const Vector2& OldViewportSize)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            WindowBack->UpdateDimensions();
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset / OldViewportSize) * WinDim;
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset / OldViewportSize) * WinDim;
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset / OldViewportSize) * WinDim;
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset / OldViewportSize) * WinDim;
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset / OldViewportSize) * WinDim;
                (*it).Object->UpdateDimensions(OldViewportSize);
            }
        }

        Button* Window::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetButtonInfo buttoninfo(new Button(Name,Rect,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(buttoninfo);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    buttoninfo.Object->BindActivationKeyOrButton(Codes->at(X));
            }
            buttoninfo.Object->SetVisible(Visible);
            return buttoninfo.Object;
        }

        TextButton* Window::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            TextButton* tbutton = new TextButton(Name,Rect,Glyph,Text,Parent);
            OffsetButtonInfo tbuttoninfo(tbutton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(tbuttoninfo);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            tbutton->SetVisible(Visible);
            return tbutton;
        }

        TextButton* Window::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            TextButton* tbutton = new TextButton(Name,Rect,LineHeight,Text,Parent);
            OffsetButtonInfo tbuttoninfo(tbutton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(tbuttoninfo);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            tbutton->SetVisible(Visible);
            return tbutton;
        }

        Button* Window::GetButton(ConstString& Name)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Button* button = (*it).Object;
                    return button;
                }
            }
            return 0;
        }

        Button* Window::GetButton(const Whole& Index)
        {
            return Buttons[Index].Object;
        }

        OffsetButtonInfo* Window::GetOffsetButtonInfo(ConstString& Name)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetButtonInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetButtonInfo* Window::GetOffsetButtonInfo(const Whole& Index)
        {
            OffsetButtonInfo* Offset = &Buttons[Index];
            return Offset;
        }

        Whole Window::GetNumButtons()
        {
            return Buttons.size();
        }

        void Window::DestroyButton(Button* ToBeDestroyed)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Buttons.erase(it);
                    return;
                }
            }
        }

        Rectangle* Window::CreateRectangle(const RenderableRect& Rect)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetRectangleInfo rectangle(new Rectangle(Rect,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Rectangles.push_back(rectangle);
            rectangle.Object->SetVisible(Visible);
            return rectangle.Object;
        }

        Rectangle* Window::GetRectangle(const Whole& Index)
        {
            return Rectangles[Index].Object;
        }

        OffsetRectangleInfo* Window::GetOffsetRectangleInfo(const Whole& Index)
        {
            OffsetRectangleInfo* Offset = &Rectangles[Index];
            return Offset;
        }

        Whole Window::GetNumRectangles()
        {
            return Rectangles.size();
        }

        void Window::DestroyRectangle(Rectangle* ToBeDestroyed)
        {
            for ( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Rectangles.erase(it);
                    return;
                }
            }
        }

        Caption* Window::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetCaptionInfo caption(new Caption(Name,Rect,Glyph,Text,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Captions.push_back(caption);
            caption.Object->SetVisible(Visible);
            return caption.Object;
        }

        Caption* Window::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetCaptionInfo caption(new Caption(Name,Rect,LineHeight,Text,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Captions.push_back(caption);
            caption.Object->SetVisible(Visible);
            return caption.Object;
        }

        Caption* Window::GetCaption(ConstString& Name)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Caption* caption = (*it).Object;
                    return caption;
                }
            }
            return 0;
        }

        Caption* Window::GetCaption(const Whole& Index)
        {
            return Captions[Index].Object;
        }

        OffsetCaptionInfo* Window::GetOffsetCaptionInfo(ConstString& Name)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetCaptionInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetCaptionInfo* Window::GetOffsetCaptionInfo(const Whole& Index)
        {
            OffsetCaptionInfo* Offset = &Captions[Index];
            return Offset;
        }

        Whole Window::GetNumCaptions()
        {
            return Captions.size();
        }

        void Window::DestroyCaption(Caption* ToBeDestroyed)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Captions.erase(it);
                    return;
                }
            }
        }

        MarkupText* Window::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            MarkupText* markup = new MarkupText(Name,Rect,Glyph,Text,Parent);
            OffsetMarkupTextInfo markupinfo(markup,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            MarkupTexts.push_back(markupinfo);
            markupinfo.Object->SetVisible(Visible);
            return markupinfo.Object;
        }

        MarkupText* Window::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            MarkupText* markup = new MarkupText(Name,Rect,LineHeight,Text,Parent);
            OffsetMarkupTextInfo markupinfo(markup,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            MarkupTexts.push_back(markupinfo);
            markupinfo.Object->SetVisible(Visible);
            return markupinfo.Object;
        }

        MarkupText* Window::GetMarkupText(ConstString& Name)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    MarkupText* markup = (*it).Object;
                    return markup;
                }
            }
            return 0;
        }

        MarkupText* Window::GetMarkupText(const Whole& Index)
        {
            return MarkupTexts[Index].Object;
        }

        OffsetMarkupTextInfo* Window::GetOffsetMarkupTextInfo(ConstString& Name)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetMarkupTextInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetMarkupTextInfo* Window::GetOffsetMarkupTextInfo(const Whole& Index)
        {
            OffsetMarkupTextInfo* Offset = &MarkupTexts[Index];
            return Offset;
        }

        Whole Window::GetNumMarkupTexts()
        {
            return MarkupTexts.size();
        }

        void Window::DestroyMarkupText(MarkupText* ToBeDestroyed)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    MarkupTexts.erase(it);
                    return;
                }
            }
        }

        Widget* Window::GetWidget(ConstString& Name)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Widget* widget = (*it).Object;
                    return widget;
                }
            }
            return 0;
        }

        Widget* Window::GetWidget(const Whole& Index)
        {
            return Widgets[Index].Object;
        }

        OffsetWidgetInfo* Window::GetOffsetWidgetInfo(ConstString& Name)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetWidgetInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetWidgetInfo* Window::GetOffsetWidgetInfo(const Whole& Index)
        {
            OffsetWidgetInfo* Offset = &Widgets[Index];
            return Offset;
        }

        Whole Window::GetNumWidgets()
        {
            return Widgets.size();
        }

        void Window::DestroyWidget(Widget* ToBeDestroyed)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete (*it).Object;
                    Widgets.erase(it);
                    return;
                }
            }
        }

        UI::Scrollbar* Window::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::Scrollbar* Scroll = new UI::Scrollbar(Name,Rect,Style,Parent);
            OffsetWidgetInfo ScrollInfo(Scroll,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ScrollInfo);
            Scroll->SetVisible(Visible);
            return Scroll;
        }

        UI::CheckBox* Window::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String &LabelText)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::CheckBox* Check = new UI::CheckBox(Name,Rect,Glyph,LabelText,Parent);
            OffsetWidgetInfo CheckInfo(Check,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(CheckInfo);
            Check->SetVisible(Visible);
            return Check;
        }

        UI::ButtonListBox* Window::CreateButtonListBox(ConstString& Name, const RenderableRect& Rect, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollStyle)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::ButtonListBox* BLB = new UI::ButtonListBox(Name,Rect,ScrollbarWidth,ScrollStyle,Parent);
            OffsetWidgetInfo BLBInfo(BLB,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(BLBInfo);
            BLB->SetVisible(Visible);
            return BLB;
        }

        UI::ListBox* Window::CreateListBox(ConstString& Name, const RenderableRect& Rect, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollStyle)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::ListBox* LB = new UI::ListBox(Name,Rect,ScrollbarWidth,ScrollStyle,Parent);
            OffsetWidgetInfo LBInfo(LB,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(LBInfo);
            LB->SetVisible(Visible);
            return LB;
        }

        UI::Spinner* Window::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::Spinner* Spn = new UI::Spinner(Name,Rect,SStyle,GlyphHeight,Parent);
            OffsetWidgetInfo SpnInfo(Spn,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SpnInfo);
            Spn->SetVisible(Visible);
            return Spn;
        }

        UI::ScrolledCellGrid* Window::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::ScrolledCellGrid* SCG = new UI::ScrolledCellGrid(Name,Rect,Thickness,Style,Parent);
            OffsetWidgetInfo SCGInfo(SCG,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SCGInfo);
            SCG->SetVisible(Visible);
            return SCG;
        }

        UI::PagedCellGrid* Window::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * Parent->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            UI::PagedCellGrid* PCG = new UI::PagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight,Parent);
            OffsetWidgetInfo PCGInfo(PCG,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(PCGInfo);
            PCG->SetVisible(Visible);
            return PCG;
        }
    }//UI
}//phys

#endif
