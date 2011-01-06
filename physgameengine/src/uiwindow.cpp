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
#include "uilistbox.h"
#include "uibuttonlistbox.h"
#include "uicheckbox.h"
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
            if(!Force)
                SubWidgetUpdate();
            MetaCode::ButtonState State = Manager->GetInputQueryer()->GetMouseButtonState(1);
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

        Vector2 Window::CalculateOffset(Vector2 NewSize, Vector2 OldSize, Vector2 EleOffset, UI::ResizeableAnchor Anchor)
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

        Vector2 Window::CalculateSize(Vector2 NewSize, Vector2 OldSize, Vector2 EleSize, UI::ResizeableTether Tether)
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
            return Visible;
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
            if(!Visible)
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

        Button* Window::CreateButton(ConstString& Name, Vector2 Position, Vector2 Size)
        {
            Vector2 Offset = Position - RelPosition;
            OffsetButtonInfo button(new Button(Name, Position, Size, Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(button);
            return button.Object;
        }

        TextButton* Window::CreateTextButton(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, ConstString Text)
        {
            Vector2 Offset = Position - RelPosition;
            TextButton* tbutton = new TextButton(Name, Position, Size, Glyph, Text, Parent);
            OffsetButtonInfo tbuttoninfo(tbutton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(tbuttoninfo);
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

        Button* Window::GetButton(Whole Index)
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

        OffsetButtonInfo* Window::GetOffsetButtonInfo(Whole Index)
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

        Rectangle* Window::CreateRectangle(Vector2 Position, Vector2 Size)
        {
            Vector2 Offset = Position - RelPosition;
            OffsetRectangleInfo rectangle(new Rectangle(Position,Size,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Rectangles.push_back(rectangle);
            return rectangle.Object;
        }

        Rectangle* Window::GetRectangle(Whole Index)
        {
            return Rectangles[Index].Object;
        }

        OffsetRectangleInfo* Window::GetOffsetRectangleInfo(Whole Index)
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

        Caption* Window::CreateCaption(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text)
        {
            Vector2 Offset = Position - RelPosition;
            OffsetCaptionInfo caption(new Caption(Name,Position,Size,Glyph,Text,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Captions.push_back(caption);
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

        Caption* Window::GetCaption(Whole Index)
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

        OffsetCaptionInfo* Window::GetOffsetCaptionInfo(Whole Index)
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

        MarkupText* Window::CreateMarkupText(ConstString& Name, Vector2 Position, Whole Glyph, String Text)
        {
            Vector2 Offset = Position - RelPosition;
            OffsetMarkupTextInfo markup(new MarkupText(Name,Position,Glyph,Text,Parent),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            MarkupTexts.push_back(markup);
            return markup.Object;
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

        MarkupText* Window::GetMarkupText(Whole Index)
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

        OffsetMarkupTextInfo* Window::GetOffsetMarkupTextInfo(Whole Index)
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

        Widget* Window::GetWidget(Whole Index)
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

        OffsetWidgetInfo* Window::GetOffsetWidgetInfo(Whole Index)
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
                    /*Widget::WidgetType Type = ToBeDestroyed->GetType();
                    switch (Type)
                    {
                        case Widget::Scrollbar:
                        {
                            UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*> (ToBeDestroyed);
                            delete Scroll;
                            return;
                        }
                        case Widget::CheckBox:
                        {
                            UI::CheckBox* Check = static_cast<UI::CheckBox*> (ToBeDestroyed);
                            delete Check;
                            return;
                        }
                        case Widget::ButtonListBox:
                        {
                            UI::ButtonListBox* ButtonList = static_cast<UI::ButtonListBox*> (ToBeDestroyed);
                            delete ButtonList;
                            return;
                        }
                        case Widget::ListBox:
                        {
                            UI::ListBox* List = static_cast<UI::ListBox*> (ToBeDestroyed);
                            delete List;
                            return;
                        }
                        default:
                            return;
                    }*/
                    Widgets.erase(it);
                }
            }
        }

        UI::Scrollbar* Window::CreateScrollbar(ConstString& Name, Vector2 Position, Vector2 Size, UI::ScrollbarStyle Style)
        {
            Vector2 Offset = Position - RelPosition;
            UI::Scrollbar* Scroll = new UI::Scrollbar(Name,Position,Size,Style,Parent);
            OffsetWidgetInfo ScrollInfo(Scroll,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ScrollInfo);
            return Scroll;
        }

        UI::CheckBox* Window::CreateCheckBox(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, String &LabelText)
        {
            Vector2 Offset = Position - RelPosition;
            UI::CheckBox* Check = new UI::CheckBox(Name,Position,Size,Glyph,LabelText,Parent);
            OffsetWidgetInfo CheckInfo(Check,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(CheckInfo);
            return Check;
        }

        UI::ButtonListBox* Window::CreateButtonListBox(ConstString& Name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollStyle)
        {
            Vector2 Offset = Position - RelPosition;
            UI::ButtonListBox* BLB = new UI::ButtonListBox(Name,Position,Size,ScrollbarWidth,ScrollStyle,Parent);
            OffsetWidgetInfo BLBInfo(BLB,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(BLBInfo);
            return BLB;
        }

        UI::ListBox* Window::CreateListBox(ConstString& Name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollStyle)
        {
            Vector2 Offset = Position - RelPosition;
            UI::ListBox* LB = new UI::ListBox(Name,Position,Size,ScrollbarWidth,ScrollStyle,Parent);
            OffsetWidgetInfo LBInfo(LB,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(LBInfo);
            return LB;
        }
    }//UI
}//phys

#endif
