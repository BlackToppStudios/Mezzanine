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
#ifndef _uiscrollbar_cpp
#define _uiscrollbar_cpp

#include "uiscrollbar.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "uibutton.h"
#include "uirectangle.h"
#include "inputquerytool.h"
#include "world.h"

namespace phys
{
    namespace UI
    {
        Scrollbar::Scrollbar(ConstString& name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style, Layer* parent)
            : Widget(name, parent),
              UpLeftButton(NULL),
              DownRightButton(NULL),
              ScrollerValue(0),
              IncrementDistance(0.1),
              ScrollerLock(true),
              ScrollBackLock(true),
              UpLeftLock(true),
              DownRightLock(true)
        {
            Type = Widget::Scrollbar;
            ScrollStyle = Style;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                if(Rect.Size.Y > Rect.Size.X * 2)
                {
                    Vector2 ASize = Rect.Size * Parent->GetParent()->GetViewportDimensions();
                    CalculateOffsets(ASize);
                    CreateVerticalScrollbar(Rect);
                }
                else if(Rect.Size.X > Rect.Size.Y * 2)
                {
                    Vector2 ASize = Rect.Size * Parent->GetParent()->GetViewportDimensions();
                    CalculateOffsets(ASize);
                    CreateHorizontalScrollbar(Rect);
                }
                else
                {
                    World::GetWorldPointer()->LogAndThrow("Scrollbar dimensions incompatible with this widget.");
                }
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                if(Rect.Size.Y > Rect.Size.X * 2)
                {
                    CalculateOffsets(Rect.Size);
                    CreateVerticalScrollbar(Rect);
                }
                else if(Rect.Size.X > Rect.Size.Y * 2)
                {
                    CalculateOffsets(Rect.Size);
                    CreateHorizontalScrollbar(Rect);
                }
                else
                {
                    World::GetWorldPointer()->LogAndThrow("Scrollbar dimensions incompatible with this widget.");
                }
            }
        }

        Scrollbar::~Scrollbar()
        {
            delete Scroller;
            delete ScrollBack;
            delete UpLeftButton;
            delete DownRightButton;
        }

        void Scrollbar::CreateHorizontalScrollbar(const RenderableRect& Rect)
        {
            Horizontal = true;
            //Get the size for all items involved
            RenderableRect ScrollBackRect, LeftButtonRect, RightButtonRect;
            if(UI::SB_NoButtons==ScrollStyle)
            {
                ScrollBackRect.Size = Rect.Size;
            }else{
                if(Rect.Relative)
                {
                    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                    Vector2 ARectSize(Rect.Size * WinDim);

                    ScrollBackRect.Size.X = Rect.Size.X - ((ARectSize.Y * 2) / WinDim.X);
                    ScrollBackRect.Size.Y = Rect.Size.Y;
                    LeftButtonRect.Size.X = ARectSize.Y / WinDim.X;
                    LeftButtonRect.Size.Y = Rect.Size.Y;
                    RightButtonRect.Size.X = ARectSize.Y / WinDim.X;
                    RightButtonRect.Size.Y = Rect.Size.Y;
                }else{
                    ScrollBackRect.Size.X = Rect.Size.X - (Rect.Size.Y * 2);
                    ScrollBackRect.Size.Y = Rect.Size.Y;
                    LeftButtonRect.Size.X = Rect.Size.Y;
                    LeftButtonRect.Size.Y = Rect.Size.Y;
                    RightButtonRect.Size.X = Rect.Size.Y;
                    RightButtonRect.Size.Y = Rect.Size.Y;
                }
            }
            //Get the position for all items involved and configure their offsets
            if(UI::SB_Separate==ScrollStyle)
            {
                ScrollBackRect.Position.X = Rect.Position.X + LeftButtonRect.Size.X;
                ScrollBackRect.Position.Y = Rect.Position.Y;
                LeftButtonRect.Position = Rect.Position;
                RightButtonRect.Position.X = Rect.Position.X + LeftButtonRect.Size.X + ScrollBackRect.Size.X;
                RightButtonRect.Position.Y = Rect.Position.Y;
            }
            else if(UI::SB_TogetherDownRight==ScrollStyle)
            {
                ScrollBackRect.Position = Rect.Position;
                LeftButtonRect.Position.X = Rect.Position.X + ScrollBackRect.Size.X;
                LeftButtonRect.Position.Y = Rect.Position.Y;
                RightButtonRect.Position.X = Rect.Position.X + ScrollBackRect.Size.X + LeftButtonRect.Size.X;
                RightButtonRect.Position.Y = Rect.Position.Y;
            }
            else if(UI::SB_TogetherUpLeft==ScrollStyle)
            {
                ScrollBackRect.Position.X = Rect.Position.X + LeftButtonRect.Size.X + RightButtonRect.Size.X;
                ScrollBackRect.Position.Y = Rect.Position.Y;
                LeftButtonRect.Position = Rect.Position;
                RightButtonRect.Position.X = Rect.Position.X + LeftButtonRect.Size.X;
                RightButtonRect.Position.Y = Rect.Position.Y;
            }
            else
            {
                ScrollBackRect.Position = Rect.Position;
            }
            //Now create the objects, since we have all the position and size data we need
            ScrollBack = new Rectangle(ScrollBackRect,Parent);
            Scroller = new Button(Name+"SC",ScrollBackRect,Parent);
            if(LeftButtonRect.Size.X > 0 && LeftButtonRect.Size.Y > 0)
            {
                UpLeftButton = new Button(Name+"LB",LeftButtonRect,Parent);
            }
            if(RightButtonRect.Size.X > 0 && RightButtonRect.Size.Y > 0)
            {
                DownRightButton = new Button(Name+"RB",RightButtonRect,Parent);
            }
            CalculateScrollLimits();
        }

        void Scrollbar::CreateVerticalScrollbar(const RenderableRect& Rect)
        {
            Horizontal = false;
            //Get the size for all items involved
            RenderableRect ScrollBackRect, UpButtonRect, DownButtonRect;
            if(UI::SB_NoButtons==ScrollStyle)
            {
                ScrollBackRect.Size = Rect.Size;
            }else{
                if(Rect.Relative)
                {
                    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                    Vector2 ARectSize(Rect.Size * WinDim);

                    ScrollBackRect.Size.X = Rect.Size.X;
                    ScrollBackRect.Size.Y = Rect.Size.Y - ((ARectSize.X * 2) / WinDim.Y);
                    UpButtonRect.Size.X = Rect.Size.X;
                    UpButtonRect.Size.Y = ARectSize.X / WinDim.Y;
                    DownButtonRect.Size.X = Rect.Size.X;
                    DownButtonRect.Size.Y = ARectSize.X / WinDim.Y;
                }else{
                    ScrollBackRect.Size.X = Rect.Size.X;
                    ScrollBackRect.Size.Y = Rect.Size.Y - (Rect.Size.X * 2);
                    UpButtonRect.Size.X = Rect.Size.X;
                    UpButtonRect.Size.Y = Rect.Size.X;
                    DownButtonRect.Size.X = Rect.Size.X;
                    DownButtonRect.Size.Y = Rect.Size.X;
                }
            }
            //Get the position for all items involved and configure their offsets
            if(UI::SB_Separate==ScrollStyle)
            {
                ScrollBackRect.Position.X = Rect.Position.X;
                ScrollBackRect.Position.Y = Rect.Position.Y + UpButtonRect.Size.Y;
                UpButtonRect.Position = Rect.Position;
                DownButtonRect.Position.X = Rect.Position.X;
                DownButtonRect.Position.Y = Rect.Position.Y + UpButtonRect.Size.Y + ScrollBackRect.Size.Y;
            }
            else if(UI::SB_TogetherDownRight==ScrollStyle)
            {
                ScrollBackRect.Position = Rect.Position;
                UpButtonRect.Position.X = Rect.Position.X;
                UpButtonRect.Position.Y = Rect.Position.Y + ScrollBackRect.Size.Y;
                DownButtonRect.Position.X = Rect.Position.X;
                DownButtonRect.Position.Y = Rect.Position.Y + ScrollBackRect.Size.Y + UpButtonRect.Size.Y;
            }
            else if(UI::SB_TogetherUpLeft==ScrollStyle)
            {
                ScrollBackRect.Position.X = Rect.Position.X;
                ScrollBackRect.Position.Y = Rect.Position.Y + UpButtonRect.Size.Y + DownButtonRect.Size.Y;
                UpButtonRect.Position = Rect.Position;
                DownButtonRect.Position.X = Rect.Position.X;
                DownButtonRect.Position.Y = Rect.Position.Y + UpButtonRect.Size.Y;
            }
            else
            {
                ScrollBackRect.Position = Rect.Position;
            }
            //Now create the objects, since we have all the position and size data we need
            ScrollBack = new Rectangle(ScrollBackRect,Parent);
            Scroller = new Button(Name+"SC",ScrollBackRect,Parent);
            if(UpButtonRect.Size.X > 0 && UpButtonRect.Size.Y > 0)
            {
                UpLeftButton = new Button(Name+"UB",UpButtonRect,Parent);
            }
            if(DownButtonRect.Size.X > 0 && DownButtonRect.Size.Y > 0)
            {
                DownRightButton = new Button(Name+"DB",DownButtonRect,Parent);
            }
            CalculateScrollLimits();
        }

        bool Scrollbar::IsValidDimensions(const Vector2& Size)
        {
            if(Size.X > Size.Y * 2)
            {
                return true;
            }
            else if(Size.Y > Size.X * 2)
            {
                return true;
            }
            return false;
        }

        void Scrollbar::SetHorizontal(const Vector2& Size)
        {
            if(Size.X > Size.Y * 2)
            {
                Horizontal = true;
            }
            else if(Size.Y > Size.X * 2)
            {
                Horizontal = false;
            }
        }

        void Scrollbar::CalculateOffsets(const Vector2& Size)
        {
            if(Horizontal)
            {
                if(UI::SB_Separate==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(Size.Y,0);
                    UpLeftButtonOffset = Vector2(0,0);
                    DownRightButtonOffset = Vector2((Size.X - (Size.Y * 2)) + Size.Y,0);
                }
                else if(UI::SB_TogetherDownRight==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(0,0);
                    UpLeftButtonOffset = Vector2(Size.X - (Size.Y * 2),0);
                    DownRightButtonOffset = Vector2((Size.X - (Size.Y * 2)) + Size.Y,0);
                }
                else if(UI::SB_TogetherUpLeft==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(Size.Y * 2,0);
                    UpLeftButtonOffset = Vector2(0,0);
                    DownRightButtonOffset = Vector2(Size.Y,0);
                }
            }else{
                if(UI::SB_Separate==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(0,Size.X);
                    UpLeftButtonOffset = Vector2(0,0);
                    DownRightButtonOffset = Vector2(0,(Size.Y - (Size.X * 2)) + Size.X);
                }
                else if(UI::SB_TogetherDownRight==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(0,0);
                    UpLeftButtonOffset = Vector2(0,Size.Y - (Size.X * 2));
                    DownRightButtonOffset = Vector2(0,(Size.Y - (Size.X * 2)) + Size.X);
                }
                else if(UI::SB_TogetherUpLeft==ScrollStyle)
                {
                    ScrollBackOffset = Vector2(0,Size.X * 2);
                    UpLeftButtonOffset = Vector2(0,0);
                    DownRightButtonOffset = Vector2(0,Size.X);
                }
            }
        }

        void Scrollbar::CalculateScrollLimits()
        {
            if(Horizontal)
            {
                ScrollerLowerLimit = ScrollBack->GetActualPosition().X;
                ScrollerUpperLimit = ScrollBack->GetActualPosition().X + ScrollBack->GetActualSize().X;
            }else{
                ScrollerLowerLimit = ScrollBack->GetActualPosition().Y;
                ScrollerUpperLimit = ScrollBack->GetActualPosition().Y + ScrollBack->GetActualSize().Y;
            }
        }

        void Scrollbar::CalculateScrollValue()
        {
            Real Pos = 0;
            if(Horizontal)
            {
                Pos = Scroller->GetActualPosition().X;
            }else{
                Pos = Scroller->GetActualPosition().Y;
            }
            Real RelPos = Pos - ScrollerLowerLimit;
            Real RelLimit = ScrollerUpperLimit - ScrollerLowerLimit;
            ScrollerValue = RelPos / RelLimit;
        }

        void Scrollbar::SetToWithinLimits(Real& Coord)
        {
            Real SizeOffset = 0;
            if(Horizontal)
            {
                SizeOffset = Scroller->GetActualSize().X;
            }else{
                SizeOffset = Scroller->GetActualSize().Y;
            }
            if(Coord < ScrollerLowerLimit)
            {
                Coord = ScrollerLowerLimit;
            }
            else if(Coord + SizeOffset > ScrollerUpperLimit)
            {
                Coord = ScrollerUpperLimit - SizeOffset;
            }
        }

        void Scrollbar::SetLocation(const Vector2& Position)
        {
            ScrollBack->SetActualPosition(Position + ScrollBackOffset);
            CalculateScrollLimits();
            Vector2 ScrollVal(0,0);
            if(Horizontal)
            {
                ScrollVal.X = (ScrollerUpperLimit - ScrollerLowerLimit) * ScrollerValue;
            }else{
                ScrollVal.Y = (ScrollerUpperLimit - ScrollerLowerLimit) * ScrollerValue;
            }
            Scroller->SetActualPosition(Position + ScrollBackOffset + ScrollVal);
            if(UpLeftButton)
            {
                UpLeftButton->SetActualPosition(Position + UpLeftButtonOffset);
            }
            if(DownRightButton)
            {
                DownRightButton->SetActualPosition(Position + DownRightButtonOffset);
            }
        }

        void Scrollbar::SetArea(const Vector2& Size)
        {
            if(Horizontal)
            {
                ScrollBack->SetActualSize(Vector2(Size.X - (Size.Y * 2),Size.Y));
                if(UpLeftButton)
                {
                    UpLeftButton->SetActualSize(Vector2(Size.Y,Size.Y));
                }
                if(DownRightButton)
                {
                    DownRightButton->SetActualSize(Vector2(Size.Y,Size.Y));
                }
            }else{
                ScrollBack->SetActualSize(Vector2(Size.X,Size.Y - (Size.X * 2)));
                if(UpLeftButton)
                {
                    UpLeftButton->SetActualSize(Vector2(Size.X,Size.X));
                }
                if(DownRightButton)
                {
                    DownRightButton->SetActualSize(Vector2(Size.X,Size.X));
                }
            }
            CalculateScrollLimits();
        }

        void Scrollbar::ButtonScroll(bool UpLeft)
        {
            if((!UpLeftLock && HoveredButton == UpLeftButton) || (!DownRightLock && HoveredButton == DownRightButton))
            {
                Vector2 Loc = Scroller->GetActualPosition();
                Real Scroll = ScrollBack->GetActualSize().X * IncrementDistance;
                if(UpLeft)
                {
                    Scroll = -Scroll;
                }
                if(Horizontal)
                {
                    Loc.X+=Scroll;
                    SetToWithinLimits(Loc.X);
                }else{
                    Loc.Y+=Scroll;
                    SetToWithinLimits(Loc.Y);
                }
                Scroller->SetActualPosition(Loc);
            }
        }

        void Scrollbar::MouseScroll(Vector2& Scroll)
        {
            if(!ScrollerLock)
            {
                Vector2 Loc = Scroller->GetActualPosition();
                if(Horizontal)
                {
                    Scroll.Y = Loc.Y;
                    Scroll.X = Loc.X + Scroll.X;
                    SetToWithinLimits(Scroll.X);
                }else{
                    Scroll.X = Loc.X;
                    Scroll.Y = Loc.Y + Scroll.Y;
                    SetToWithinLimits(Scroll.Y);
                }
                Scroller->SetActualPosition(Scroll);
            }
        }

        void Scrollbar::ScrollBackScroll()
        {
            if(!ScrollBackLock)
            {
                Vector2 MousePos = InputQueryTool::GetMouseCoordinates();
                Vector2 ScPos = Scroller->GetActualPosition();
                Vector2 ScSize = Scroller->GetActualSize();
                if(Horizontal)
                {
                    if(MousePos.X < ScPos.X)//left
                    {
                        ScPos.X = ScPos.X - ScSize.X <= ScrollerLowerLimit ? ScrollerLowerLimit : ScPos.X - ScSize.X ;
                        Scroller->SetActualPosition(ScPos);
                    }
                    else if(MousePos.X > ScPos.X + ScSize.X)//right
                    {
                        ScPos.X = ScPos.X + (ScSize.X * 2) >= ScrollerUpperLimit ? ScrollerUpperLimit - ScSize.X : ScPos.X + (ScSize.X * 2) ;
                        Scroller->SetActualPosition(ScPos);
                    }
                }else{
                    if(MousePos.Y < ScPos.Y)//above
                    {
                        ScPos.Y = ScPos.Y - ScSize.Y <= ScrollerLowerLimit ? ScrollerLowerLimit : ScPos.Y - ScSize.Y ;
                        Scroller->SetActualPosition(ScPos);
                    }
                    else if(MousePos.Y > ScPos.Y + ScSize.Y)//below
                    {
                        ScPos.Y = ScPos.Y + (ScSize.Y * 2) >= ScrollerUpperLimit ? ScrollerUpperLimit - ScSize.Y : ScPos.Y + ScSize.Y ;
                        Scroller->SetActualPosition(ScPos);
                    }
                }
            }
        }

        void Scrollbar::Update(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredButton)
            {
                if(MetaCode::BUTTON_PRESSING == State || MetaCode::BUTTON_DOWN == State)
                {
                    if(HoveredButton == Scroller)
                    {
                        if(MetaCode::BUTTON_PRESSING == State)
                        {
                            ScrollerLock = false;
                        }
                        else if(MetaCode::BUTTON_DOWN == State)
                        {
                            Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
                            MouseScroll(Offset);
                        }
                    }
                    else if(HoveredButton == UpLeftButton)
                    {
                        if(MetaCode::BUTTON_PRESSING == State)
                        {
                            UpLeftLock = false;
                        }
                        else if(MetaCode::BUTTON_DOWN == State)
                        {
                            ButtonScroll(true);
                        }
                    }
                    else if(HoveredButton == DownRightButton)
                    {
                        if(MetaCode::BUTTON_PRESSING == State)
                        {
                            DownRightLock = false;
                        }
                        else if(MetaCode::BUTTON_DOWN == State)
                        {
                            ButtonScroll(false);
                        }
                    }
                }
            }
            else if(HoveredBack)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    ScrollBackLock = false;
                    ScrollBackScroll();
                }
                else if(MetaCode::BUTTON_DOWN == State)
                {
                    if(ScrollBackLock && !ScrollerLock)
                    {
                        Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
                        MouseScroll(Offset);
                    }else{
                        ScrollBackScroll();
                    }
                }
            }
            else if(Force && MetaCode::BUTTON_DOWN == State)
            {
                Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
                MouseScroll(Offset);
            }
            else if(MetaCode::BUTTON_LIFTING == State)
            {
                ScrollerLock = true;
                ScrollBackLock = true;
                UpLeftLock = true;
                DownRightLock = true;
            }
            CalculateScrollValue();
        }

        void Scrollbar::SetVisible(bool visible)
        {
            ScrollBack->SetVisible(visible);
            Scroller->SetVisible(visible);
            if(UpLeftButton)
                UpLeftButton->SetVisible(visible);
            if(DownRightButton)
                DownRightButton->SetVisible(visible);
            Visible = visible;
        }

        bool Scrollbar::IsVisible()
        {
            return Visible && Parent->IsVisible() && Parent->GetParent()->IsVisible();
        }

        void Scrollbar::Show()
        {
            ScrollBack->Show();
            Scroller->Show();
            if(UpLeftButton)
                UpLeftButton->Show();
            if(DownRightButton)
                DownRightButton->Show();
            Visible = true;
        }

        void Scrollbar::Hide()
        {
            ScrollBack->Hide();
            Scroller->Hide();
            if(UpLeftButton)
                UpLeftButton->Hide();
            if(DownRightButton)
                DownRightButton->Hide();
            Visible = false;
        }

        bool Scrollbar::CheckMouseHover()
        {
            if(!IsVisible())
                return false;
            if(Scroller->CheckMouseHover())
            {
                HoveredButton = Scroller;
                HoveredBack = NULL;
                //Update();
                return true;
            }
            else if(UpLeftButton->CheckMouseHover())
            {
                HoveredButton = UpLeftButton;
                HoveredBack = NULL;
                //Update();
                return true;
            }
            else if(DownRightButton->CheckMouseHover())
            {
                HoveredButton = DownRightButton;
                HoveredBack = NULL;
                //Update();
                return true;
            }
            else if(ScrollBack->CheckMouseHover())
            {
                HoveredButton = NULL;
                HoveredBack = ScrollBack;
                //Update();
                return true;
            }
            HoveredButton = NULL;
            HoveredBack = NULL;
            return false;
        }

        Real Scrollbar::GetScrollerValue()
        {
            return ScrollerValue;
        }

        void Scrollbar::SetIncrementDistance(const Real& IncDist)
        {
            IncrementDistance = IncDist;
        }

        void Scrollbar::SetScrollerSize(const Real& Size)
        {
            if(Size > 1 || Size < 0)
                return;
            Vector2 SB = ScrollBack->GetActualSize();
            Vector2 Sc = Scroller->GetActualSize();
            if(Horizontal)
            {
                Vector2 NewSize(SB.X * Size,Sc.Y);
                Scroller->SetActualSize(NewSize);
            }else{
                Vector2 NewSize(Sc.X,SB.Y * Size);
                Scroller->SetActualSize(NewSize);
            }
        }

        void Scrollbar::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 Adjusted = Position * Parent->GetParent()->GetViewportDimensions();
            SetLocation(Adjusted);
        }

        Vector2 Scrollbar::GetPosition()
        {
            return RelPosition;
        }

        void Scrollbar::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            SetLocation(Position);
        }

        Vector2 Scrollbar::GetActualPosition()
        {
            return (RelPosition * Parent->GetParent()->GetViewportDimensions());
        }

        void Scrollbar::SetSize(const Vector2& Size)
        {
            if(!IsValidDimensions(Size))
                return;
            RelSize = Size;
            Vector2 Adjusted = Size * Parent->GetParent()->GetViewportDimensions();
            SetHorizontal(Adjusted);
            if(ScrollStyle==SB_NoButtons)
            {
                ScrollBack->SetActualSize(Adjusted);
            }else{
                Vector2 Loc = RelPosition * Parent->GetParent()->GetViewportDimensions();
                CalculateOffsets(Adjusted);
                SetArea(Adjusted);
                SetLocation(Loc);
            }
        }

        Vector2 Scrollbar::GetSize()
        {
            return RelSize;
        }

        void Scrollbar::SetActualSize(const Vector2& Size)
        {
            if(!IsValidDimensions(Size))
                return;
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            SetHorizontal(Size);
            if(ScrollStyle==SB_NoButtons)
            {
                ScrollBack->SetActualSize(Size);
            }else{
                Vector2 Loc = RelPosition * Parent->GetParent()->GetViewportDimensions();
                CalculateOffsets(Size);
                SetArea(Size);
                SetLocation(Loc);
            }
        }

        Vector2 Scrollbar::GetActualSize()
        {
            return (RelSize * Parent->GetParent()->GetViewportDimensions());
        }

        Button* Scrollbar::GetScroller()
        {
            return Scroller;
        }

        Button* Scrollbar::GetUpLeftButton()
        {
            return UpLeftButton;
        }

        Button* Scrollbar::GetDownRightButton()
        {
            return DownRightButton;
        }

        Rectangle* Scrollbar::GetScrollBack()
        {
            return ScrollBack;
        }
    }
}

#endif
