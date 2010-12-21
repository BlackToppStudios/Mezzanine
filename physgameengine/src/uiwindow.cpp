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
#ifndef _uiwindow_cpp
#define _uiwindow_cpp

#include "uiwindow.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uicaption.h"
#include "uimarkuptext.h"
#include "uilinelist.h"
#include "uirectangle.h"
#include "uibutton.h"
#include "uitextbutton.h"
#include "uiscrollbar.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace phys
{
    namespace UI
    {
        Window::Window(ConstString& name, Vector2 Position, Vector2 Size, UILayer* Layer)
            : Widget(name,Layer)
        {
            RelPosition = Position;
            RelSize = Size;
            Type = Widget::Window;

            WindowBack = new Rectangle(Position,Size,Parent);
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

        }

        void Window::SetLocation(Vector2 &Position)
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

        void Window::SetArea(Vector2 &Size)
        {
            Vector2 OldSize = WindowBack->GetActualSize();
            WindowBack->SetActualSize(Size);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
            }
        }

        Vector2 Window::CalculateOffset(Vector2 NewSize, Vector2 OldSize, Vector2 Offset, UI::ResizeableAnchor Anchor)
        {
            Vector2 NewOffset;
            switch (Anchor)
            {
                case RA_AnchorTopLeft:
                {
                    NewOffset = Offset;
                    break;
                }
                case RA_AnchorTop:
                {
                    break;
                }
                case RA_AnchorTopRight:
                {
                    break;
                }
                case RA_AnchorLeft:
                {
                    break;
                }
                case RA_AnchorMiddle:
                {
                    break;
                }
                case RA_AnchorRight:
                {
                    break;
                }
                case RA_AnchorBottomLeft:
                {
                    break;
                }
                case RA_AnchorBottom:
                {
                    break;
                }
                case RA_AnchorBottomRight:
                {
                    break;
                }
                default:
                    return NewOffset;
            }
            return NewOffset;
        }

        void Window::SetVisible(bool visible)
        {
            if(Visible == visible)
                return;
            WindowTitle->SetVisible(visible);
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
            return Visible;
        }

        void Window::Show()
        {
            if(Visible)
                return;
            WindowTitle->Show();
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
            WindowTitle->Hide();
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

        bool Window::CheckMouseHover()
        {

        }

        void Window::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Vector2 ActPos = Position * Manager->GetWindowDimensions();
            SetLocation(ActPos);
        }

        Vector2 Window::GetPosition()
        {
            return RelPosition;
        }

        void Window::SetActualPosition(Vector2 Position)
        {
            RelPosition = Position / Manager->GetWindowDimensions();
            SetLocation(Position);
        }

        Vector2 Window::GetActualPosition()
        {
            return RelPosition * Manager->GetWindowDimensions();
        }

        void Window::SetSize(Vector2 Size)
        {
            RelSize = Size;
            Vector2 ActSize = Size * Manager->GetWindowDimensions();
            SetArea(ActSize);
        }

        Vector2 Window::GetSize()
        {
            return RelSize;
        }

        void Window::SetActualSize(Vector2 Size)
        {
            RelSize = Size / Manager->GetWindowDimensions();
            SetArea(Size);
        }

        Vector2 Window::GetActualSize()
        {
            return RelSize * Manager->GetWindowDimensions();
        }
    }//UI
}//phys

#endif
