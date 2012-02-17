//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uimenu_cpp
#define _uimenu_cpp

#include "uimenu.h"
#include "uimenuwindow.h"

#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        Menu::Menu(ConstString name, const RenderableRect& Rect, Layer* PLayer)
            : Widget(name,PLayer)
        {
            Type = Widget::W_Menu;
            RelPosition.X = -1;
            RelPosition.Y = -1;
            RelSize.X = -1;
            RelSize.Y = -1;

            ConstString NewName = name+"root";
            RootWindow = new UI::MenuWindow(NewName,Rect,this,ParentLayer);
            MenuStack.push_back(RootWindow);
            AddSubRenderable(0,RenderablePair(NULL,RootWindow));
        }

        Menu::~Menu()
        {
            delete RootWindow;
            MenuStack.clear();
        }

        void Menu::UpdateImpl(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredSubWidget)
            {
                Button* button = HoveredSubWidget->GetHoveredButton();
                if(button && State == MetaCode::BUTTON_LIFTING)
                {
                    UI::MenuWindow* MenWin = static_cast<UI::MenuWindow*>(HoveredSubWidget);
                    UI::MenuWindow* ChildMenWin = MenWin->GetWindowOfAccessButton(button);
                    if(ChildMenWin)
                    {
                        if(!ChildMenWin->IsVisible())
                        {
                            if(MenuStack.back()->GetAutoHide())
                                MenuStack.back()->Hide();
                            MenuStack.push_back(ChildMenWin);
                            AddSubRenderable(SubRenderables.size(),RenderablePair(NULL,ChildMenWin));
                            ChildMenWin->Show();
                            return;
                        }else{
                            RollMenuBackToWindow(MenWin);
                        }
                    }
                    else if(button == MenWin->GetBackButton())
                    {
                        MenWin->Hide();
                        MenuStack.pop_back();
                        SubRenderables.erase(SubRenderables.size() - 1);
                        if(!MenuStack.back()->IsVisible())
                            MenuStack.back()->Show();
                        return;
                    }
                }
            }
        }

        void Menu::SetVisibleImpl(bool visible)
        {
            for( std::vector<UI::MenuWindow*>::iterator it = MenuStack.begin() ; it != MenuStack.end() ; it++ )
            {
                if(visible)
                {
                    if(!(*it)->GetAutoHide())
                        (*it)->SetVisible(visible);
                }else{
                    (*it)->SetVisible(visible);
                }
            }
        }

        bool Menu::CheckMouseHoverImpl()
        {
            bool HoverRet = false;
            for( std::vector<UI::MenuWindow*>::reverse_iterator it = MenuStack.rbegin() ; it != MenuStack.rend() ; it++ )
            {
                if((*it)->IsVisible())
                {
                    if((*it)->CheckMouseHover())
                    {
                        HoveredSubWidget = (*it);
                        HoveredButton = HoveredSubWidget->GetHoveredButton();
                        HoverRet = true;
                    }
                }
            }
            return HoverRet;
        }

        void Menu::RollMenuBackToWindow(UI::MenuWindow* Win)
        {
            for( std::vector<UI::MenuWindow*>::reverse_iterator rit = MenuStack.rbegin() ; rit != MenuStack.rend() ; rit++ )
            {
                if(!(*rit)->IsVisible())
                    (*rit)->Show();
                if( RootWindow == (*rit) )
                    break;
                if( Win != (*rit))
                {
                    (*rit)->Hide();
                    MenuStack.pop_back();
                    SubRenderables.erase(SubRenderables.size() - 1);
                }else{
                    break;
                }
            }
        }

        void Menu::SetPosition(const Vector2& Position)
        {
            MenuStack.back()->SetPosition(Position);
        }

        Vector2 Menu::GetPosition()
        {
            return MenuStack.back()->GetPosition();
        }

        void Menu::SetActualPosition(const Vector2& Position)
        {
            MenuStack.back()->SetActualPosition(Position);
        }

        Vector2 Menu::GetActualPosition()
        {
            return MenuStack.back()->GetActualPosition();
        }

        void Menu::SetSize(const Vector2& Size)
        {
            MenuStack.back()->SetSize(Size);
        }

        Vector2 Menu::GetSize()
        {
            return MenuStack.back()->GetSize();
        }

        void Menu::SetActualSize(const Vector2& Size)
        {
            MenuStack.back()->SetActualSize(Size);
        }

        Vector2 Menu::GetActualSize()
        {
            return MenuStack.back()->GetActualSize();
        }

        void Menu::UpdateDimensions()
        {
            RootWindow->UpdateDimensions();
        }

        MenuWindow* Menu::GetRootWindow()
        {
            return RootWindow;
        }

        MenuWindow* Menu::GetTopWindow()
        {
            return MenuStack.back();
        }

        void Menu::_AppendVertices(ScreenVertexData& Vertices)
        {
            for( Whole X = 0 ; X < MenuStack.size() ; ++X )
            {
                if( MenuStack[X]->IsVisible() )
                    MenuStack[X]->_AppendVertices(Vertices);
            }
        }
    }//UI
}//Mezzanine

#endif
